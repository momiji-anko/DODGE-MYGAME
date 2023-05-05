/*
プレイシーン
*/
#include "pch.h"

#include "DeviceResources.h"
#include "../GameMain.h"
#include "Libraries/MyLibraries/DebugCamera.h"
#include "Libraries/MyLibraries/GridFloor.h"
#include "PlayScene.h"
#include"Game/PlayScene/MyRandom.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Libraries/MyLibraries/TextureManager.h"
#include"Game/PlayScene/GameContext/GameContext.h"

const float PlayScene::COUNT_DOWN_TIME_S = 4.0f;

/// <summary>
/// コンストラクタ
/// </summary>
PlayScene::PlayScene()
	:
	m_commonState{},
	m_countDownTime(0.0f),
	m_musicID(0),
	m_pAdx2(),
	m_pModel{},
	m_obstacleManager{},
	m_itemManeger{},
	m_stageManeger{},
	m_pCamera{},
	m_spriteBatch{},
	m_countDownTexture{},
	m_shadowMap{},
	m_fadeInOut{},
	m_aliveTime{},
	m_players{},
	m_playerMode(),
	m_stageNum()
{

	m_pCamera = std::make_unique<Camera>();
}

/// <summary>
/// デストラクタ
/// </summary>
PlayScene::~PlayScene()
{
	
	m_pAdx2->Finalize();
}

/// <summary>
/// 初期化
/// </summary>
void PlayScene::Initialize()
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();

	//フェード作成
	m_fadeInOut = std::make_unique<Fade>();
	m_fadeInOut->Create();
	m_fadeInOut->Initialize(DirectX::SimpleMath::Vector3::Zero);
	m_fadeInOut->FadeIn();

	//障害物マネージャー作成
	m_obstacleManager = std::make_unique< ObstacleManager>();
	m_obstacleManager->Initialize( m_stageNum);

	//シャドウマップ作成
	m_shadowMap = std::make_unique<ShadowMap>();
	m_shadowMap->Initialize(device, L"Resources/Shaders");

	//ステージマネージャー作成
	m_stageManeger = std::make_unique<StageManager>();
	m_stageManeger->Initialize( m_stageNum);
	m_stageManeger->SetShadowMap(m_shadowMap.get());

	//アイテムマネージャー作成
	m_itemManeger = std::make_unique< ItemManager>();
	m_itemManeger->Initialize();
	m_itemManeger->SetStageManeger(m_stageManeger.get());

	//カウントダウン初期化	
	m_countDownTime = COUNT_DOWN_TIME_S;

	//Adx取得
	m_pAdx2 = &ADX2::GetInstance();
	//初期化	
	m_pAdx2->Initialize("Resources/Sounds/DODGESound.acf",
		"Resources/Sounds/CueSheet_0.acb");

	//プレイヤー作成
	CreatePlayer();

	//BGMを鳴らす
	m_musicID = m_pAdx2->Play(CRI_CUESHEET_0_PLAY);

	//アライブタイム取得
	m_aliveTime = &AliveTimer::GetInstance();
	//初期化	
	m_aliveTime->Initialize();

	//キーボードステートトラッカーの生成
	m_keyboardStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	GameContext& gameContext = GameContext::GetInstance();
	//コモンステートをゲームコンテキストにセット
	gameContext.SetCommonState(m_commonState.get());
	//スプライトバッチをゲームコンテキストにセット
	gameContext.SetSpriteBatcth(m_spriteBatch.get());
	//キーボードステートトラッカーをゲームコンテキストにセット
	gameContext.SetKeyboardStateTracker(m_keyboardStateTracker.get());

}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
/// <returns>次のシーン</returns>
GAME_SCENE PlayScene::Update(const DX::StepTimer& timer)
{
	//経過時間
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	//キーボードステートトラッカーの更新
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_keyboardStateTracker->Update(keyState);

	m_isTabKey = keyState.IsKeyDown(DirectX::Keyboard::Keys::Tab);

	if (m_isTabKey)
	{
		return GAME_SCENE::NONE;
	}

	//フェード更新
	m_fadeInOut->Update(timer);
	
	//フェードアウトしたならリザルトシーンへ移行
	if (m_fadeInOut->ISClose())
	{
		return GAME_SCENE::RESULT;
	}

	//フェードインするまで処理しない
	if (m_fadeInOut->ISOpen() == false)
	{
		return GAME_SCENE::NONE;
	}
	
	//カウントダウンを経過時間で引く
	m_countDownTime -= elapsedTime;

	//カウントダウンが０になるまで処理しない
	if (m_countDownTime > 0)
	{
		return GAME_SCENE::NONE;
	}
	
	//アライブタイム更新
	m_aliveTime->Update(timer);

	//プレイヤー更新
	for (std::vector<std::unique_ptr<Player>>::iterator player = m_players.begin(); player != m_players.end(); ++player)
	{
		(*player)->Update(timer);
	}

	//障害物マネージャー更新
	m_obstacleManager->Update(timer);
	
	//障害物に渡すプレイヤーの番号
	size_t randomPlayer = static_cast<size_t>(MyRandom::GetIntRange(0, m_players.size() - 1));
	//障害物マネージャーにプレイヤーの座標を送る
	m_obstacleManager->SetPlayerPosition(m_players[randomPlayer]->GetPosition());

	//アイテムマネージャー更新
	m_itemManeger->Update(timer);

	//ステージマネージャー更新
	m_stageManeger->Update(timer);
	

	//全てプレイヤーが死んでいればフェードアウトする
	if (AllPlayerIsDead())
	{
		m_fadeInOut->FadeOut();
		
	}

	return GAME_SCENE::NONE;
}

