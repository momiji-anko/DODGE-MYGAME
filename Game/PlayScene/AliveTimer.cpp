/*
* 2023/03/23
* Blink.cpp
* �����@��
*/
#include"pch.h"
#include"AliveTimer.h"
#include"DeviceResources.h"
#include<WICTextureLoader.h>
#include"Libraries/MyLibraries/TextureManager.h"
#include"Game/PlayScene/GameContext/GameContext.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
AliveTimer::AliveTimer()
	:
	m_timer_s(0.0f),
	m_numTexture(nullptr),
	m_spriteBatch(nullptr)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
AliveTimer::~AliveTimer()
{
}

/// <summary>
/// ������
/// </summary>
void AliveTimer::Initialize()
{
	//�Q�[���R���e�L�X�g����X�v���C�g�o�b�`�擾
	m_spriteBatch = GameContext::GetInstance().GetSpriteBatcth();
	//�e�N�X�`���ǂݍ���
	 m_numTexture = TextureManager::GetInstance().LoadTexture(L"Resources/Textures/num.png");

	m_timer_s = 0.0f;
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer"></param>
void AliveTimer::Update(const DX::StepTimer& timer)
{
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	m_timer_s += elapsedTime;
}

/// <summary>
/// �`��
/// </summary>
void AliveTimer::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//	�E�B���h�E�T�C�Y�̎擾
	float width = static_cast<float>(pDR->GetOutputSize().right);

	DirectX::SimpleMath::Vector2 oneNumPos{ width / 2 + 30, 30 };
	DirectX::SimpleMath::Vector2 hundredNumPos{ oneNumPos.x - 120, oneNumPos.y };
	DirectX::SimpleMath::Vector2 tenNumPos{ oneNumPos.x - 60, oneNumPos.y };

	//�����̐؂���
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

	//�����̕\��
	//��̈ʂ̕\��
	m_spriteBatch->Draw(m_numTexture.Get(), oneNumPos, &rectNum[static_cast<int>(m_timer_s) % 10], DirectX::Colors::White, 0.0f, { 0.0f,0.0f }, 1, DirectX::SpriteEffects_None, 0);
	//�\�̈ʂ̕\��
	m_spriteBatch->Draw(m_numTexture.Get(), tenNumPos, &rectNum[static_cast<int>(m_timer_s / 10) % 10], DirectX::Colors::White, 0.0f, { 0.0f,0.0f }, 1, DirectX::SpriteEffects_None, 0);
	//�S�̈ʂ̕\��
	m_spriteBatch->Draw(m_numTexture.Get(), hundredNumPos, &rectNum[static_cast<int>(m_timer_s / 100) % 10], DirectX::Colors::White, 0.0f, { 0.0f,0.0f }, 1, DirectX::SpriteEffects_None, 0);


}

/// <summary>
/// �I������
/// </summary>
void AliveTimer::Finalize()
{
}
