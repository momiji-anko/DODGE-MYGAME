/*//
タイトルシーン
*/

#include "pch.h"

#include <WICTextureLoader.h>
#include <SimpleMath.h>

#include "../GameMain.h"
#include "DeviceResources.h"

#include "TitleScene.h"
#include"Libraries/MyLibraries/Camera.h"
#include"Libraries/MyLibraries/TextureManager.h"
#include"Game/PlayScene/GameContext/GameContext.h"
#include"Libraries/MyLibraries/FileLoadManager.h"

//タイトルの移動時間
const float TitleScene::MOVE_TIME = 3.0f;
//タイトルの移動角度
const float TitleScene::MOVE_RORATION = 0.1f;
//タイトルの初期角度
const float TitleScene::RORATION_ZERO = 0.0f;
//タイトルの回転ルーチンのリセット
const int TitleScene::ROTATION_RESET_ROUTINE = 2;
//カメラの回転速度
const float TitleScene::CAMERA_ROT_SPEED = 0.001f;
//アルファの最大値
const float TitleScene::ALPHA_MAX_NUM = 1.0f;
//アルファの最小値
const float TitleScene::ALPHA_MIN_NUM = 0.0f;

/// <summary>
/// コンストラクタ
/// </summary>
TitleScene::TitleScene()
	:
	m_alpha(1.0f),
	m_alphaSpeed(-0.01f),
	m_cameraRot(0.0f),
	m_titleTimer_s(0.0f),
	m_titleRoutine(0),
	m_titlePosition{},
	m_titleRotetion(0.0f),
	m_titleSelect(TitleState::FADEIN),
	m_stageNum(0),
	m_stageManager{},
	m_playerMode(GameMain::PlayerMode::Player1),
	m_titleAlpha(0.7f),
	m_pAdx2(),
	m_musicID(0),
	m_stageTextureBasePosition(DirectX::SimpleMath::Vector2::Zero),
	m_stageTexturetime(5),
	m_nextPosition(DirectX::SimpleMath::Vector2::Zero),
	m_prePosition(DirectX::SimpleMath::Vector2::Zero)
{
}

/// <summary>
/// コンストラクタ
/// </summary>
TitleScene::~TitleScene()
{
	//Adx2の終了処理
	m_pAdx2->Finalize();
}