/// <summary>
/// 描画
/// </summary>
void PlayScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_commonState->DepthRead(), 0);


	// 天球モデル描画
	m_pModel->Draw(context, *m_commonState.get(), DirectX::SimpleMath::Matrix::Identity, m_pCamera->GetViewMatrix(),m_pCamera->GetProjectionMatrix());

	//ステージマネージャー描画
	m_stageManeger->Draw(m_pCamera.get());

	//障害物マネージャー描画
	m_obstacleManager->Draw(m_pCamera.get());

	//アイテムマネージャー描画
	m_itemManeger->Draw(m_pCamera.get());
	
	
	//プレイヤーの描画
	for (std::vector<std::unique_ptr<Player>>::iterator player = m_players.begin(); player != m_players.end(); ++player)
	{
		(*player)->Draw(m_pCamera.get());
	}

	//画像の描画
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	//タイムの表示
	m_aliveTime->Draw();

	//カウントダウン描画
	DrawCountDown();
	
	//プレイヤーの盾描画
	for (std::vector<std::unique_ptr<Player>>::iterator player = m_players.begin(); player != m_players.end(); ++player)
	{
		(*player)->TextureDraw(m_spriteBatch.get());
	}
	
	if (m_isTabKey)
	{
		m_spriteBatch->Draw(m_playerMoveKey.Get(), DirectX::SimpleMath::Vector2::Zero, nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero, 1.0f);
	}

	//画像描画終了
	m_spriteBatch->End();
	
	//フェード描画
	m_fadeInOut->Render();

	//影生成
	CreateShadow();
}

/// <summary>
/// 終了処理
/// </summary>
void PlayScene::Finalize()
{
}

