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

using namespace DirectX;

/*--------------------------------------------------
コンストラクタ
--------------------------------------------------*/
TitleScene::TitleScene()
	:
	m_alpha(1.0f),
	m_alphaVel(0.01f),
	m_fade(1.0f),
	m_flag(false),
	m_flagFadeIn(false),
	m_cameraRot(0.0f),
	m_titleTimer_s(0.0f),
	m_titleRoutine(0),
	m_titlePosition{},
	m_titleRotetion(0.0f),
	m_stageSelect(StageSelect::Stage1),
	m_titleSelect(TitleState::FADEIN),
	m_modeSelectNum(0)
{
}

/*--------------------------------------------------
デストラクタ
--------------------------------------------------*/
TitleScene::~TitleScene()
{
	m_pAdx2->Finalize();
}

/*--------------------------------------------------
初期化
--------------------------------------------------*/
void TitleScene::Initialize()
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	m_pAdx2 = &ADX2::GetInstance();

	m_pAdx2->Initialize("Resources/Sounds/DODGESound.acf",
		"Resources/Sounds/CueSheet_0.acb");

	m_musicID = m_pAdx2->Play(CRI_CUESHEET_0_TITLE);

	// ウィンドウサイズの取得
	RECT size = pDR->GetOutputSize();
	m_size = size;
	m_titlePosition.resize(2);
	m_titlePosition[0] = DirectX::SimpleMath::Vector2(-size.right,0);
	m_titlePosition[1] = DirectX::SimpleMath::Vector2(size.right, 0);
	m_titleAlpha = 0.7f;


	m_keyboardStateTracker = std::make_unique < DirectX::Keyboard::KeyboardStateTracker >();
	
}

/*--------------------------------------------------
更新
戻り値	:次のシーン番号
--------------------------------------------------*/
GAME_SCENE TitleScene::Update(const DX::StepTimer& timer)
{
	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	//キーボードステートトラッカーの更新
	m_keyboardStateTracker->Update(keyState);
	
	// マウス入力情報を取得する
	DirectX::Mouse::State mouseState = DirectX::Mouse::Get().GetState();

	static const float MOVE_TIME = 3.0f;

	float time = timer.GetElapsedSeconds();

	switch (m_titleSelect)
	{
	case TitleScene::TitleState::FADEIN:
		if (!m_flagFadeIn)
		{
			m_fade -= 0.03f;
		}

		if (m_fade <= 0)
		{
			m_flagFadeIn = true;
			m_titleSelect = TitleState::TITLE;
		}
		
		break;
	case TitleScene::TitleState::TITLE:
		if (m_keyboardStateTracker->IsKeyPressed(Keyboard::Keys::Space) && m_flagFadeIn == true)
		{
			m_titleSelect = TitleScene::TitleState::MODESELECT;
			m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);
			
		}
		break;
	case TitleScene::TitleState::STAGESELECT:

		if (keyState.IsKeyDown(Keyboard::Keys::Left))
		{
			//m_flag = true;
			m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);

		}

		break;
	case TitleScene::TitleState::MODESELECT:


		if (m_keyboardStateTracker->IsKeyPressed(Keyboard::Keys::Left))
		{
			m_modeSelectNum = 0;
			m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);
			m_playerMode = GameMain::PlayerMode::Player1;

		}
		if (m_keyboardStateTracker->IsKeyPressed(Keyboard::Keys::Right))
		{
			m_modeSelectNum = 1;
			m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);
			

		}

		if (m_keyboardStateTracker->IsKeyPressed(Keyboard::Keys::Space) && m_flagFadeIn == true)
		{
			m_flag = true;
			m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);
			m_titleSelect = TitleState::FADEOUT;
			m_playerMode = GameMain::PlayerMode::Player1;
			m_playerMode = static_cast<GameMain::PlayerMode>(m_modeSelectNum + 1);
		}

		break;
	case TitleScene::TitleState::FADEOUT:
		break;
	default:
		break;
	}



	
	m_cameraRot += 0.001f;
	m_titleTimer_s += time;
	switch (m_titleRoutine)
	{
	case 0:
		m_titlePosition[0].x= Lerp(-m_size.right, 0, easeOutCubic(m_titleTimer_s / MOVE_TIME));
		m_titlePosition[1].x= Lerp(m_size.right, 0, easeOutCubic(m_titleTimer_s / MOVE_TIME));
		break;
	case 1:
		m_titleTimer_s = 0;
		m_titleRoutine++;
		m_titleAlpha = 1.0f;
	case 2:
		m_titleRotetion = Lerp(0.0f, 0.1f, m_titleTimer_s / MOVE_TIME);
		break;
	case 3:
		m_titleTimer_s = 0;
		m_titleRoutine++;
	case 4:
		m_titleRotetion = Lerp(0.1f, 0.0f, m_titleTimer_s / MOVE_TIME);
		break;
	case 5:
		m_titleTimer_s = 0;
		m_titleRoutine++;
	case 6:
		m_titleRotetion = Lerp(0.0f, -0.1f, m_titleTimer_s / MOVE_TIME);
		break;
	case 7:
		m_titleTimer_s = 0;
		m_titleRoutine++;
	case 8:
		m_titleRotetion = Lerp(-0.1f, 0.0f, m_titleTimer_s / MOVE_TIME);
		break;
	case 9:
		m_titleTimer_s = 0;
		m_titleRoutine = 2;
	default:
		break;
	}

	if (m_titleTimer_s > MOVE_TIME)
	{
		m_titleTimer_s = MOVE_TIME;
		m_titleRoutine++;
	}

	if (m_flag)
		m_fade += 0.03f;

	m_alpha -= m_alphaVel;

	if (m_alpha < 0 || m_alpha > 1)
		m_alphaVel *= -1.0f;

	if (m_fade > 1)
	{
		m_pAdx2->Stop(m_musicID);
		return GAME_SCENE::PLAY;
	}
	return GAME_SCENE::NONE;
}