/// <summary>
/// 初期化
/// </summary>
void TitleScene::Initialize()
{

	
	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//Adx取得
	m_pAdx2 = &ADX2::GetInstance();
	//Adx初期化
	m_pAdx2->Initialize("Resources/Sounds/DODGESound.acf",
		"Resources/Sounds/CueSheet_0.acb");
	//タイトルBGMを流す
	m_musicID = m_pAdx2->Play(CRI_CUESHEET_0_TITLE);

	// ウィンドウサイズの取得
	RECT size = pDR->GetOutputSize();
	//タイトルのポジションを初期化
	m_titlePosition.resize(2);
	m_titlePosition[0] = DirectX::SimpleMath::Vector2(static_cast<float>(-size.right), 0);
	m_titlePosition[1] = DirectX::SimpleMath::Vector2(static_cast<float>(size.right), 0);
	

	//フェードの作成
	m_fadeInOut = std::make_unique<Fade>();
	m_fadeInOut->Create();
	m_fadeInOut->Initialize(DirectX::SimpleMath::Vector3::Zero);
	
	//キーボードステートトラッカー取得
	m_keyboardStateTracker = std::make_unique < DirectX::Keyboard::KeyboardStateTracker >();

	GameContext::GetInstance().SetCommonState(m_commonState.get());
	GameContext::GetInstance().SetKeyboardStateTracker(m_keyboardStateTracker.get());
	GameContext::GetInstance().SetSpriteBatcth(m_spriteBatch.get());

	m_stageSelect = std::make_unique<StageSelect>();
	m_stageSelect->Initialize();
	m_stageSelect->SetStageManager(m_stageManager.get());
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
/// <returns>次のシーン</returns>
GAME_SCENE TitleScene::Update(const DX::StepTimer& timer)
{
	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	//キーボードステートトラッカーの更新
	m_keyboardStateTracker->Update(keyState);
	//フェードの更新
	m_fadeInOut->Update(timer);
	

	//カメラの回転
	m_cameraRot += CAMERA_ROT_SPEED;

	//タイトルの状態更新
	TitleStateUpdate(timer);

	//タイトル文字の動き
	TitleMove(timer);
	
	//アルファをアルファスピードで足す
	m_alpha += m_alphaSpeed;

	//アルファが０か１になったらアルファスピードを反転させる
	if (m_alpha < ALPHA_MIN_NUM || m_alpha > ALPHA_MAX_NUM)
	{
		//アルファスピードを反転させる
		m_alphaSpeed *= -1.0f;
	}
		
	//フェードアウトしている状態かつタイトルの状態がフェードアウト状態であればプレイシーンに移行する
	if (m_fadeInOut->ISClose() && TitleScene::TitleState::FADEOUT == m_titleSelect)
	{
		//BGMを止める
		m_pAdx2->Stop(m_musicID);
		//プレイシーンに移行
		return GAME_SCENE::PLAY;
	}
	return GAME_SCENE::NONE;
}

/// <summary>
/// 描画
/// </summary>
void TitleScene::Draw()
{
	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();


	// ビュー行列
	DirectX::SimpleMath::Matrix view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix projection;
	// カメラ座標
	DirectX::SimpleMath::Vector3 eye = {cos(m_cameraRot)*18,10,-sin(m_cameraRot)*18};
	// 注視点
	DirectX::SimpleMath::Vector3 target = DirectX::SimpleMath::Vector3::Zero;
	// 上向きベクトル
	DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::UnitY;
	// ビュー行列計算
	view = DirectX::SimpleMath::Matrix::CreateLookAt(eye, target, up);
	// ウィンドウサイズの取得
	RECT size = pDR->GetOutputSize();
	float aspectRatio = float(size.right) / float(size.bottom);

	//カメラの見える範囲の設定
	float nearPlane = 1.0f;
	float farPlane = 100.0f;
	float fov = 45.0f;
	//視野角をラジアンで取得
	float fieldOfView = DirectX::XMConvertToRadians(fov);
	//プロジェクション行列計算
	projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
	//カメラの作成
	Camera camera;
	//計算したプロジェクション行列を設定する
	camera.SetProjectionMatrix(projection);
	//計算したビュー行列を設定する
	camera.SetViewMatrix(view);
	
	//ステージを描画
	m_stageManager->Draw(&camera);
	
	
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	
	//PUSH_SPASE_KEYのカラー
	DirectX::SimpleMath::Vector4 pushColor{ 1.0f,1.0f,1.0f,m_alpha };

	//タイトル文字描画
	for (DirectX::SimpleMath::Vector2& position : m_titlePosition)
	{
		m_spriteBatch->Draw(m_titileTexture.Get(), position + DirectX::SimpleMath::Vector2(640, 220), nullptr, DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_titleAlpha), m_titleRotetion, DirectX::SimpleMath::Vector2(640, 220));
	}

	//CRIWAREの座標
	DirectX::SimpleMath::Vector2 CRIWARETexPos{ static_cast<float>(size.right) - 110.0f, static_cast<float>(size.bottom) - 110.0f };
	//CRIWAREの拡大率
	float CRIWAREScale = 0.1f;
	//CRIWAREの表示
	m_spriteBatch->Draw(m_CRIWARETexture.Get(), CRIWARETexPos, nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero, CRIWAREScale);
	
	//PUSH_SPASE_KEYの座標
	DirectX::SimpleMath::Vector2 pushSpaseKeyPos(0, 0);

	switch (m_titleSelect)
	{
	case TitleScene::TitleState::TITLE:
	
		//PUSH_SPASE_KEYの描画
		m_spriteBatch->Draw(m_pushTexture.Get(), pushSpaseKeyPos, nullptr, pushColor, 0.0f, DirectX::SimpleMath::Vector2::Zero);

		break;
	case TitleScene::TitleState::STAGESELECT:

		// ステージ選択の画像を描画
		//DrawStageSelect();
		m_stageSelect->Draw();

		break;

	case TitleScene::TitleState::MODESELECT:
	
		// モード選択の画像を描画
		DrawModeSelect();

		break;
	}

	m_spriteBatch->End();

	//フェードの描画
	m_fadeInOut->Render();

}

