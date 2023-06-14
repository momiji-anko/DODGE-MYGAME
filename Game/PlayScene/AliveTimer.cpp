/*
* 2023/03/23
* Blink.cpp
* 麻生　楓
*/
#include"pch.h"
#include"AliveTimer.h"
#include"DeviceResources.h"
#include<WICTextureLoader.h>
#include"Libraries/MyLibraries/TextureManager.h"
#include"Game/PlayScene/GameContext/GameContext.h"

/// <summary>
/// コンストラクタ
/// </summary>
AliveTimer::AliveTimer()
	:
	m_timer_s(0.0f),
	m_numTexture(nullptr),
	m_spriteBatch(nullptr)
{
}

/// <summary>
/// デストラクタ
/// </summary>
AliveTimer::~AliveTimer()
{
}

/// <summary>
/// 初期化
/// </summary>
void AliveTimer::Initialize()
{
	//ゲームコンテキストからスプライトバッチ取得
	m_spriteBatch = GameContext::GetInstance().GetSpriteBatcth();
	//テクスチャ読み込み
	 m_numTexture = TextureManager::GetInstance().LoadTexture(L"Resources/Textures/num.png");

	m_timer_s = 0.0f;
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer"></param>
void AliveTimer::Update(const DX::StepTimer& timer)
{
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	m_timer_s += elapsedTime;
}

/// <summary>
/// 描画
/// </summary>
void AliveTimer::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//	ウィンドウサイズの取得
	float width = static_cast<float>(pDR->GetOutputSize().right);

	DirectX::SimpleMath::Vector2 oneNumPos{ width / 2 + 30, 30 };
	DirectX::SimpleMath::Vector2 hundredNumPos{ oneNumPos.x - 120, oneNumPos.y };
	DirectX::SimpleMath::Vector2 tenNumPos{ oneNumPos.x - 60, oneNumPos.y };

	//数字の切り取り
	RECT rectNum[10] = {
	{0	,0, 52,64},//0
	{52	,0, 97,64},//1
	{96	,0,140,64},//2
	{140,0,190,64},//3
	{190,0,243,64},//4
	{243,0,293,64},//5
	{293,0,341,64},//6
	{341,0,388,64},//7
	{388,0,436,64},//8
	{436,0,483,64} //9
	};

	//数字の表示
	//一の位の表示
	m_spriteBatch->Draw(m_numTexture.Get(), oneNumPos, &rectNum[static_cast<int>(m_timer_s) % 10], DirectX::Colors::White, 0.0f, { 0.0f,0.0f }, 1, DirectX::SpriteEffects_None, 0);
	//十の位の表示
	m_spriteBatch->Draw(m_numTexture.Get(), tenNumPos, &rectNum[static_cast<int>(m_timer_s / 10) % 10], DirectX::Colors::White, 0.0f, { 0.0f,0.0f }, 1, DirectX::SpriteEffects_None, 0);
	//百の位の表示
	m_spriteBatch->Draw(m_numTexture.Get(), hundredNumPos, &rectNum[static_cast<int>(m_timer_s / 100) % 10], DirectX::Colors::White, 0.0f, { 0.0f,0.0f }, 1, DirectX::SpriteEffects_None, 0);


}

/// <summary>
/// 終了処理
/// </summary>
void AliveTimer::Finalize()
{
}
