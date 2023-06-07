#include"pch.h"
#include"ModeSelect.h"
#include"Libraries/MyLibraries/TextureManager.h"
#include"Game/PlayScene/GameContext/GameContext.h"
#include"Libraries/AXD2/Adx2.h"
#include"DeviceResources.h"
#include"../../Helpers/CueSheet_0.h"
#include"../../Helpers/DODGESound_acf.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ModeSelect::ModeSelect()
	:
	m_modeFilePaths{},
	m_playerMode(GameMain::PlayerMode::Player1)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ModeSelect::~ModeSelect()
{
}

/// <summary>
/// ������
/// </summary>
void ModeSelect::Initialize()
{
	//���[�h�t�@�C���p�X�̐ݒ�
	m_modeFilePaths.resize(static_cast<int>(GameMain::PlayerMode::Player2));
	m_modeFilePaths[0] = L"Resources/Textures/SOLO.png";
	m_modeFilePaths[1] = L"Resources/Textures/MARUTI.png";

	//�e�N�X�`���}�l�[�W���[�̎擾
	TextureManager& textureManager = TextureManager::GetInstance();

	//�摜�̓ǂݍ���
	for (int i = 0; i < m_modeFilePaths.size(); i++)
	{
		m_modeSelectTextures.push_back(textureManager.LoadTexture(m_modeFilePaths[i].c_str()));
	}

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
/// <returns>���̑I����ʂɍs����</returns>
bool ModeSelect::Update(const DX::StepTimer& timer)
{
	//�L�[�{�[�h�X�e�[�g�g���b�J�[�̎擾
	DirectX::Keyboard::KeyboardStateTracker* keybord = GameContext::GetInstance().GetKeyboardStateTracker();

	//���[�h�I��
	if (keybord->IsKeyPressed(DirectX::Keyboard::Right))
	{
		m_playerMode = GameMain::PlayerMode::Player2;

		ADX2::GetInstance().Play(CRI_CUESHEET_0_BUTTON);
	}
	else if (keybord->IsKeyPressed(DirectX::Keyboard::Left))
	{
		m_playerMode = GameMain::PlayerMode::Player1;

		ADX2::GetInstance().Play(CRI_CUESHEET_0_BUTTON);
	}

	//�X�y�[�X�L�[���������Ɉڍs����
	if (keybord->IsKeyPressed(DirectX::Keyboard::Space))
	{
		ADX2::GetInstance().Play(CRI_CUESHEET_0_BUTTON);

		return true;
	}

	return false;
}

/// <summary>
/// �`��
/// </summary>
void ModeSelect::Draw()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// �E�B���h�E�T�C�Y�̎擾
	RECT size = pDR->GetOutputSize();

	//window�T�C�Y��4���̂P�̒l
	float windowHeightQuarter = static_cast<float>(size.right / 4.0f);
	//window�T�C�Y�̔����̒l
	float windowHeightHelf = static_cast<float>(size.right / 2.0f);
	//���[�h�摜��Y���W
	float modeSelectPosY = static_cast<float>(size.bottom / 2.0f + size.bottom / 4.0);

	//���[�h�̉摜���W
	DirectX::SimpleMath::Vector2 modeSelectPosition[2] =
	{
		//�\�����[�h
		DirectX::SimpleMath::Vector2{windowHeightHelf - windowHeightQuarter , modeSelectPosY },
		//�}���`���[�h
		DirectX::SimpleMath::Vector2{windowHeightHelf + windowHeightQuarter , modeSelectPosY },

	};

	//�摜�̊g�嗦
	float scale = 2.0f;

	for (int i = 0; i < m_modeSelectTextures.size(); i++)
	{
		//�J���[
		DirectX::SimpleMath::Vector4 color = DirectX::Colors::White;

		//�摜�̒��S�ʒu
		const DirectX::SimpleMath::Vector2& center = textureManager.GetTextureSize(m_modeFilePaths[i].c_str()) / 2.0f;

		//���݂̑I�����Ă��郂�[�h��ԐF�ŕ\������
		if (i == static_cast<int>(m_playerMode) - 1)
		{
			color = DirectX::Colors::Red;
		}

		//�摜�`��
		GameContext::GetInstance().GetSpriteBatcth()->Draw(m_modeSelectTextures[i].Get(), modeSelectPosition[i], nullptr, color, 0.0f, center, scale);
	}


}

/// <summary>
/// �I������
/// </summary>
void ModeSelect::Finalize()
{
}