/// <summary>
/// タイトルの状態の更新
/// </summary>
void TitleScene::TitleStateUpdate(const DX::StepTimer& timer)
{
	//現在のタイトルの状態で処理を切り替える
	switch (m_titleSelect)
	{
	case TitleScene::TitleState::FADEIN:
		//フェードイン開始
		m_fadeInOut->FadeIn();

		//フェードインが終わったらタイトルの状態に変更
		if (m_fadeInOut->ISOpen())
		{
			m_titleSelect = TitleState::TITLE;
		}

		break;
	case TitleScene::TitleState::TITLE:

		//スペースキーを押したらステージセレクト状態に変更
		if (m_keyboardStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Space))
		{
			m_titleSelect = TitleScene::TitleState::STAGESELECT;
			//選択音を出す
			m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);

		}

		break;
	case TitleScene::TitleState::STAGESELECT:

		//ステージセレクトの更新
		//StageSelectUpdate(timer);
		if(m_stageSelect->Update(timer))
		{
			m_titleSelect = TitleScene::TitleState::MODESELECT;
		}


		break;
	case TitleScene::TitleState::MODESELECT:

		//モードセレクトの更新
		ModeSelectUpdate();

		break;
	case TitleScene::TitleState::FADEOUT:

		//フェードアウト開始
		m_fadeInOut->FadeOut();
		
		break;
	default:
		break;
	}

}

/// <summary>
/// タイトル文字の動き
/// </summary>
/// <param name="timer">タイマー</param>
void TitleScene::TitleMove(const DX::StepTimer& timer)
{
	//タイトル文字の移動後のアルファ値
	static const float MOVED_ALPHA_NUM = 1.0f;
	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// ウィンドウサイズの取得
	RECT size = pDR->GetOutputSize();

	//経過時間
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	//タイトルタイマーを経過時間で足す
	m_titleTimer_s += elapsedTime_s;

	//タイトルの移動前の座標
	DirectX::SimpleMath::Vector2 titleStartPosition = DirectX::SimpleMath::Vector2(static_cast<float>(size.right), 0.0f);
	//タイトルの移動後の座標
	DirectX::SimpleMath::Vector2 titleMovedPosition = DirectX::SimpleMath::Vector2::Zero;

	
	switch (m_titleRoutine)
	{
	case 0:
		//ラープで移動
		m_titlePosition[0] = DirectX::SimpleMath::Vector2::Lerp(-titleStartPosition, titleMovedPosition, easeOutCubic(m_titleTimer_s / MOVE_TIME));
		m_titlePosition[1] = DirectX::SimpleMath::Vector2::Lerp(titleStartPosition, titleMovedPosition, easeOutCubic(m_titleTimer_s / MOVE_TIME));
		break;
	case 1:
		//タイマー初期化
		m_titleTimer_s = 0;
		m_titleRoutine++;
		//アルファ値を１にする
		m_titleAlpha = MOVED_ALPHA_NUM;
	case 2:
		//タイトル文字の回転
		m_titleRotetion = Lerp(RORATION_ZERO, MOVE_RORATION, m_titleTimer_s / MOVE_TIME);
		break;
	case 3:
		//タイマー初期化
		m_titleTimer_s = 0;
		m_titleRoutine++;
	case 4:
		
		//タイトル文字の回転
		m_titleRotetion = Lerp(MOVE_RORATION, RORATION_ZERO, m_titleTimer_s / MOVE_TIME);
		
		break;
	case 5:
		//タイマー初期化
		m_titleTimer_s = 0;
		m_titleRoutine++;
	case 6:

		//タイトル文字の回転
		m_titleRotetion = Lerp(RORATION_ZERO, -MOVE_RORATION, m_titleTimer_s / MOVE_TIME);
		
		break;
	case 7:
		m_titleTimer_s = 0;
		m_titleRoutine++;
	case 8:
		
		//タイトル文字の回転
		m_titleRotetion = Lerp(-MOVE_RORATION, RORATION_ZERO, m_titleTimer_s / MOVE_TIME);
		
		break;
	case 9:
		m_titleTimer_s = 0;
		//回転を繰り返す
		m_titleRoutine = ROTATION_RESET_ROUTINE;
	default:
		break;
	}

	//移動し終わったら次に行く
	if (m_titleTimer_s > MOVE_TIME)
	{
		m_titleTimer_s = MOVE_TIME;
		m_titleRoutine++;
	}


}

