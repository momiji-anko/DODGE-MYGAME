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
	m_titleSelect(TitleState::FADEIN),
	m_stageNum(0),
	m_stageManager{},
	m_playerMode(GameMain::PlayerMode::Player1),
	m_pAdx2(),
	m_musicID(0)
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
	

	//フェードの作成
	m_fadeInOut = std::make_unique<Fade>();
	m_fadeInOut->Create();
	m_fadeInOut->Initialize(DirectX::SimpleMath::Vector3::Zero);
	
	//キーボードステートトラッカー取得
	m_keyboardStateTracker = std::make_unique < DirectX::Keyboard::KeyboardStateTracker >();

	GameContext::GetInstance().SetCommonState(m_commonState.get());
	GameContext::GetInstance().SetKeyboardStateTracker(m_keyboardStateTracker.get());
	GameContext::GetInstance().SetSpriteBatcth(m_spriteBatch.get());

	m_stageSelect = std::make_unique<StageSelect>(m_stageManager.get());
	m_stageSelect->Initialize();
	
	m_modeSelect = std::make_unique<ModeSelect>();
	m_modeSelect->Initialize();

	m_titleText = std::make_unique<TitleText>();
	m_titleText->Initialize();
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
	m_titleText->Update(timer);

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

	//タイトルの状態の描画
	TitleStateDraw();

	//タイトル文字描画
	m_titleText->Draw();


	//CRIWAREの座標
	DirectX::SimpleMath::Vector2 CRIWARETexPos{ static_cast<float>(size.right) - 110.0f, static_cast<float>(size.bottom) - 110.0f };
	//CRIWAREの拡大率
	float CRIWAREScale = 0.1f;
	//CRIWAREの表示
	m_spriteBatch->Draw(m_CRIWARETexture.Get(), CRIWARETexPos, nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero, CRIWAREScale);
	
	//PUSH_SPASE_KEYの座標
	DirectX::SimpleMath::Vector2 pushSpaseKeyPos(0, 0);


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
		if(m_stageSelect->Update(timer))
		{
			m_titleSelect = TitleScene::TitleState::MODESELECT;
			m_stageNum = m_stageSelect->GetSelectStageNum();
		}


		break;
	case TitleScene::TitleState::MODESELECT:

		//モードセレクトの更新
		if (m_modeSelect->Update(timer))
		{
			m_titleSelect = TitleScene::TitleState::FADEOUT;
			m_playerMode = m_modeSelect->GetPlayerMode();
		}

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
/// タイトルの状態の描画
/// </summary>
void TitleScene::TitleStateDraw()
{

	//PUSH_SPASE_KEYの座標
	DirectX::SimpleMath::Vector2 pushSpaseKeyPos(0, 0);

	//PUSH_SPASE_KEYのカラー
	DirectX::SimpleMath::Vector4 pushColor{ 1.0f,1.0f,1.0f,m_alpha };

	switch (m_titleSelect)
	{
	case TitleScene::TitleState::TITLE:

		//PUSH_SPASE_KEYの描画
		m_spriteBatch->Draw(m_pushTexture.Get(), pushSpaseKeyPos, nullptr, pushColor, 0.0f, DirectX::SimpleMath::Vector2::Zero);

		break;
	case TitleScene::TitleState::STAGESELECT:

		// ステージ選択の画像を描画
		m_stageSelect->Draw();

		break;

	case TitleScene::TitleState::MODESELECT:

		// モード選択の画像を描画
		m_modeSelect->Draw();

		break;
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

	//PUSH_SPACE_KEYの画像の読み込み
	m_pushTexture = textureManager.LoadTexture(L"Resources/Textures/PushSpaceKey.png");
	//CRIWARE画像の読み込み
	m_CRIWARETexture = textureManager.LoadTexture(L"Resources/Textures/CRIWARELOGO_1.png");


	//ステージマネージャー
	m_stageManager = std::make_unique<StageManager>();
	//初期化
	m_stageManager->Initialize(StageManager::StageSelect::Stage1);

}

