/*
タイトルシーン
*/

#include "pch.h"

#include <WICTextureLoader.h>
#include <SimpleMath.h>

#include "../GameMain.h"
#include "DeviceResources.h"

#include "ResultScene.h"
#include<string>
#include"Game/PlayScene/AliveTimer.h"
using namespace DirectX;

/*--------------------------------------------------
コンストラクタ
--------------------------------------------------*/
ResultScene::ResultScene():
	m_fade(1.0f),
	m_flag(false),
	m_flagFadeIn(false),
	m_alpha(1.0f),
	m_alphaVel(0.01f),
	m_rank(RANK::NONE),
	m_resultTime_s(0.0f),
	m_rankPos{},
	m_cameraRot(0.0f),
	m_scene(ResultState::TIME)
{
}

/*--------------------------------------------------
デストラクタ
--------------------------------------------------*/
ResultScene::~ResultScene()
{
	m_pAdx2->Finalize();
}

/*--------------------------------------------------
初期化
--------------------------------------------------*/
void ResultScene::Initialize()
{
	m_pAdx2 = &ADX2::GetInstance();

	m_pAdx2->Initialize("Resources/Sounds/DODGESound.acf",
		"Resources/Sounds/CueSheet_0.acb");

	m_musicID = m_pAdx2->Play(CRI_CUESHEET_0_RESULT);
	AliveTimer* aliveTime = &AliveTimer::GetInstance();
	m_time = aliveTime->GetTimer();
	if (m_time <= 35)
	{

		m_rank = RANK::D;
	}
	else if (m_time <= 60)
	{

		m_rank = RANK::C;
	}
	else if (m_time <= 90)
	{

		m_rank = RANK::B;
	}
	else if (m_time <= 150)
	{

		m_rank = RANK::A;
	}
	else 
	{

		m_rank = RANK::S;
	}

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3Dデバイスとデバイスコンテキストの取得
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	m_stageManeger = std::make_unique<StageManeger>();
	m_stageManeger->Initialize(m_commonState.get(), StageManeger::StageSelect::Stage1);

}

/*--------------------------------------------------
更新
戻り値	:次のシーン番号
--------------------------------------------------*/
GAME_SCENE ResultScene::Update(const DX::StepTimer& timer)
{
	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	// マウス入力情報を取得する
	DirectX::Mouse::State mouseState = DirectX::Mouse::Get().GetState();

	if (!m_flagFadeIn)
	{
		m_fade -= 0.03f;
	}

	if (m_fade <= 0)
	{
		m_flagFadeIn = true;
	}
	m_alpha -= m_alphaVel;
	m_cameraRot += 0.001f;

	if (m_alpha < 0 || m_alpha > 1)
		m_alphaVel *= -1.0f;

	if (m_flag)
	{
		m_fade += 0.03f;
	}
	switch (m_scene)
	{
	case ResultScene::ResultState::TIME:

		break;
	case ResultScene::ResultState::RANK:
		break;
	case ResultScene::ResultState::SCENET:
		break;
	default:
		break;
	}
	if (keyState.Space && m_flagFadeIn == true)
	{
		m_flag = true;
	}

	if (m_fade > 1)
	{
		return GAME_SCENE::TITLE;
	}

	return GAME_SCENE::NONE;
}