/// <summary>
/// ステージ選択の画像を描画
/// </summary>
void TitleScene::DrawStageSelect()
{
	//ステージ画像の数
	int stageSelectTextureNum = 3;

	DirectX::SimpleMath::Vector2 stageBasePosition = DirectX::SimpleMath::Vector2::Zero;

	FileLoadManager& fileLoadManager = FileLoadManager::GetInstance();

	int stageNum = fileLoadManager.LoadFile(L"Resources/StageData/").size();

	//ステージ画像テクスチャのロード
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> stageSelectTexture;

	stageSelectTexture.resize(stageSelectTextureNum);
	//ステージ画像のファイルパス
	std::wstring stageTexFileName = { L"Resources/Textures/stageText.png" };
	std::wstring stageNumFileName = { L"Resources/Textures/stageNum.png" };

	TextureManager& textureManager = TextureManager::GetInstance();

	//ステージ画像テクスチャのロード
	m_stageSelectTexture[0] = textureManager.LoadTexture(stageTexFileName.c_str());
	DirectX::SimpleMath::Vector2 StageTextTextureSize = textureManager.GetTextureSize(stageTexFileName.c_str());

	m_stageSelectTexture[1] = textureManager.LoadTexture(stageNumFileName.c_str());
	DirectX::SimpleMath::Vector2 StageNumTextureSize = textureManager.GetTextureSize(stageNumFileName.c_str());

	float stageNumSize = (StageNumTextureSize.x ) / 10;
	
	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// ウィンドウサイズの取得
	RECT size = pDR->GetOutputSize();

	//ステージ画像のY座標
	float stageSelectPosY = static_cast<float>(size.bottom / 2.0f + size.bottom / 4.0);

	//ステージ画像の拡大率
	float stageTexScale = 1.5f;

	float stagePositionX = size.right ;

	DirectX::SimpleMath::Vector2 stageTextTextureOrigin = StageTextTextureSize / 2.0f;

	//ステージ画像の色
	DirectX::SimpleMath::Vector4 stageSelectColor = DirectX::Colors::White;

	stageBasePosition = DirectX::SimpleMath::Vector2(-(stagePositionX * m_stageNum)+ size.right / 2.0f,0);

	for (int i = 0; i < stageNum; i++)
	{
		//カラー
		DirectX::SimpleMath::Vector4 color = DirectX::Colors::White;

		DirectX::SimpleMath::Vector2 texturePosition = DirectX::SimpleMath::Vector2(stagePositionX * i, stageSelectPosY);
		RECT texRect = 
		{
			stageNumSize* i,
			0,
			stageNumSize* (i+1) - (stageNumSize/5),
			StageNumTextureSize.y
		};

		//現在の選択されているステージは赤色で表示する
		if (i == m_stageNum)
		{
			color = DirectX::Colors::Red;
		}

		//ステージ描画
		m_spriteBatch->Draw(m_stageSelectTexture[0].Get(), m_stageTextureBasePosition + texturePosition , nullptr, color, 0.0f, stageTextTextureOrigin, stageTexScale);

		m_spriteBatch->Draw(m_stageSelectTexture[1].Get(), m_stageTextureBasePosition + texturePosition + DirectX::SimpleMath::Vector2((StageTextTextureSize.x)+ stageNumSize/2, 0), &texRect, color, 0.0f, DirectX::SimpleMath::Vector2(stageNumSize, StageNumTextureSize.y/2), stageTexScale);

	}

}

