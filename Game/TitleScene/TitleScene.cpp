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
	m_titleSelect(TitleState::FADEIN),
	m_modeSelectNum(0),
	m_stageNum(0), 
	m_stageManager{}
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


	m_fadeInOut = std::make_unique<Fade>();
	m_fadeInOut->Create();
	m_fadeInOut->Initialize(DirectX::SimpleMath::Vector3::Zero,1.0f);
	m_fadeInOut->FadeIn();

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
	
	m_fadeInOut->Update(timer);
	
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
			m_titleSelect = TitleScene::TitleState::STAGESELECT;
			m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);
			
		}
		break;
	case TitleScene::TitleState::STAGESELECT:

		if (m_keyboardStateTracker->IsKeyPressed(Keyboard::Keys::Left))
		{
			m_stageNum -= 1;
			m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);

		}
		if (m_keyboardStateTracker->IsKeyPressed(Keyboard::Keys::Right))
		{
			m_stageNum += 1;
			m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);


		}

		if (m_stageNum < 0)
		{
			m_stageNum = 0;
		}

		if (m_stageNum > 2)
		{
			m_stageNum = 2;
		}

		if (m_keyboardStateTracker->IsKeyPressed(Keyboard::Keys::Space) && m_flagFadeIn == true)
		{
			m_titleSelect = TitleScene::TitleState::MODESELECT;
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

			m_playerMode = static_cast<GameMain::PlayerMode>(m_modeSelectNum + 1);
		}

		break;
	case TitleScene::TitleState::FADEOUT:
		m_fadeInOut->FadeOut();
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

	if (m_fadeInOut->ISClose() && TitleScene::TitleState::FADEOUT == m_titleSelect)
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

	// デバイスコンテキストの取得
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

	m_stageManager->Draw(&camera);

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
	{
		m_spriteBatch->Draw(m_pushTexture.Get(), pos, nullptr, pushColor, 0.0f, DirectX::SimpleMath::Vector2::Zero);

	}
		break;
	case TitleScene::TitleState::STAGESELECT:
	{
		float aa = size.right / 3.0f;
		
		DirectX::SimpleMath::Vector2 stageSelectPosition[] =
		{
			DirectX::SimpleMath::Vector2{size.right / 2.0f - aa , size.bottom / 2.0f + size.bottom / 4.0f  },
			DirectX::SimpleMath::Vector2{size.right / 2.0f  , size.bottom / 2.0f + size.bottom / 4.0f },
			DirectX::SimpleMath::Vector2{size.right / 2.0f + aa , size.bottom / 2.0f + size.bottom / 4.0f }
		};

		DirectX::SimpleMath::Vector4 stageSelectColor[] =
		{
			DirectX::SimpleMath::Vector4{1.0f,1.0f,1.0f,1.0f},
			DirectX::SimpleMath::Vector4{1.0f,0.0f,0.0f,1.0f},
		};
		float texSize[] = { 117.5f,123,102 };
		for (int i = 0; i < m_stageSelectTexture.size(); i++)
		{
			DirectX::SimpleMath::Vector4 color = stageSelectColor[0];

			if (i == m_stageNum)
			{
				color = stageSelectColor[1];
			}

			m_spriteBatch->Draw(m_stageSelectTexture[i].Get(), stageSelectPosition[i], nullptr, color, 0.0f, DirectX::SimpleMath::Vector2(texSize[i], 39.0f), 1.5f);

		}
	}
		break;
	case TitleScene::TitleState::MODESELECT:
	{
		

		DirectX::SimpleMath::Vector2 modeSelectPosition[3] =
		{
			DirectX::SimpleMath::Vector2{size.right / 2.0f - size.right / 4.0f , size.bottom / 2.0f + size.bottom / 4.0f },
			DirectX::SimpleMath::Vector2{size.right / 2.0f + size.right / 4.0f , size.bottom / 2.0f + size.bottom / 4.0f },
			
		};

		DirectX::SimpleMath::Vector4 modeSelectColor[3] =
		{
			DirectX::SimpleMath::Vector4{1.0f,static_cast<float>(m_modeSelectNum),static_cast<float>(m_modeSelectNum),1.0f},
			DirectX::SimpleMath::Vector4{1.0f,1.0f - m_modeSelectNum,1.0f - m_modeSelectNum,1.0f},
		};
		float tex[3] = {92,102};

		for (int i = 0; i < m_modeSelectTextures.size(); i++)
		{

			m_spriteBatch->Draw(m_modeSelectTextures[i].Get(), modeSelectPosition[i], nullptr, modeSelectColor[i], 0.0f, DirectX::SimpleMath::Vector2(tex[i], 38.0f),2.0f);

		}
	}
		break;


	}
	


	m_spriteBatch->End();

	m_fadeInOut->Render();

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

	TextureManager& textureManager = TextureManager::GetInstance();
	
	m_titileTexture = textureManager.LoadTexture(L"Resources/Textures/title.png");
	m_pushTexture = textureManager.LoadTexture(L"Resources/Textures/PushSpaceKey.png");
	m_blackTexture = textureManager.LoadTexture(L"Resources/Textures/black.png");
	m_CRIWARETexture = textureManager.LoadTexture(L"Resources/Textures/CRIWARELOGO_1.png");




	m_stageManager = std::make_unique<StageManager>();
	m_stageManager->Initialize(m_commonState.get(), StageManager::StageSelect::Stage1);

	//プレイヤーモードのテクスチャのロード
	m_modeSelectTextures.resize(static_cast<int>(GameMain::PlayerMode::Player2));

	std::wstring modeTexFileName[static_cast<int>(GameMain::PlayerMode::Player2)] =
	{
		L"Resources/Textures/SOLO.png",
		L"Resources/Textures/MARUTI.png",
	};


	for (int i = 0; i < static_cast<int>(GameMain::PlayerMode::Player2); i++)
	{
		m_modeSelectTextures[i] = textureManager.LoadTexture(modeTexFileName[i].c_str());
		
	}

	//プレイヤーモードのテクスチャのロード
	m_stageSelectTexture.resize(3);
	int stageTexNum = static_cast<int>(m_stageSelectTexture.size());
	std::wstring stageTexFileName[] =
	{
		L"Resources/Textures/stage1.png",
		L"Resources/Textures/stage2.png",
		L"Resources/Textures/stage3.png",
	};


	for (int i = 0; i < m_stageSelectTexture.size(); i++)
	{
		m_stageSelectTexture[i] = textureManager.LoadTexture(stageTexFileName[i].c_str());
		
	}

}
