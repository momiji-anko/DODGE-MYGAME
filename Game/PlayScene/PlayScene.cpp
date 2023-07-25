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
PlayScene::PlayScene(GameMain* parent)
	:
	m_commonState{},
	m_countDownTime(0.0f),
	m_musicID(0),
	m_pAdx2(),
	m_pModel{},
	m_obstacleManager{},
	m_itemManager{},
	m_stageManager{},
	m_pCamera{},
	m_spriteBatch{},
	m_countDownTexture{},
	m_shadowMap{},
	m_fadeInOut{},
	m_aliveTime{},
	m_playerMode(),
	m_stageNum(),
	m_playerManager{},
	m_parent{parent}
{

	m_pCamera = std::make_unique<Camera>();
}

/// <summary>
/// デストラクタ
/// </summary>
PlayScene::~PlayScene()
{
	
	
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
	m_obstacleManager->Initialize(m_stageNum);

	//シャドウマップ作成
	m_shadowMap = std::make_unique<ShadowMap>();
	m_shadowMap->Initialize(device, L"Resources/Shaders");

	//ステージマネージャー作成
	m_stageManager = std::make_unique<StageManager>();
	m_stageManager->Initialize(m_stageNum);
	m_stageManager->SetShadowMap(m_shadowMap.get());

	//アイテムマネージャー作成
	m_itemManager = std::make_unique< ItemManager>();
	m_itemManager->Initialize();
	m_itemManager->SetStageManeger(m_stageManager.get());

	//カウントダウン初期化	
	m_countDownTime = COUNT_DOWN_TIME_S;

	//Adx取得
	m_pAdx2 = &ADX2::GetInstance();
	//初期化	
	m_pAdx2->Initialize(
		"Resources/Sounds/DODGESound.acf",
		"Resources/Sounds/CueSheet_0.acb"
	);


	//BGMを鳴らす
	m_musicID = m_pAdx2->Play(CRI_CUESHEET_0_PLAY);

	//キーボードステートトラッカーの生成
	m_keyboardStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	GameContext& gameContext = GameContext::GetInstance();
	//コモンステートをゲームコンテキストにセット
	gameContext.SetCommonState(m_commonState.get());
	//キーボードステートトラッカーをゲームコンテキストにセット
	gameContext.SetKeyboardStateTracker(m_keyboardStateTracker.get());

	//プレイヤーマネージャーの作成
	m_playerManager = std::make_unique<PlayerManager>(m_playerMode,m_stageManager.get(),m_itemManager.get(),m_obstacleManager.get());
	//初期化
	m_playerManager->Initialize();

	//アライブタイム取得
	m_aliveTime = &AliveTimer::GetInstance();
	//初期化	
	m_aliveTime->Initialize();
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>

void PlayScene::Update(const DX::StepTimer& timer)
{
	//経過時間
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	//キーボードステートトラッカーの更新
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_keyboardStateTracker->Update(keyState);

	//tabキーを押している状態か確認
	m_isTabKey = keyState.IsKeyDown(DirectX::Keyboard::Keys::Tab);

	//押している場合これ以降の処理を行わない
	if (m_isTabKey)
	{
		return ;
	}

	//フェード更新
	m_fadeInOut->Update(timer);
	
	//フェードアウトしたならリザルトシーンへ移行
	if (m_fadeInOut->ISClose())
	{
		m_parent->SceneChange(m_parent->GetResultScene());
	}

	//フェードインするまで処理しない
	if (m_fadeInOut->ISOpen() == false)
	{
		return ;
	}
	
	//カウントダウンを経過時間で引く
	m_countDownTime -= elapsedTime;

	//カウントダウンが０になるまで処理しない
	if (m_countDownTime > 0)
	{
		return ;
	}
	
	//アライブタイム更新
	m_aliveTime->Update(timer);

	//プレイヤー更新
	m_playerManager->Update(timer);

	//障害物マネージャー更新
	m_obstacleManager->Update(timer);
		
	//障害物マネージャーにプレイヤーの座標を送る
	m_obstacleManager->SetPlayerPosition(m_playerManager->GetRandomPlayerPosition());

	//アイテムマネージャー更新
	m_itemManager->Update(timer);

	//ステージマネージャー更新
	m_stageManager->Update(timer);
	

	//全てプレイヤーが死んでいればフェードアウトする
	if (m_playerManager->GetPlayerIsAllDaed())
	{
		m_fadeInOut->FadeOut();
	}
	
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
	m_stageManager->Draw(m_pCamera.get());

	//障害物マネージャー描画
	m_obstacleManager->Draw(m_pCamera.get());

	//アイテムマネージャー描画
	m_itemManager->Draw(m_pCamera.get());
	
	
	//プレイヤーの描画
	m_playerManager->Draw(m_pCamera.get());

	//画像の描画
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	//タイムの表示
	m_aliveTime->Draw();

	//カウントダウン描画
	DrawCountDown();
	
	
	//tabキーが押している状態であれば操作方法を表示する
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
	ModelManager::GetInstance().Reset();

	m_pAdx2->Finalize();
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
	//スプライトバッチをゲームコンテキストにセット
	GameContext::GetInstance().SetSpriteBatcth(m_spriteBatch.get());

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

	//プレイヤーモードの取得
	m_playerMode = GameContext::GetInstance().GetPlayerMode();


	//１ｐモードと２ｐモードの操作方法画像ファイルパス
	std::wstring playerMoveKeyTex[2] = { 
		{L"Resources/Textures/1playerMoveKey.png"},
		{L"Resources/Textures/2playerMoveKey.png"}
	};

	int tes = static_cast<int>(m_playerMode) - 1;

	//モードによって読み込む画像を変える
	m_playerMoveKey = textureManager.LoadTexture(playerMoveKeyTex[tes]);

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
	m_playerManager->PlayerShadow(m_shadowMap.get(), lightView, projection);

	//アイテムの影
	m_itemManager->Shadow(m_shadowMap.get(), lightView, projection);

	
	m_shadowMap->End(context, lightView * projection);

}