/*--------------------------------------------------
描画
--------------------------------------------------*/
void TitleScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3Dデバイスとデバイスコンテキストの取得
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	


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

	view = DirectX::SimpleMath::Matrix::CreateLookAt(eye, target, up);
	// ウィンドウサイズの取得
	RECT size = pDR->GetOutputSize();
	float aspectRatio = float(size.right) / float(size.bottom);

	//カメラの見える範囲の設定
	float nearPlane = 1.0f;
	float farPlane = 100.0f;
	float fov = 45.0f;

	float fieldOfView = DirectX::XMConvertToRadians(fov);

	projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
	Camera camera;
	camera.SetProjectionMatrix(projection);
	camera.SetViewMatrix(view);

	for (std::unique_ptr<Stage>& stage : m_stageobj)
	{
		stage->Draw(&camera);
	}


	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	SimpleMath::Vector2 pos(0, 0);

	DirectX::SimpleMath::Vector4 pushColor{ 1.0f,1.0f,1.0f,m_alpha };
	DirectX::SimpleMath::Vector4 fadeColor{ 1.0f,1.0f,1.0f,m_fade };

	for (DirectX::SimpleMath::Vector2& position : m_titlePosition)
	{
		m_spriteBatch->Draw(m_titileTexture.Get(), position + DirectX::SimpleMath::Vector2(640, 220), nullptr, DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_titleAlpha), m_titleRotetion, DirectX::SimpleMath::Vector2(640, 220));
	}

	m_spriteBatch->Draw(m_CRIWARETexture.Get(), DirectX::SimpleMath::Vector2(size.right-110, size.bottom-110), nullptr, Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero,0.1f);
	

	

	switch (m_titleSelect)
	{
	case TitleScene::TitleState::TITLE:
		
		m_spriteBatch->Draw(m_pushTexture.Get(), pos, nullptr, pushColor, 0.0f, DirectX::SimpleMath::Vector2::Zero);
		
		break;
	case TitleScene::TitleState::MODESELECT:
		
		DirectX::SimpleMath::Vector2 modeSelectPosition[2] =
		{
			DirectX::SimpleMath::Vector2{size.right / 2.0f - size.right / 4.0f , size.bottom / 2.0f + size.bottom / 4.0f  },
			DirectX::SimpleMath::Vector2{size.right / 2.0f + size.right / 4.0f , size.bottom / 2.0f + size.bottom / 4.0f }
		};

		DirectX::SimpleMath::Vector4 modeSelectColor[2] =
		{
			DirectX::SimpleMath::Vector4{1.0f,static_cast<float>(m_modeSelectNum),static_cast<float>(m_modeSelectNum),1.0f},
			DirectX::SimpleMath::Vector4{1.0f,1.0f - m_modeSelectNum,1.0f - m_modeSelectNum,1.0f},
		};
		float tex[2] = {92,102};

		for (int i = 0; i < m_modeSelectTextures.size(); i++)
		{

			m_spriteBatch->Draw(m_modeSelectTextures[i].Get(), modeSelectPosition[i], nullptr, modeSelectColor[i], 0.0f, DirectX::SimpleMath::Vector2(tex[i], 38.0f),2.0f);

		}

		break;


	}
	
	m_spriteBatch->Draw(m_blackTexture.Get(), pos, nullptr, fadeColor, 0.0f, DirectX::SimpleMath::Vector2::Zero);


	m_spriteBatch->End();


}

