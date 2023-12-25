/*
ロード画面
*/

#include "pch.h"

#include "LoadingScreen.h"

#include "DeviceResources.h"
#include "../IScene.h"

#include <WICTextureLoader.h>
#include"Libraries/MyLibraries/TextureManager.h"
#include<WICTextureLoader.h>

//loading画像の移動速度
const float LoadingScreen::TEXTURE_MOVE_SPEED = 5.f;
//loading画像のリセット座標
const float LoadingScreen::TEXTURE_POSITION_RESET = -780.0f;

/*--------------------------------------------------
コンストラクタ
--------------------------------------------------*/
LoadingScreen::LoadingScreen()
	:
	m_textureStartPosition{ 0.0f,0.0f }
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

	//nowLoadingの読み込み
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/nowLoading.png",
		nullptr,
		m_texture.ReleaseAndGetAddressOf()
	);

	// 背景の読み込み
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/black.png",
		nullptr,
		m_blackTexture.ReleaseAndGetAddressOf()
	);

	//ウィンドウサイズ取得
	RECT rect = pDR->GetOutputSize();

	//Loading画像のサイズ
	DirectX::SimpleMath::Vector2 loadingTextureSize = { 690.f,120.f};

	//Loading画像の初期位置
	m_textureStartPosition = DirectX::SimpleMath::Vector2(static_cast<float>(rect.right), static_cast<float>(rect.bottom) - loadingTextureSize.y);

	//Loading画像の数
	int loadingTextureNum = 10;

	//Loading画像の座標をリサイズ
	m_texturePositions.resize(loadingTextureNum);

	//Loading画像の座標を計算
	for (int i=0 ; i<m_texturePositions.size();i++)
	{
		m_texturePositions[i] = DirectX::SimpleMath::Vector2(m_textureStartPosition.x - (i * loadingTextureSize.x), m_textureStartPosition.y);
	}
}

/*--------------------------------------------------
更新
--------------------------------------------------*/
void LoadingScreen::Update(const DX::StepTimer& timer)
{
	//警告避け
	UNREFERENCED_PARAMETER(timer);

	//画像の移動させる
	for (DirectX::SimpleMath::Vector2& texPos : m_texturePositions)
	{
		//移動
		texPos.x -= TEXTURE_MOVE_SPEED;

		//画像がリセット座標より小さくなったらスタート座標に戻す
		if (texPos.x < TEXTURE_POSITION_RESET)
		{
			texPos.x = m_textureStartPosition.x;
		}
	}
}

/*--------------------------------------------------
描画
--------------------------------------------------*/
void LoadingScreen::Draw()
{
	//画像描画開始
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	//背景の座標
	DirectX::SimpleMath::Vector2 blackTexture(0, 0);
	//背景の描画
	m_spriteBatch->Draw(m_blackTexture.Get(), blackTexture, nullptr, DirectX::SimpleMath::Vector4::One, 0.0f, DirectX::SimpleMath::Vector2::Zero);
	
	//loadingの描画
	for (DirectX::SimpleMath::Vector2& texPos : m_texturePositions)
	{
		m_spriteBatch->Draw(m_texture.Get(), texPos);
	}

	//描画終了
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
	//スレッド可能であればスレッドを起動する
	if (m_loadingThread.joinable())
	{
		m_loadingThread.join();
	}
	//ロードしている状態
	m_isLoading = true;
	//スレッドを使用して次のシーンのリソースを読み込む
	m_loadingThread = std::thread(
		[=]
		{
			targetScene->LoadResources();
			m_isLoading = false;
		});
}