/*--------------------------------------------------
描画
--------------------------------------------------*/
void ResultScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	RECT rect = pDR->GetOutputSize();
	// D3Dデバイスとデバイスコンテキストの取得
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	ウィンドウサイズの取得
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	// ビュー行列
	DirectX::SimpleMath::Matrix view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix projection;
	// カメラ座標
	DirectX::SimpleMath::Vector3 eye = { cos(m_cameraRot) * 18,10,-sin(m_cameraRot) * 18 };
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
	RenderPlayStage(&camera);


	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	SimpleMath::Vector2 pos(0.0f, 0.0f);

	//m_spriteBatch->Draw(m_numTexture.Get(), pos, nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero);
	
	
	//m_spriteBatch->Draw(m_rankBackTexture.Get(), pos, nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero);

	DirectX::SimpleMath::Vector2 resultPos{ width / 2 , 100};
	m_spriteBatch->Draw(m_resultTexture.Get(), resultPos, nullptr, Colors::White, 0.0f, { 129.0f,44.0f }, 2.0f, SpriteEffects_None, 0);


	SimpleMath::Vector2 onePos{ width / 2 +150 , height / 2 };
	SimpleMath::Vector2 tenPos{ onePos.x - 85, onePos.y };
	SimpleMath::Vector2 hundredPos{ onePos.x - 170, onePos.y };

	RECT rectNum[10] = {
	{0	,0,52	,64},//0
	{52	,0,97	,64},//1
	{96	,0,140	,64},//2
	{140,0,190,64},//3
	{190,0,243,64},//4
	{243,0,293,64},//5
	{293,0,341,64},//6
	{341,0,388,64},//7
	{388,0,436,64},//8
	{436,0,483,64} //9
	};
	
	DirectX::SimpleMath::Vector2 timePos{ width / 3 - 50,height / 2 +20};

	m_spriteBatch->Draw(m_aliveTimeTexture.Get(), timePos + DirectX::SimpleMath::Vector2{ 83.0f, 38.0f }, nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2{ 83.0f,38.0f }, 2.0f, SpriteEffects_None, 0);
	m_spriteBatch->Draw(m_numTexture.Get(), onePos, &rectNum[static_cast<int>(m_time)%10], Colors::White, 0.0f, { 0.0f,0.0f }, 2.0f, SpriteEffects_None, 0);
	m_spriteBatch->Draw(m_numTexture.Get(), tenPos, &rectNum[static_cast<int>(m_time / 10)%10], Colors::White, 0.0f, { 0.0f,0.0f }, 2.0f, SpriteEffects_None, 0);
	m_spriteBatch->Draw(m_numTexture.Get(), hundredPos, &rectNum[static_cast<int>(m_time / 100)%10], Colors::White, 0.0f, { 0.0f,0.0f }, 2.0f, SpriteEffects_None, 0);
	

	int rank;

	RECT rectRank[] = {
	{450,0,600  ,150},//D	3
	{300,0,450	,150},//C	2
	{150,0,300	,150},//B	1
	{0	,0,150	,150},//A	0
	{600,0,750  ,150},//s	4
	};

	if (m_time <= 15)
	{
		
		rank = 0;
	}
	else if (m_time <= 30)
	{
		
		rank = 1;
	}
	else if (m_time <= 60)
	{
		
		rank = 2;
	}
	else if (m_time <= 120)
	{
		
		rank = 3;
	}
	else if (m_time >= 120)
	{
		
		rank = 4;
	}


	pos = { 0, 0 };
	DirectX::SimpleMath::Vector2 rankStrPos{ width / 3 -30, height / 4  + 37.0f};
	m_spriteBatch->Draw(m_rankStringTexture.Get(), rankStrPos + DirectX::SimpleMath::Vector2(97.0f, 37.0f), nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(97.0f, 37.0f), 2.0f); 
	m_spriteBatch->Draw(m_rankTexture.Get(), DirectX::SimpleMath::Vector2(width/2+60,height/4), &rectRank[static_cast<int>(m_rank)], DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero);

	DirectX::SimpleMath::Vector4 pushColor{ 1.0f,1.0f,1.0f,m_alpha };

	m_spriteBatch->Draw(m_pushTexture.Get(), pos, nullptr, pushColor, 0.0f, DirectX::SimpleMath::Vector2::Zero);

	

	DirectX::SimpleMath::Vector4 fadeColor{ 1.0f,1.0f,1.0f,m_fade };
	m_spriteBatch->Draw(m_blackTexture.Get(), pos, nullptr, fadeColor, 0.0f, DirectX::SimpleMath::Vector2::Zero);

	m_spriteBatch->End();
}

/*--------------------------------------------------
終了処理
--------------------------------------------------*/
void ResultScene::Finalize()
{
}
/*--------------------------------------------------
リソースの読み込み
--------------------------------------------------*/
void ResultScene::LoadResources()
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
		L"Resources/Textures/num.png",
		nullptr,
		m_numTexture.ReleaseAndGetAddressOf()
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
		L"Resources/Textures/PushSpaceKey.png",
		nullptr,
		m_pushTexture.ReleaseAndGetAddressOf()
	);

	// テクスチャの読み込み
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/rank.png",
		nullptr,
		m_rankTexture.ReleaseAndGetAddressOf()
	);

	// テクスチャの読み込み
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/time.png",
		nullptr,
		m_aliveTimeTexture.ReleaseAndGetAddressOf()
	);

	// テクスチャの読み込み
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/A.png",
		nullptr,
		m_rankBackTexture.ReleaseAndGetAddressOf()
	);

	// テクスチャの読み込み
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/result.png",
		nullptr,
		m_resultTexture.ReleaseAndGetAddressOf()
	);
	
	// テクスチャの読み込み
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/rankString.png",
		nullptr,
		m_rankStringTexture.ReleaseAndGetAddressOf()
	);


}

void ResultScene::RenderPlayStage(Camera* camera)
{
	m_stageManeger->Draw(camera);
	/*m_itemManeger->Draw(camera);
	m_player->Draw(camera);
	m_obstacleManeger->Draw(camera);*/
}