/// <summary>
/// モード選択の画像を描画
/// </summary>
void TitleScene::DrawModeSelect()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// ウィンドウサイズの取得
	RECT size = pDR->GetOutputSize();

	//windowサイズの4分の１の値
	float windowHeightQuarter = static_cast<float>(size.right / 4.0f);
	//windowサイズの半分の値
	float windowHeightHelf = static_cast<float>(size.right / 2.0f);
	//モード画像のY座標
	float modeSelectPosY = static_cast<float>(size.bottom / 2.0f + size.bottom / 4.0);

	//モードの画像座標
	DirectX::SimpleMath::Vector2 modeSelectPosition[2] =
	{
		//ソロモード
		DirectX::SimpleMath::Vector2{windowHeightHelf - windowHeightQuarter , modeSelectPosY },
		//マルチモード
		DirectX::SimpleMath::Vector2{windowHeightHelf + windowHeightQuarter , modeSelectPosY },

	};

	//モード画像の色
	DirectX::SimpleMath::Vector4 modeSelectColor = DirectX::Colors::White;

	float tex[2] = { 92,102 };

	//モード画像の中心点位置
	DirectX::SimpleMath::Vector2 modeTexCenterPos[] =
	{
		//ソロモード
		DirectX::SimpleMath::Vector2(92.0f,38.0f),
		//マルチモード
		DirectX::SimpleMath::Vector2(102.0f,38.0f),
	};

	for (int i = 0; i < m_modeSelectTextures.size(); i++)
	{
		//カラー
		DirectX::SimpleMath::Vector4 color = modeSelectColor;

		//現在の選択しているモードを赤色で表示する
		if (i == static_cast<int>(m_playerMode) - 1)
		{
			color = DirectX::Colors::Red;
		}

		//モード画像描画
		m_spriteBatch->Draw(m_modeSelectTextures[i].Get(), modeSelectPosition[i], nullptr, color, 0.0f, modeTexCenterPos[i], 2.0f);
	}

}

/// <summary>
/// 終了処理
/// </summary>
void TitleScene::Finalize()
{

	
}
/// <summary>
/// リーソスの読み込み
/// </summary>
void TitleScene::LoadResources()
{
	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3Dデバイスとデバイスコンテキストの取得
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// コモンステート::D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// スプライトバッチ::デバッグ情報の表示に必要
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	TextureManager& textureManager = TextureManager::GetInstance();
	
	m_titileTexture = textureManager.LoadTexture(L"Resources/Textures/title.png");
	m_pushTexture = textureManager.LoadTexture(L"Resources/Textures/PushSpaceKey.png");
	m_CRIWARETexture = textureManager.LoadTexture(L"Resources/Textures/CRIWARELOGO_1.png");



	//ステージマネージャー
	m_stageManager = std::make_unique<StageManager>();
	//初期化
	m_stageManager->Initialize(StageManager::StageSelect::Stage1);

	
	m_modeSelectTextures.resize(static_cast<int>(GameMain::PlayerMode::Player2));

	//プレイヤーモード画像のファイルパス
	std::wstring modeTexFileName[] =
	{
		L"Resources/Textures/SOLO.png",
		L"Resources/Textures/MARUTI.png",
	};

	//プレイヤーモードのテクスチャのロード
	for (int i = 0; i < static_cast<int>(GameMain::PlayerMode::Player2); i++)
	{
		m_modeSelectTextures[i] = textureManager.LoadTexture(modeTexFileName[i].c_str());
		
	}

	//ステージ画像の数
	int stageSelectTextureNum = 3;
	//ステージ画像テクスチャのロード
	m_stageSelectTexture.resize(stageSelectTextureNum);
	//ステージ画像のファイルパス
	std::wstring stageTexFileName[] =
	{
		L"Resources/Textures/stage1.png",
		L"Resources/Textures/stage2.png",
		L"Resources/Textures/stage3.png",
	};

	//ステージ画像テクスチャのロード
	for (int i = 0; i < m_stageSelectTexture.size(); i++)
	{
		m_stageSelectTexture[i] = textureManager.LoadTexture(stageTexFileName[i].c_str());
	}
	

}

