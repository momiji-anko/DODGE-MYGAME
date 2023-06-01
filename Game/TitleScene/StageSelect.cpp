#include"pch.h"
#include"StageSelect.h"
#include"Libraries/MyLibraries/TextureManager.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"DeviceResources.h"
#include"Game/PlayScene/GameContext/GameContext.h"


void StageSelect::Initialize()
{

	//�f�o�C�X���\�[�X�擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// �E�B���h�E�T�C�Y�̎擾
	RECT size = pDR->GetOutputSize();

	m_stageTextTextureFileName = L"Resources/Textures/stageText.png";
	
	m_stageNumTextureFileName = L"Resources/Textures/stageNum.png";

	m_stageArrowTextureFileName = L"Resources/Textures/stageSelectArrow.png";

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


}

bool StageSelect::Update(const DX::StepTimer& timer)
{
	GameContext& gameContext = GameContext::GetInstance();

	DirectX::Keyboard::KeyboardStateTracker* keyboard = gameContext.GetKeyboardStateTracker();

	//�{�^������������
	bool isKeyPush = false;

	//�X�e�[�W�I��
	if (keyboard->IsKeyPressed(DirectX::Keyboard::Keys::Down))
	{
		//��������true�ɂ���
		isKeyPush = true;

		m_selectStageNum -= 1;
		

	}
	else if (keyboard->IsKeyPressed(DirectX::Keyboard::Keys::Up))
	{
		//��������true�ɂ���
		isKeyPush = true;

		m_selectStageNum += 1;
	}

	FileLoadManager& fileLoadManager = FileLoadManager::GetInstance();

	int stageMaxNum = fileLoadManager.LoadFile(L"Resources/StageData/").size();


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
		else if (m_selectStageNum > stageMaxNum - 1)
		{
			m_selectStageNum = stageMaxNum - 1;
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

	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& stageNumTexture = textureManager.LoadTexture(m_stageNumTextureFileName.c_str());
	DirectX::SimpleMath::Vector2 stageNumTextureSize = textureManager.GetTextureSize(m_stageNumTextureFileName.c_str());

	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& stageTextTexture = textureManager.LoadTexture(m_stageTextTextureFileName.c_str());
	DirectX::SimpleMath::Vector2 stageTextTextureSize = textureManager.GetTextureSize(m_stageTextTextureFileName.c_str());
	
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& stageArrowTexture = textureManager.LoadTexture(m_stageArrowTextureFileName.c_str());
	DirectX::SimpleMath::Vector2 stageArrowTextureSize = textureManager.GetTextureSize(m_stageArrowTextureFileName.c_str());



	GameContext& gameContext = GameContext::GetInstance();

	DirectX::SpriteBatch* spriteBatch= gameContext.GetSpriteBatcth();


	float stageNumSize = (stageNumTextureSize.x) / 10;

	int i = m_selectStageNum;
	RECT texRect =
	{
		stageNumSize * i,
		0,
		stageNumSize * (i + 1) - (stageNumSize / 5),
		stageNumTextureSize.y
	};

	//�J���[
	DirectX::SimpleMath::Vector4 color = DirectX::Colors::White;

	spriteBatch->Draw(stageTextTexture.Get(), m_nowPosition, nullptr, color, 0.0f, stageTextTextureSize / 2.0f, 1.5f);

	DirectX::SimpleMath::Vector2 numPosition = m_nowPosition + DirectX::SimpleMath::Vector2(stageTextTextureSize.x + stageNumTextureSize.x / 10, 0);

	spriteBatch->Draw(stageNumTexture.Get(), numPosition, &texRect, color, 0.0f, DirectX::SimpleMath::Vector2(stageNumSize, stageNumTextureSize.y / 2.0f), 1.5f);

	spriteBatch->Draw(stageArrowTexture.Get(), numPosition - DirectX::SimpleMath::Vector2(0,stageArrowTextureSize.y), &texRect, color, 0.0f, stageArrowTextureSize/2, 0.5f);


}

void StageSelect::Finalize()
{

}