/*--------------------------------------------------
終了処理
--------------------------------------------------*/
void TitleScene::Finalize()
{

	
}
/*--------------------------------------------------
リソースの読み込み
--------------------------------------------------*/
void TitleScene::LoadResources()
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3Dデバイスとデバイスコンテキストの取得
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// コモンステート::D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// スプライトバッチ::デバッグ情報の表示に必要
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	// テクスチャの読み込み
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/title.png",
		nullptr,
		m_titileTexture.ReleaseAndGetAddressOf()
	);
	// テクスチャの読み込み
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/PushSpaceKey.png",
		nullptr,
		m_pushTexture.ReleaseAndGetAddressOf()
	);

	// テクスチャの読み込み
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/black.png",
		nullptr,
		m_blackTexture.ReleaseAndGetAddressOf()
	);
	// テクスチャの読み込み
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/CRIWARELOGO_1.png",
		nullptr,
		m_CRIWARETexture.ReleaseAndGetAddressOf()
	);

	//	エフェクトファクトリの作成
	EffectFactory* factory2 = new EffectFactory(pDR->GetD3DDevice());

	//	テクスチャの読み込みパス指定
	factory2->SetDirectory(L"Resources/Models");

	//	ファイルを指定してモデルデータ読み込み
	m_stageModel = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/StageObject.cmo",
		*factory2
	);

	delete factory2;

	m_stagePosition.resize(4);
	for (int i = 0; i < 4; i++)
	{
		if (i < 2)
		{
			m_stagePosition[i] = { 6.0f - i * 12.0f,-1.0f,6.0f };
		}
		else
		{
			m_stagePosition[i] = { 6.0f - (i - 2) * 12.0f ,-1.0f ,-6.0f };
		}

	}

	int num = 0;
	m_stageobj.resize(4);
	for (std::unique_ptr<Stage>& stage : m_stageobj)
	{
		stage = std::make_unique<Stage>();
		stage->Initialize(DirectX::SimpleMath::Vector3::Zero, m_stagePosition[num], true, 0, nullptr, m_stageModel.get(), m_commonState.get());
		stage->SetStageType(static_cast <Stage::StageType>(num));
		stage->Setshader(nullptr);
		num++;
	}

	//プレイヤーモードのテクスチャのロード
	m_modeSelectTextures.resize(static_cast<int>(GameMain::PlayerMode::Player2));

	const wchar_t* modeTexFileName[static_cast<int>(GameMain::PlayerMode::Player2)] =
	{
		L"Resources/Textures/SOLO.png",
		L"Resources/Textures/MARUTI.png",
	};


	for (int i = 0; i < static_cast<int>(GameMain::PlayerMode::Player2); i++)
	{
		CreateWICTextureFromFile(
			device,
			modeTexFileName[i],
			nullptr,
			m_modeSelectTextures[i].ReleaseAndGetAddressOf()
		);
	}

}
