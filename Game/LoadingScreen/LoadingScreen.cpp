/*
ロード画面
*/

#include "pch.h"

#include "LoadingScreen.h"

#include "DeviceResources.h"
#include "../IScene.h"

#include <WICTextureLoader.h>

using namespace DirectX;


/*--------------------------------------------------
コンストラクタ
--------------------------------------------------*/
LoadingScreen::LoadingScreen()
	:
	m_texPosition{ 0.0f,0.0f }
{
}

/*--------------------------------------------------
デストラクタ
--------------------------------------------------*/
LoadingScreen::~LoadingScreen()
{
	if (m_loadingThread.joinable())
	{
		m_loadingThread.join();
	}
}

/*--------------------------------------------------
初期化
--------------------------------------------------*/
void LoadingScreen::Initialize(IScene* targetScene)
{
	// 対象シーンのリソースの読み込み
	LoadSceneResources(targetScene);


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
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/nowLoading.png",
		nullptr,
		m_texture.ReleaseAndGetAddressOf()
	);

	// テクスチャの読み込み
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/black.png",
		nullptr,
		m_blackTexture.ReleaseAndGetAddressOf()
	);

	RECT rect = pDR->GetOutputSize();

	m_texPosition = DirectX::SimpleMath::Vector2(rect.right, rect.bottom - 120);

	m_texturePositions.resize(10);

	for (int i=0 ; i<m_texturePositions.size();i++)
	{
		m_texturePositions[i] = DirectX::SimpleMath::Vector2(m_texPosition.x - (i * 690.0f), m_texPosition.y);
	}
}

/*--------------------------------------------------
更新
戻り値	:次のシーン番号
--------------------------------------------------*/
void LoadingScreen::Update(const DX::StepTimer& timer)
{


	for (DirectX::SimpleMath::Vector2& texPos : m_texturePositions)
	{
		texPos.x -= 5.0f;

		if (texPos.x < -780.0f)
		{
			texPos.x = m_texPosition.x;
		}
	}
}

/*--------------------------------------------------
描画
--------------------------------------------------*/
void LoadingScreen::Draw()
{
	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	m_spriteFont->DrawString(m_spriteBatch.get(), L"Load Screen", DirectX::XMFLOAT2(10, 10));
	SimpleMath::Vector2 blackTexture(0, 0);
	m_spriteBatch->Draw(m_blackTexture.Get(), blackTexture, nullptr, DirectX::SimpleMath::Vector4::One, 0.0f, DirectX::SimpleMath::Vector2::Zero);
	SimpleMath::Vector2 pos(640 - 128, 360 - 128);
	//m_spriteBatch->Draw(m_texture.Get(), m_texPosition);

	for (DirectX::SimpleMath::Vector2& texPos : m_texturePositions)
	{
		m_spriteBatch->Draw(m_texture.Get(), texPos);
	}

	m_spriteBatch->End();
}

/*--------------------------------------------------
終了処理
--------------------------------------------------*/
void LoadingScreen::Finalize()
{
}

/*--------------------------------------------------
	// 読み込み中かどうかの取得
--------------------------------------------------*/
bool LoadingScreen::IsLoading()
{
 return m_isLoading;
}


/*--------------------------------------------------
シーンのリソースの読み込み
--------------------------------------------------*/
void LoadingScreen::LoadSceneResources(IScene* targetScene)
{
	if (m_loadingThread.joinable())
	{
		m_loadingThread.join();
	}
	m_isLoading = true;
	m_loadingThread = std::thread(
		[=]
		{
			targetScene->LoadResources();
			m_isLoading = false;
		});
}
