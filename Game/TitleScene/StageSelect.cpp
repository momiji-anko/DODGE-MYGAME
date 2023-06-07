#include"pch.h"
#include"StageSelect.h"
#include"Libraries/MyLibraries/TextureManager.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"DeviceResources.h"
#include"Game/PlayScene/GameContext/GameContext.h"


StageSelect::StageSelect(StageManager* stageManager)
	:
	m_stageTextTextureFilePath{},
	m_stageNumTextureFilePath{},
	m_stageArrowTextureFilePath{},
	m_nowPosition{DirectX::SimpleMath::Vector2::Zero},
	m_selectStageNum{0},
	m_stageManager{ stageManager },
	m_stageMaxNum{0},
	m_stageTextTexture{},
	m_stageNumTexture{},
	m_stageArrowTexture{}
{
}

StageSelect::~StageSelect()
{
}

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
	m_stageTextTexture = textureManager.LoadTexture(m_stageTextTextureFilePath.c_str());
	m_stageNumTexture = textureManager.LoadTexture(m_stageNumTextureFilePath.c_str());
	m_stageArrowTexture = textureManager.LoadTexture(m_stageArrowTextureFilePath.c_str());

	///���C�Ȃ����������R
	/*
	* 
		�@�@�^�܁R
		�@��/����@�܁R
		�@ |�@3�@(���P]
		�@������ (���P]
		�P�P�P�P�P�P�P�P
	*/

	//�X�e�[�W�摜��Y���W
	float stageSelectPosY = static_cast<float>(size.bottom / 2.0f + size.bottom / 4.0);

	float stagePositionX = static_cast<float>(size.right);

	m_nowPosition = { stagePositionX /2,stageSelectPosY };

	FileLoadManager& fileLoadManager = FileLoadManager::GetInstance();

	m_stageMaxNum = fileLoadManager.LoadFile(L"Resources/StageData/").size();


}

bool StageSelect::Update(const DX::StepTimer& timer)
{
	GameContext& gameContext = GameContext::GetInstance();

	DirectX::Keyboard::KeyboardStateTracker* keyboard = gameContext.GetKeyboardStateTracker();

	//�{�^������������
	bool isKeyPush = false;

	//�X�e�[�W�I��
	if (keyboard->IsKeyPressed(DirectX::Keyboard::Keys::Left))
	{
		//��������true�ɂ���
		isKeyPush = true;

		m_selectStageNum -= 1;
		

	}
	else if (keyboard->IsKeyPressed(DirectX::Keyboard::Keys::Right))
	{
		//��������true�ɂ���
		isKeyPush = true;

		m_selectStageNum += 1;
	}


	//��������X�e�[�W�}�l�[�W���[�̃X�e�[�W��ς���
	if (isKeyPush)
	{
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

		m_stageManager->Initialize(static_cast<StageManager::StageSelect>(m_selectStageNum));

	}

	//�X�y�[�X�L�[�������Ǝ��̑I����ʂɍs���Ɉړ�
	if (keyboard->IsKeyPressed(DirectX::Keyboard::Keys::Space))
	{
		ADX2::GetInstance().Play(CRI_CUESHEET_0_BUTTON);

		return true;
	}

	return false;
}

void StageSelect::Draw()
{
	//�f�o�C�X���\�[�X�擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	TextureManager& textureManager = TextureManager::GetInstance();

	DirectX::SimpleMath::Vector2 stageNumTextureSize = textureManager.GetTextureSize(m_stageNumTextureFilePath.c_str());

	DirectX::SimpleMath::Vector2 stageTextTextureSize = textureManager.GetTextureSize(m_stageTextTextureFilePath.c_str());
	
	DirectX::SimpleMath::Vector2 stageArrowTextureSize = textureManager.GetTextureSize(m_stageArrowTextureFilePath.c_str());


	GameContext& gameContext = GameContext::GetInstance();

	DirectX::SpriteBatch* spriteBatch= gameContext.GetSpriteBatcth();


	float stageNumSize = (stageNumTextureSize.x) / 10;

	
	RECT texRect ={

		stageNumSize * m_selectStageNum,
		0,
		stageNumSize * (m_selectStageNum + 1) - (stageNumSize / 5),
		stageNumTextureSize.y
	};

	float textureScale = 1.5f;

	//�J���[
	DirectX::SimpleMath::Vector4 color = DirectX::Colors::White;

	spriteBatch->Draw(m_stageTextTexture.Get(), m_nowPosition, nullptr, color, 0.0f, stageTextTextureSize / 2.0f, 1.5f);

	DirectX::SimpleMath::Vector2 numPosition = m_nowPosition + DirectX::SimpleMath::Vector2(stageTextTextureSize.x + stageNumTextureSize.x / 10, 0);

	spriteBatch->Draw(m_stageNumTexture.Get(), numPosition, &texRect, color, 0.0f, DirectX::SimpleMath::Vector2(stageNumSize, stageNumTextureSize.y / 2.0f), textureScale);


	if (m_selectStageNum > 0)
	{
		spriteBatch->Draw(m_stageArrowTexture.Get(), m_nowPosition - DirectX::SimpleMath::Vector2(stageArrowTextureSize.x + stageTextTextureSize.x, 0), nullptr, color, DirectX::XM_PI, stageArrowTextureSize / 2, textureScale);
	}
	
	if (m_selectStageNum < m_stageMaxNum - 1)
	{
		spriteBatch->Draw(m_stageArrowTexture.Get(), numPosition + DirectX::SimpleMath::Vector2(stageArrowTextureSize.x, 0), nullptr, color, 0.0f, stageArrowTextureSize / 2, textureScale);
	}
	


}

void StageSelect::Finalize()
{

}