/// <summary>
/// ステージセレクトの更新
/// </summary>
void TitleScene::StageSelectUpdate(const DX::StepTimer& timer)
{
	//ボタンを押したか
	bool isKeyPush = false;

	//ステージ選択
	if (m_keyboardStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Left))
	{
		//押したらtrueにする
		isKeyPush = true;

		m_stageNum -= 1;
		m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);

	}
	else if (m_keyboardStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Right))
	{
		//押したらtrueにする
		isKeyPush = true;

		m_stageNum += 1;

		m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);
	}


	FileLoadManager& fileLoadManager = FileLoadManager::GetInstance();

	int stageNum = fileLoadManager.LoadFile(L"Resources/StageData/").size();

	//ステージ選択の値がステージの数より低くったらステージ１にする
	if (m_stageNum < static_cast<int>(StageManager::StageSelect::Stage1))
	{
		m_stageNum = static_cast<int>(StageManager::StageSelect::Stage1);
	}
	//ステージ選択の値がステージの数より大きくなったらステージ３にする
	else if (m_stageNum > stageNum-1)
	{
		m_stageNum = stageNum-1;
	}


	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	// ウィンドウサイズの取得
	RECT size = pDR->GetOutputSize();

	DirectX::SimpleMath::Vector2 pos = DirectX::SimpleMath::Vector2( size.right/2, 0);


	//押したらステージマネージャーのステージを変える
	if (isKeyPush)
	{
		m_stageManager->Initialize( static_cast<StageManager::StageSelect>(m_stageNum));

		m_stageTexturetime = 0.0f;

		m_nextPosition = (pos * -(m_stageNum)) + DirectX::SimpleMath::Vector2( size.right / 2.0f,0);
		m_prePosition = m_stageTextureBasePosition;
	}

	//スペースキーを押すとモードセレクトに移動
	if (m_keyboardStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Space))
	{
		m_titleSelect = TitleScene::TitleState::MODESELECT;
		m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);

	}



	m_stageTextureBasePosition.x = -(size.right * m_stageNum) + size.right / 2.0f;

	static const float MOVE_TIME = 1;

	m_stageTexturetime += timer.GetElapsedSeconds();




	m_stageTextureBasePosition=DirectX::SimpleMath::Vector2::Lerp(m_prePosition, m_nextPosition, m_stageTexturetime / MOVE_TIME);

	if (m_stageTexturetime > MOVE_TIME)
	{
		m_stageTexturetime = MOVE_TIME;
		m_prePosition = m_stageTextureBasePosition;
	}


}

/// <summary>
/// モードセレクトの更新
/// </summary>
void TitleScene::ModeSelectUpdate()
{
	//モード選択
	if (m_keyboardStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Left))
	{
		m_playerMode = GameMain::PlayerMode::Player1;
		m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);

	}
	if (m_keyboardStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Right))
	{
		m_playerMode = GameMain::PlayerMode::Player2;
		m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);

	}

	//スペースキーを押すとフェードアウトしプレイシーンに移行する
	if (m_keyboardStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Space))
	{

		m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);
		m_titleSelect = TitleState::FADEOUT;

	}
}
