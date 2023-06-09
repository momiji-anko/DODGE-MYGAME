#include"pch.h"
#include"StageSelect.h"
#include"Libraries/MyLibraries/TextureManager.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"DeviceResources.h"
#include"Game/PlayScene/GameContext/GameContext.h"
#include"Libraries/MyLibraries/MemoryLeakDetector.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="stageManager">�X�e�[�W�}�l�[�W���[�̐��|�C���^</param>
StageSelect::StageSelect(StageManager* stageManager)
	:
	m_stageTextTextureFilePath{},
	m_stageNumTextureFilePath{},
	m_stageArrowTextureFilePath{},
	m_texturePosition{DirectX::SimpleMath::Vector2::Zero},
	m_selectStageNum{0},
	m_stageManager{ stageManager },
	m_stageMaxNum{0},
	m_stageTextTexture{},
	m_stageNumTexture{},
	m_stageArrowTexture{}
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
StageSelect::~StageSelect()
{
	m_stageTextTexture.Reset();
	m_stageNumTexture.Reset();
	m_stageArrowTexture.Reset();
}

/// <summary>
/// ������
/// </summary>
void StageSelect::Initialize()
{

	//�f�o�C�X���\�[�X�擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// �E�B���h�E�T�C�Y�̎擾
	RECT size = pDR->GetOutputSize();

	//�e�N�X�`���t�@�C���p�X�̏�����
	m_stageTextTextureFilePath = L"Resources/Textures/stageText.png";
	m_stageNumTextureFilePath = L"Resources/Textures/stageNum.png";
	m_stageArrowTextureFilePath = L"Resources/Textures/Arrow.png";

	//�e�N�X�`���}�l�[�W���[�̎擾
	TextureManager& textureManager = TextureManager::GetInstance();

	//�e�N�X�`���̓ǂݍ���
	m_stageTextTexture = textureManager.LoadTexture(m_stageTextTextureFilePath);
	m_stageNumTexture = textureManager.LoadTexture(m_stageNumTextureFilePath);
	m_stageArrowTexture = textureManager.LoadTexture(m_stageArrowTextureFilePath);


	//�X�e�[�W�摜��Y���W
	float stageSelectPosY = static_cast<float>(size.bottom / 2.0f + size.bottom / 4.0);
	//�X�e�[�W�摜��X���W
	float stagePositionX = static_cast<float>(size.right / 2.0f);
	//�X�e�[�W�摜�̍��W�ݒ�
	m_texturePosition = { stagePositionX,stageSelectPosY };

	//�t�@�C���}�l�[�W���[�̎擾
	FileLoadManager& fileLoadManager = FileLoadManager::GetInstance();
	//�t�@�C���̐��擾
	m_stageMaxNum = fileLoadManager.LoadFile(L"Resources/StageData/").size();


}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
/// <returns>���̑I����ʂɍs����</returns>
bool StageSelect::Update(const DX::StepTimer& timer)
{
	//�Q�[���R���e�L�X�g�擾
	GameContext& gameContext = GameContext::GetInstance();
	//�Q�[���R���e�L�X�g����L�[�{�[�h�X�e�[�g�g���b�J�[�̎擾
	DirectX::Keyboard::KeyboardStateTracker* keyboard = gameContext.GetKeyboardStateTracker();

	//�{�^������������
	bool isKeyPush = false;

	//�X�e�[�W�I��
	if (keyboard->IsKeyPressed(DirectX::Keyboard::Keys::Left))
	{
		//��������true�ɂ���
		isKeyPush = true;

		//�X�e�[�W�̔ԍ������炷
		m_selectStageNum -= 1;

	}
	else if (keyboard->IsKeyPressed(DirectX::Keyboard::Keys::Right))
	{
		//��������true�ɂ���
		isKeyPush = true;

		//�X�e�[�W�̔ԍ��𑝂₷
		m_selectStageNum += 1;
	}


	//��������X�e�[�W�}�l�[�W���[�̃X�e�[�W��ς���
	if (isKeyPush)
	{
		//�����o��
		ADX2::GetInstance().Play(CRI_CUESHEET_0_BUTTON);

		//�X�e�[�W�I���̒l���X�e�[�W�̐����Ⴍ������X�e�[�W�P�ɂ���
		if (m_selectStageNum < static_cast<int>(StageManager::StageSelect::Stage1))
		{
			m_selectStageNum = static_cast<int>(StageManager::StageSelect::Stage1);
		}
		//�X�e�[�W�I���̒l���X�e�[�W�̐����傫���Ȃ�����X�e�[�W�R�ɂ���
		else if (m_selectStageNum > m_stageMaxNum - 1)
		{
			m_selectStageNum = m_stageMaxNum - 1;
		}

		//�w�i�̃X�e�[�W��ς���
		m_stageManager->Initialize(static_cast<StageManager::StageSelect>(m_selectStageNum));

	}

	//�X�y�[�X�L�[�������Ǝ��̑I����ʂɍs���Ɉړ�
	if (keyboard->IsKeyPressed(DirectX::Keyboard::Keys::Space))
	{
		//�����o��
		ADX2::GetInstance().Play(CRI_CUESHEET_0_BUTTON);

		return true;
	}

	return false;
}

