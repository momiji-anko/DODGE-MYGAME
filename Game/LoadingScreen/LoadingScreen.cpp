/*
���[�h���
*/

#include "pch.h"

#include "LoadingScreen.h"

#include "DeviceResources.h"
#include "../IScene.h"

#include <WICTextureLoader.h>
#include"Libraries/MyLibraries/TextureManager.h"
#include<WICTextureLoader.h>

//loading�摜�̈ړ����x
const float LoadingScreen::TEXTURE_MOVE_SPEED = 5.f;
//loading�摜�̃��Z�b�g���W
const float LoadingScreen::TEXTURE_POSITION_RESET = -780.0f;

/*--------------------------------------------------
�R���X�g���N�^
--------------------------------------------------*/
LoadingScreen::LoadingScreen()
	:
	m_textureStartPosition{ 0.0f,0.0f }
{
}

/*--------------------------------------------------
�f�X�g���N�^
--------------------------------------------------*/
LoadingScreen::~LoadingScreen()
{
	if (m_loadingThread.joinable())
	{
		m_loadingThread.join();
	}
}

/*--------------------------------------------------
������
--------------------------------------------------*/
void LoadingScreen::Initialize(IScene* targetScene)
{
	// �ΏۃV�[���̃��\�[�X�̓ǂݍ���
	LoadSceneResources(targetScene);

	//�f�o�C�X���\�[�X�擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3D�f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// �R�����X�e�[�g::D3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// �X�v���C�g�o�b�`::�f�o�b�O���̕\���ɕK�v
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	//nowLoading�̓ǂݍ���
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/nowLoading.png",
		nullptr,
		m_texture.ReleaseAndGetAddressOf()
	);

	// �w�i�̓ǂݍ���
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/black.png",
		nullptr,
		m_blackTexture.ReleaseAndGetAddressOf()
	);

	//�E�B���h�E�T�C�Y�擾
	RECT rect = pDR->GetOutputSize();

	//Loading�摜�̃T�C�Y
	DirectX::SimpleMath::Vector2 loadingTextureSize = { 690.f,120.f};

	//Loading�摜�̏����ʒu
	m_textureStartPosition = DirectX::SimpleMath::Vector2(static_cast<float>(rect.right), static_cast<float>(rect.bottom) - loadingTextureSize.y);

	//Loading�摜�̐�
	int loadingTextureNum = 10;

	//Loading�摜�̍��W�����T�C�Y
	m_texturePositions.resize(loadingTextureNum);

	//Loading�摜�̍��W���v�Z
	for (int i=0 ; i<m_texturePositions.size();i++)
	{
		m_texturePositions[i] = DirectX::SimpleMath::Vector2(m_textureStartPosition.x - (i * loadingTextureSize.x), m_textureStartPosition.y);
	}
}

/*--------------------------------------------------
�X�V
--------------------------------------------------*/
void LoadingScreen::Update(const DX::StepTimer& timer)
{
	//�x������
	UNREFERENCED_PARAMETER(timer);

	//�摜�̈ړ�������
	for (DirectX::SimpleMath::Vector2& texPos : m_texturePositions)
	{
		//�ړ�
		texPos.x -= TEXTURE_MOVE_SPEED;

		//�摜�����Z�b�g���W��菬�����Ȃ�����X�^�[�g���W�ɖ߂�
		if (texPos.x < TEXTURE_POSITION_RESET)
		{
			texPos.x = m_textureStartPosition.x;
		}
	}
}

/*--------------------------------------------------
�`��
--------------------------------------------------*/
void LoadingScreen::Draw()
{
	//�摜�`��J�n
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	//�w�i�̍��W
	DirectX::SimpleMath::Vector2 blackTexture(0, 0);
	//�w�i�̕`��
	m_spriteBatch->Draw(m_blackTexture.Get(), blackTexture, nullptr, DirectX::SimpleMath::Vector4::One, 0.0f, DirectX::SimpleMath::Vector2::Zero);
	
	//loading�̕`��
	for (DirectX::SimpleMath::Vector2& texPos : m_texturePositions)
	{
		m_spriteBatch->Draw(m_texture.Get(), texPos);
	}

	//�`��I��
	m_spriteBatch->End();
}

/*--------------------------------------------------
�I������
--------------------------------------------------*/
void LoadingScreen::Finalize()
{
}

/*--------------------------------------------------
	// �ǂݍ��ݒ����ǂ����̎擾
--------------------------------------------------*/
bool LoadingScreen::IsLoading()
{
	return m_isLoading;
}


/*--------------------------------------------------
�V�[���̃��\�[�X�̓ǂݍ���
--------------------------------------------------*/
void LoadingScreen::LoadSceneResources(IScene* targetScene)
{
	//�X���b�h�\�ł���΃X���b�h���N������
	if (m_loadingThread.joinable())
	{
		m_loadingThread.join();
	}
	//���[�h���Ă�����
	m_isLoading = true;
	//�X���b�h���g�p���Ď��̃V�[���̃��\�[�X��ǂݍ���
	m_loadingThread = std::thread(
		[=]
		{
			targetScene->LoadResources();
			m_isLoading = false;
		});
}