/// <summary>
/// リソース読み込み
/// </summary>
void PlayScene::LoadResources()
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// スプライトバッチ::デバッグ情報の表示に必要
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);


	//	コモンステート::D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	//モデルマネージャー取得
	ModelManager& modelManager = ModelManager::GetInstance();

	//天球モデル読み込み
	m_pModel = modelManager.LoadModel(L"Resources/Models/tenkyuu.cmo");
	
	//天球モデルの暗いところを消す
	m_pModel->UpdateEffects([&](DirectX::IEffect* effect)
		{
			DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
			if (lights)
			{
				// ライトの影響をなくす
				lights->SetAmbientLightColor(DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.0f));
				lights->SetLightEnabled(0, false);
				lights->SetLightEnabled(1, false);
				lights->SetLightEnabled(2, false);
			}
			DirectX::BasicEffect* basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			if (basicEffect)
			{
				// エミッション色を白に設定する
				basicEffect->SetEmissiveColor(DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
			}
		});

	//テクスチャマネージャー終了
	TextureManager& textureManager = TextureManager::GetInstance();
	//カウントダウンの画像
	m_countDownTexture = textureManager.LoadTexture(L"Resources/Textures/num.png");

	std::wstring playerMoveKeyTex[2] = { 
		{L"Resources/Textures/1playerMoveKey.png"},
		{L"Resources/Textures/2playerMoveKey.png"}
	};

	m_playerMoveKey = textureManager.LoadTexture(playerMoveKeyTex[static_cast<int>(m_playerMode) - 1]);

}

/// <summary>
/// プレイヤーの作成
/// </summary>
void PlayScene::CreatePlayer()
{


	//プレイヤーのキーデータ
	std::vector<std::vector<DirectX::Keyboard::Keys>> playerKeyData =
	{
		//プレイヤー１のキーデータ(右、左、前、後ろ、ジャンプ)
		{DirectX::Keyboard::Keys::Right,DirectX::Keyboard::Keys::Left,DirectX::Keyboard::Keys::Up,DirectX::Keyboard::Keys::Down,DirectX::Keyboard::Keys::RightShift},	
		//プレイヤー２のキーデータ(右、左、前、後ろ、ジャンプ)
		{DirectX::Keyboard::Keys::D,DirectX::Keyboard::Keys::A,DirectX::Keyboard::Keys::W,DirectX::Keyboard::Keys::S,DirectX::Keyboard::Keys::Space}
	};

	//プレイヤーのモデルファイルパス
	std::vector<std::vector<std::wstring>>playerModelFile = {
		//プレイヤー１のモデルファイルパス（アイドル状態、左足出している状態、右足出している状態、ジャンプしている状態）
		{ L"Resources/Models/playeraidoru.cmo",L"Resources/Models/playerhidari.cmo",L"Resources/Models/playermigiasi.cmo",L"Resources/Models/playerjanp.cmo"},
		//プレイヤー２のモデルファイルパス（アイドル状態、左足出している状態、右足出している状態、ジャンプしている状態）
		{L"Resources/Models/Player2idoru.cmo",L"Resources/Models/Player2hidari.cmo",L"Resources/Models/Player2Migi.cmo",L"Resources/Models/Player2Janp.cmo"}
	};

	//二人用プレイヤースタート座標
	DirectX::SimpleMath::Vector3 playersStartPos[2] =
	{
		DirectX::SimpleMath::Vector3{3.0f,0.0f,6.0f} ,
		DirectX::SimpleMath::Vector3{-3.0f,0.0f,6.0f}
	};

	//プレイヤーモードが1人モードなら一人用のスタート座標、キーデータにする
	if (m_playerMode == GameMain::PlayerMode::Player1)
	{
		playersStartPos[0] = DirectX::SimpleMath::Vector3(0.0f, 7.0f, 6.0f);
		playerKeyData[0] = { DirectX::Keyboard::Keys::Right,DirectX::Keyboard::Keys::Left,DirectX::Keyboard::Keys::Up,DirectX::Keyboard::Keys::Down,DirectX::Keyboard::Keys::Space };
	}
	//プレイヤーモードの数プレイヤーを作成
	for (int i = 0; i < static_cast<int>(m_playerMode); i++)
	{
		m_players.push_back(std::make_unique<Player>());

		//ステージマネージャー設定
		m_players[i]->SetStageManeger(m_stageManeger.get());
		//アイテムマネージャー設定
		m_players[i]->SetIteManeger(m_itemManeger.get());
		//障害物マネージャー設定
		m_players[i]->SetObstacleManager(m_obstacleManager.get());
		
		//プレイヤーモデルファイルパス設定
		m_players[i]->SetPlayerModelFile(playerModelFile[i]);
		//プレイヤーの移動キー設定
		m_players[i]->SetKeys(playerKeyData[i]);
		//プレイヤーID設定
		m_players[i]->SetTypeInt(i);
		
		//初期化
		m_players[i]->Initialize(DirectX::SimpleMath::Vector3::Zero, playersStartPos[i], DirectX::SimpleMath::Vector3(2.0f, 2.0f, 2.0f), DirectX::SimpleMath::Vector3::Zero, true, nullptr, nullptr);
	}


}

