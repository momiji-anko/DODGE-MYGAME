#include"pch.h"
#include"AliveTimer.h"
#include"DeviceResources.h"
#include<WICTextureLoader.h>

AliveTimer::AliveTimer()
	:
	m_timer_s(0.0f),
	m_numTexture(nullptr),
	m_spriteBatch(nullptr),
	m_commonState(nullptr)
{
}

AliveTimer::~AliveTimer()
{
}

void AliveTimer::Initialize(DirectX::CommonStates* commonState)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	
	m_commonState = commonState;

	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/num.png",
		nullptr,
		m_numTexture.ReleaseAndGetAddressOf()
	);
	m_timer_s = 0.0f;

	

}

void AliveTimer::Update(const DX::StepTimer& timer)
{
	float time = timer.GetElapsedSeconds();

	

	m_timer_s += time;
}

void AliveTimer::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	ウィンドウサイズの取得
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	DirectX::SimpleMath::Vector2 onePos{ width / 2 + 30, 30 };
	DirectX::SimpleMath::Vector2 Pos2{ onePos.x - 120, onePos.y };
	DirectX::SimpleMath::Vector2 tenPos{ onePos.x - 60, onePos.y };
	RECT rect[10] = {
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

	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	m_spriteBatch->Draw(m_numTexture.Get(), onePos, &rect[static_cast<int>(m_timer_s) % 10], DirectX::Colors::White, 0.0f, { 0.0f,0.0f }, 1, DirectX::SpriteEffects_None, 0);
	m_spriteBatch->Draw(m_numTexture.Get(), tenPos, &rect[static_cast<int>(m_timer_s / 10) % 10], DirectX::Colors::White, 0.0f, { 0.0f,0.0f }, 1, DirectX::SpriteEffects_None, 0);
	m_spriteBatch->Draw(m_numTexture.Get(), Pos2, &rect[static_cast<int>(m_timer_s / 100) % 10], DirectX::Colors::White, 0.0f, { 0.0f,0.0f }, 1, DirectX::SpriteEffects_None, 0);

	m_spriteBatch->End();

}

void AliveTimer::Finalize()
{
}