/// <summary>
/// �`��
/// </summary>
void StageSelect::Draw()
{
	//�f�o�C�X���\�[�X�擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//�e�N�X�`���}�l�[�W���[�̎擾
	TextureManager& textureManager = TextureManager::GetInstance();

	//�X�e�[�W�ԍ��e�N�X�`���̉摜�T�C�Y���擾
	DirectX::SimpleMath::Vector2 stageNumTextureSize = textureManager.GetTextureSize(m_stageNumTextureFilePath);
	//�X�e�[�W�e�L�X�g�e�N�X�`���̉摜�T�C�Y���擾
	DirectX::SimpleMath::Vector2 stageTextTextureSize = textureManager.GetTextureSize(m_stageTextTextureFilePath);
	//�A���[�e�N�X�`���̉摜�T�C�Y���擾
	DirectX::SimpleMath::Vector2 stageArrowTextureSize = textureManager.GetTextureSize(m_stageArrowTextureFilePath);

	//�Q�[���R���e�L�X�g�擾
	GameContext& gameContext = GameContext::GetInstance();
	//�Q�[���R���e�L�X�g����X�v���C�g�o�b�`�擾
	DirectX::SpriteBatch* spriteBatch= gameContext.GetSpriteBatcth();

	//�����̈�}�X�̃T�C�Y
	float stageNumSize = (stageNumTextureSize.x) / 10;

	//�؂���ʒu
	RECT texRect ={

		stageNumSize * m_selectStageNum + 1,
		0,
		stageNumSize * (m_selectStageNum + 1) - (stageNumSize / 5),
		stageNumTextureSize.y
	};

	//�g�嗦
	float textureScale = 1.5f;

	//�J���[
	DirectX::SimpleMath::Vector4 color = DirectX::Colors::Red;

	//�X�e�[�W�����̕`��
	spriteBatch->Draw(m_stageTextTexture.Get(), m_texturePosition, nullptr, color, 0.0f, stageTextTextureSize / 2.0f, textureScale);

	//�����̈ʒu
	DirectX::SimpleMath::Vector2 numPosition = m_texturePosition + DirectX::SimpleMath::Vector2(stageTextTextureSize.x + stageNumTextureSize.x / 10, 0);

	//�����̕`��
	spriteBatch->Draw(m_stageNumTexture.Get(), numPosition, &texRect, color, 0.0f, DirectX::SimpleMath::Vector2(stageNumSize, stageNumTextureSize.y / 2.0f), textureScale);

	//�I�����Ă���X�e�[�W�ԍ����P��荂����΍����\��
	if (m_selectStageNum > 0)
	{
		spriteBatch->Draw(m_stageArrowTexture.Get(), m_texturePosition - DirectX::SimpleMath::Vector2(stageArrowTextureSize.x + stageTextTextureSize.x, 0), nullptr, DirectX::Colors::White, DirectX::XM_PI, stageArrowTextureSize / 2, textureScale);
	}
	
	//�I�����Ă���X�e�[�W�ԍ����X�e�[�W�̍ő吔���Ⴍ�����
	if (m_selectStageNum < m_stageMaxNum - 1)
	{
		spriteBatch->Draw(m_stageArrowTexture.Get(), numPosition + DirectX::SimpleMath::Vector2(stageArrowTextureSize.x, 0), nullptr, DirectX::Colors::White, 0.0f, stageArrowTextureSize / 2, textureScale);
	}
	
}

/// <summary>
/// �I������
/// </summary>
void StageSelect::Finalize()
{

}