/// <summary>
/// 全てプレイヤーが死んでいるか確認
/// </summary>
/// <returns>true=全て死んでいる、false=全て死んでいない</returns>
bool PlayScene::AllPlayerIsDead()
{
	//プレイヤーのデスカウント
	int  deathCount = 0;

	//全てのプレイヤーが死んでいるか確認
	for (std::vector<std::unique_ptr<Player>>::iterator player = m_players.begin(); player != m_players.end(); ++player)
	{
		//アクティブ状態状態ではなければカウントを増やす
		if (!(*player)->IsActive())
		{
			deathCount++;
		}
	}

	//デスカウントがプレイヤーの同じ数であれば全てのプレイヤーが死んでいる
	if (deathCount == m_players.size())
	{
		//全てのプレイヤーが死んでいる
		return true;
	}

	//全てのプレイヤーが死んでいない
	return false;
}

/// <summary>
/// カウントダウンの表示
/// </summary>
void PlayScene::DrawCountDown()
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//	ウィンドウサイズの取得
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	//カウントダウン画像の切り取り位置
	RECT countDownRect[] = {
	{0	,70,180,130},//START
	{52	,0	,97	,64},//1
	{96	,0	,140,64},//2
	{140,0	,190,64},//3
	{190,0	,243,64},//4

	};

	//カウントダウンの座標
	DirectX::SimpleMath::Vector2 countDownPos{ width / 2, height / 2 };

	//画像の中心座標
	DirectX::SimpleMath::Vector2 centerPos{ 26,60 };

	//カウントダウンのが０以上であれば表示
	if (m_countDownTime >= 0)
	{
		//カウントダウンが０の場合中心座標を変える
		if (static_cast<int>(m_countDownTime) == 0)
		{
			centerPos.x += countDownRect[0].right / 2;
		}

		m_spriteBatch->Draw(m_countDownTexture.Get(), countDownPos, &countDownRect[static_cast<int>(m_countDownTime)], DirectX::Colors::White, 0.0f, centerPos, 2.0f);

	}

}

/// <summary>
/// 影生成
/// </summary>
void PlayScene::CreateShadow()
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//シャドウマップ
	m_shadowMap->Begin(context);

	//ライトの座標
	DirectX::SimpleMath::Vector3 lightPos(-10.1f, 15.0f, 10.1f);
	
	//ビュー行列
	DirectX::SimpleMath::Matrix lightView = DirectX::SimpleMath::Matrix::CreateLookAt(lightPos, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector3::Up);
	//プロジェクション行列
	DirectX::SimpleMath::Matrix projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(90.0f), 1.0f, 1.0f, 400.0f);
	
	//障害物の影
	m_obstacleManager->ObstacleShadow(m_shadowMap.get(), lightView, projection);

	//プレイヤーの影
	for (std::vector<std::unique_ptr<Player>>::iterator player = m_players.begin(); player != m_players.end(); ++player)
	{
		(*player)->CreateShadow(m_shadowMap.get(), lightView, projection);
	}

	//アイテムの影
	m_itemManeger->Shadow(m_shadowMap.get(), lightView, projection);

	
	m_shadowMap->End(context, lightView * projection);

}
