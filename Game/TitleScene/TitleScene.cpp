/*//
�^�C�g���V�[��
*/

#include "pch.h"

#include <WICTextureLoader.h>
#include <SimpleMath.h>

#include "../GameMain.h"
#include "DeviceResources.h"

#include "TitleScene.h"
#include"Libraries/MyLibraries/Camera.h"
#include"Libraries/MyLibraries/TextureManager.h"
#include"Game/PlayScene/GameContext/GameContext.h"

//�^�C�g���̈ړ�����
const float TitleScene::MOVE_TIME = 3.0f;
//�^�C�g���̈ړ��p�x
const float TitleScene::MOVE_RORATION = 0.1f;
//�^�C�g���̏����p�x
const float TitleScene::RORATION_ZERO = 0.0f;
//�^�C�g���̉�]���[�`���̃��Z�b�g
const int TitleScene::ROTATION_RESET_ROUTINE = 2;
//�J�����̉�]���x
const float TitleScene::CAMERA_ROT_SPEED = 0.001f;
//�A���t�@�̍ő�l
const float TitleScene::ALPHA_MAX_NUM = 1.0f;
//�A���t�@�̍ŏ��l
const float TitleScene::ALPHA_MIN_NUM = 0.0f;

/// <summary>
/// �R���X�g���N�^
/// </summary>
TitleScene::TitleScene()
	:
	m_alpha(1.0f),
	m_alphaSpeed(-0.01f),
	m_cameraRot(0.0f),
	m_titleTimer_s(0.0f),
	m_titleRoutine(0),
	m_titlePosition{},
	m_titleRotetion(0.0f),
	m_titleSelect(TitleState::FADEIN),
	m_stageNum(0),
	m_stageManager{},
	m_playerMode(GameMain::PlayerMode::Player1),
	m_titleAlpha(0.7f),
	m_pAdx2(),
	m_musicID(0)
{
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
TitleScene::~TitleScene()
{
	//Adx2�̏I������
	m_pAdx2->Finalize();
}

/// <summary>
/// ������
/// </summary>
void TitleScene::Initialize()
{
	//�f�o�C�X���\�[�X�擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//Adx�擾
	m_pAdx2 = &ADX2::GetInstance();
	//Adx������
	m_pAdx2->Initialize("Resources/Sounds/DODGESound.acf",
		"Resources/Sounds/CueSheet_0.acb");
	//�^�C�g��BGM�𗬂�
	m_musicID = m_pAdx2->Play(CRI_CUESHEET_0_TITLE);

	// �E�B���h�E�T�C�Y�̎擾
	RECT size = pDR->GetOutputSize();
	//�^�C�g���̃|�W�V������������
	m_titlePosition.resize(2);
	m_titlePosition[0] = DirectX::SimpleMath::Vector2(static_cast<float>(-size.right), 0);
	m_titlePosition[1] = DirectX::SimpleMath::Vector2(static_cast<float>(size.right), 0);
	

	//�t�F�[�h�̍쐬
	m_fadeInOut = std::make_unique<Fade>();
	m_fadeInOut->Create();
	m_fadeInOut->Initialize(DirectX::SimpleMath::Vector3::Zero);
	
	//�L�[�{�[�h�X�e�[�g�g���b�J�[�擾
	m_keyboardStateTracker = std::make_unique < DirectX::Keyboard::KeyboardStateTracker >();

	GameContext::GetInstance().SetCommonState(m_commonState.get());

	
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
/// <returns>���̃V�[��</returns>
GAME_SCENE TitleScene::Update(const DX::StepTimer& timer)
{
	// �L�[���͏����擾����
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	//�L�[�{�[�h�X�e�[�g�g���b�J�[�̍X�V
	m_keyboardStateTracker->Update(keyState);
	//�t�F�[�h�̍X�V
	m_fadeInOut->Update(timer);
	

	//�J�����̉�]
	m_cameraRot += CAMERA_ROT_SPEED;

	//�^�C�g���̏�ԍX�V
	TitleStateUpdate();

	//�^�C�g�������̓���
	TitleMove(timer);
	
	//�A���t�@���A���t�@�X�s�[�h�ő���
	m_alpha += m_alphaSpeed;

	//�A���t�@���O���P�ɂȂ�����A���t�@�X�s�[�h�𔽓]������
	if (m_alpha < ALPHA_MIN_NUM || m_alpha > ALPHA_MAX_NUM)
	{
		//�A���t�@�X�s�[�h�𔽓]������
		m_alphaSpeed *= -1.0f;
	}
		
	//�t�F�[�h�A�E�g���Ă����Ԃ��^�C�g���̏�Ԃ��t�F�[�h�A�E�g��Ԃł���΃v���C�V�[���Ɉڍs����
	if (m_fadeInOut->ISClose() && TitleScene::TitleState::FADEOUT == m_titleSelect)
	{
		//BGM���~�߂�
		m_pAdx2->Stop(m_musicID);
		//�v���C�V�[���Ɉڍs
		return GAME_SCENE::PLAY;
	}
	return GAME_SCENE::NONE;
}

/// <summary>
/// �`��
/// </summary>
void TitleScene::Draw()
{
	//�f�o�C�X���\�[�X�擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();


	// �r���[�s��
	DirectX::SimpleMath::Matrix view;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix projection;
	// �J�������W
	DirectX::SimpleMath::Vector3 eye = {cos(m_cameraRot)*18,10,-sin(m_cameraRot)*18};
	// �����_
	DirectX::SimpleMath::Vector3 target = DirectX::SimpleMath::Vector3::Zero;
	// ������x�N�g��
	DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::UnitY;
	// �r���[�s��v�Z
	view = DirectX::SimpleMath::Matrix::CreateLookAt(eye, target, up);
	// �E�B���h�E�T�C�Y�̎擾
	RECT size = pDR->GetOutputSize();
	float aspectRatio = float(size.right) / float(size.bottom);

	//�J�����̌�����͈͂̐ݒ�
	float nearPlane = 1.0f;
	float farPlane = 100.0f;
	float fov = 45.0f;
	//����p�����W�A���Ŏ擾
	float fieldOfView = DirectX::XMConvertToRadians(fov);
	//�v���W�F�N�V�����s��v�Z
	projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
	//�J�����̍쐬
	Camera camera;
	//�v�Z�����v���W�F�N�V�����s���ݒ肷��
	camera.SetProjectionMatrix(projection);
	//�v�Z�����r���[�s���ݒ肷��
	camera.SetViewMatrix(view);
	
	//�X�e�[�W��`��
	m_stageManager->Draw(&camera);
	
	
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	
	//PUSH_SPASE_KEY�̃J���[
	DirectX::SimpleMath::Vector4 pushColor{ 1.0f,1.0f,1.0f,m_alpha };

	//�^�C�g�������`��
	for (DirectX::SimpleMath::Vector2& position : m_titlePosition)
	{
		m_spriteBatch->Draw(m_titileTexture.Get(), position + DirectX::SimpleMath::Vector2(640, 220), nullptr, DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_titleAlpha), m_titleRotetion, DirectX::SimpleMath::Vector2(640, 220));
	}

	//CRIWARE�̍��W
	DirectX::SimpleMath::Vector2 CRIWARETexPos{ static_cast<float>(size.right) - 110.0f, static_cast<float>(size.bottom) - 110.0f };
	//CRIWARE�̊g�嗦
	float CRIWAREScale = 0.1f;
	//CRIWARE�̕\��
	m_spriteBatch->Draw(m_CRIWARETexture.Get(), CRIWARETexPos, nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero, CRIWAREScale);
	
	//PUSH_SPASE_KEY�̍��W
	DirectX::SimpleMath::Vector2 pushSpaseKeyPos(0, 0);

	switch (m_titleSelect)
	{
	case TitleScene::TitleState::TITLE:
	
		//PUSH_SPASE_KEY�̕`��
		m_spriteBatch->Draw(m_pushTexture.Get(), pushSpaseKeyPos, nullptr, pushColor, 0.0f, DirectX::SimpleMath::Vector2::Zero);

		break;
	case TitleScene::TitleState::STAGESELECT:

		// �X�e�[�W�I���̉摜��`��
		DrawStageSelect();
		
		break;

	case TitleScene::TitleState::MODESELECT:
	
		// ���[�h�I���̉摜��`��
		DrawModeSelect();

		break;
	}

	m_spriteBatch->End();

	//�t�F�[�h�̕`��
	m_fadeInOut->Render();

}

/// <summary>
/// �^�C�g���̏�Ԃ̍X�V
/// </summary>
void TitleScene::TitleStateUpdate()
{
	//���݂̃^�C�g���̏�Ԃŏ�����؂�ւ���
	switch (m_titleSelect)
	{
	case TitleScene::TitleState::FADEIN:
		//�t�F�[�h�C���J�n
		m_fadeInOut->FadeIn();

		//�t�F�[�h�C�����I�������^�C�g���̏�ԂɕύX
		if (m_fadeInOut->ISOpen())
		{
			m_titleSelect = TitleState::TITLE;
		}

		break;
	case TitleScene::TitleState::TITLE:

		//�X�y�[�X�L�[����������X�e�[�W�Z���N�g��ԂɕύX
		if (m_keyboardStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Space))
		{
			m_titleSelect = TitleScene::TitleState::STAGESELECT;
			//�I�������o��
			m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);

		}

		break;
	case TitleScene::TitleState::STAGESELECT:

		//�X�e�[�W�Z���N�g�̍X�V
		StageSelectUpdate();

		break;
	case TitleScene::TitleState::MODESELECT:

		//���[�h�Z���N�g�̍X�V
		ModeSelectUpdate();

		break;
	case TitleScene::TitleState::FADEOUT:

		//�t�F�[�h�A�E�g�J�n
		m_fadeInOut->FadeOut();
		
		break;
	default:
		break;
	}

}

/// <summary>
/// �^�C�g�������̓���
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void TitleScene::TitleMove(const DX::StepTimer& timer)
{
	//�^�C�g�������̈ړ���̃A���t�@�l
	static const float MOVED_ALPHA_NUM = 1.0f;
	//�f�o�C�X���\�[�X�擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// �E�B���h�E�T�C�Y�̎擾
	RECT size = pDR->GetOutputSize();

	//�o�ߎ���
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	//�^�C�g���^�C�}�[���o�ߎ��Ԃő���
	m_titleTimer_s += elapsedTime_s;

	//�^�C�g���̈ړ��O�̍��W
	DirectX::SimpleMath::Vector2 titleStartPosition = DirectX::SimpleMath::Vector2(static_cast<float>(size.right), 0.0f);
	//�^�C�g���̈ړ���̍��W
	DirectX::SimpleMath::Vector2 titleMovedPosition = DirectX::SimpleMath::Vector2::Zero;

	
	switch (m_titleRoutine)
	{
	case 0:
		//���[�v�ňړ�
		m_titlePosition[0] = DirectX::SimpleMath::Vector2::Lerp(-titleStartPosition, titleMovedPosition, easeOutCubic(m_titleTimer_s / MOVE_TIME));
		m_titlePosition[1] = DirectX::SimpleMath::Vector2::Lerp(titleStartPosition, titleMovedPosition, easeOutCubic(m_titleTimer_s / MOVE_TIME));
		break;
	case 1:
		//�^�C�}�[������
		m_titleTimer_s = 0;
		m_titleRoutine++;
		//�A���t�@�l���P�ɂ���
		m_titleAlpha = MOVED_ALPHA_NUM;
	case 2:
		//�^�C�g�������̉�]
		m_titleRotetion = Lerp(RORATION_ZERO, MOVE_RORATION, m_titleTimer_s / MOVE_TIME);
		break;
	case 3:
		//�^�C�}�[������
		m_titleTimer_s = 0;
		m_titleRoutine++;
	case 4:
		
		//�^�C�g�������̉�]
		m_titleRotetion = Lerp(MOVE_RORATION, RORATION_ZERO, m_titleTimer_s / MOVE_TIME);
		
		break;
	case 5:
		//�^�C�}�[������
		m_titleTimer_s = 0;
		m_titleRoutine++;
	case 6:

		//�^�C�g�������̉�]
		m_titleRotetion = Lerp(RORATION_ZERO, -MOVE_RORATION, m_titleTimer_s / MOVE_TIME);
		
		break;
	case 7:
		m_titleTimer_s = 0;
		m_titleRoutine++;
	case 8:
		
		//�^�C�g�������̉�]
		m_titleRotetion = Lerp(-MOVE_RORATION, RORATION_ZERO, m_titleTimer_s / MOVE_TIME);
		
		break;
	case 9:
		m_titleTimer_s = 0;
		//��]���J��Ԃ�
		m_titleRoutine = ROTATION_RESET_ROUTINE;
	default:
		break;
	}

	//�ړ����I������玟�ɍs��
	if (m_titleTimer_s > MOVE_TIME)
	{
		m_titleTimer_s = MOVE_TIME;
		m_titleRoutine++;
	}


}

/// <summary>
/// �X�e�[�W�I���̉摜��`��
/// </summary>
void TitleScene::DrawStageSelect()
{
	//�f�o�C�X���\�[�X�擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// �E�B���h�E�T�C�Y�̎擾
	RECT size = pDR->GetOutputSize();

	//�X�e�[�W�Q����X�e�[�W�P�ƂR������Ă��
	float stage2ToStage1And3Distace = static_cast<float>(size.right / 3.0f);
	//�E�B���h�E�̉����̃Z���^�[���W
	float stage2Pos = static_cast<float>(size.right / 2.0f);
	//�X�e�[�W�摜��Y���W
	float stageSelectPosY = static_cast<float>(size.bottom / 2.0f + size.bottom / 4.0);

	//�X�e�[�W�|�W�V����
	DirectX::SimpleMath::Vector2 stageSelectPosition[] =
	{
		//�X�e�[�W�P�̍��W
		DirectX::SimpleMath::Vector2{stage2Pos - stage2ToStage1And3Distace , stageSelectPosY  },
		//�X�e�[�W�Q�̍��W
		DirectX::SimpleMath::Vector2{stage2Pos  , stageSelectPosY },
		//�X�e�[�W�R�̍��W
		DirectX::SimpleMath::Vector2{stage2Pos + stage2ToStage1And3Distace , stageSelectPosY }
	};

	//�X�e�[�W�摜�̐F
	DirectX::SimpleMath::Vector4 stageSelectColor = DirectX::Colors::White;

	//�X�e�[�W�摜�̒��S�_�ʒu
	DirectX::SimpleMath::Vector2 stageTexCenterPos[] =
	{
		//�X�e�[�W�P
		DirectX::SimpleMath::Vector2(117.5f,39.0f),
		//�X�e�[�W�Q
		DirectX::SimpleMath::Vector2(123.0f,39.0f),
		//�X�e�[�W�R
		DirectX::SimpleMath::Vector2(102.0f,39.0f)
	};

	//�X�e�[�W�摜�̊g�嗦
	float stageTexScale = 1.5f;

	//�X�e�[�W�摜�`��
	for (int i = 0; i < m_stageSelectTexture.size(); i++)
	{
		//�J���[
		DirectX::SimpleMath::Vector4 color = stageSelectColor;

		//���݂̑I������Ă���X�e�[�W�͐ԐF�ŕ\������
		if (i == m_stageNum)
		{
			color = DirectX::Colors::Red;
		}
		//�X�e�[�W�`��
		m_spriteBatch->Draw(m_stageSelectTexture[i].Get(), stageSelectPosition[i], nullptr, color, 0.0f, stageTexCenterPos[i], stageTexScale);

	}

}

/// <summary>
/// ���[�h�I���̉摜��`��
/// </summary>
void TitleScene::DrawModeSelect()
{
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

	//���[�h�摜�̐F
	DirectX::SimpleMath::Vector4 modeSelectColor = DirectX::Colors::White;

	float tex[2] = { 92,102 };

	//���[�h�摜�̒��S�_�ʒu
	DirectX::SimpleMath::Vector2 modeTexCenterPos[] =
	{
		//�\�����[�h
		DirectX::SimpleMath::Vector2(92.0f,38.0f),
		//�}���`���[�h
		DirectX::SimpleMath::Vector2(102.0f,38.0f),
	};

	for (int i = 0; i < m_modeSelectTextures.size(); i++)
	{
		//�J���[
		DirectX::SimpleMath::Vector4 color = modeSelectColor;

		//���݂̑I�����Ă��郂�[�h��ԐF�ŕ\������
		if (i == static_cast<int>(m_playerMode) - 1)
		{
			color = DirectX::Colors::Red;
		}

		//���[�h�摜�`��
		m_spriteBatch->Draw(m_modeSelectTextures[i].Get(), modeSelectPosition[i], nullptr, color, 0.0f, modeTexCenterPos[i], 2.0f);
	}

}

/// <summary>
/// �I������
/// </summary>
void TitleScene::Finalize()
{

	
}
/// <summary>
/// ���[�\�X�̓ǂݍ���
/// </summary>
void TitleScene::LoadResources()
{
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

	TextureManager& textureManager = TextureManager::GetInstance();
	
	m_titileTexture = textureManager.LoadTexture(L"Resources/Textures/title.png");
	m_pushTexture = textureManager.LoadTexture(L"Resources/Textures/PushSpaceKey.png");
	m_CRIWARETexture = textureManager.LoadTexture(L"Resources/Textures/CRIWARELOGO_1.png");



	//�X�e�[�W�}�l�[�W���[
	m_stageManager = std::make_unique<StageManager>();
	//������
	m_stageManager->Initialize(StageManager::StageSelect::Stage1);

	
	m_modeSelectTextures.resize(static_cast<int>(GameMain::PlayerMode::Player2));

	//�v���C���[���[�h�摜�̃t�@�C���p�X
	std::wstring modeTexFileName[] =
	{
		L"Resources/Textures/SOLO.png",
		L"Resources/Textures/MARUTI.png",
	};

	//�v���C���[���[�h�̃e�N�X�`���̃��[�h
	for (int i = 0; i < static_cast<int>(GameMain::PlayerMode::Player2); i++)
	{
		m_modeSelectTextures[i] = textureManager.LoadTexture(modeTexFileName[i].c_str());
		
	}

	//�X�e�[�W�摜�̐�
	int stageSelectTextureNum = 3;
	//�X�e�[�W�摜�e�N�X�`���̃��[�h
	m_stageSelectTexture.resize(stageSelectTextureNum);
	//�X�e�[�W�摜�̃t�@�C���p�X
	std::wstring stageTexFileName[] =
	{
		L"Resources/Textures/stage1.png",
		L"Resources/Textures/stage2.png",
		L"Resources/Textures/stage3.png",
	};

	//�X�e�[�W�摜�e�N�X�`���̃��[�h
	for (int i = 0; i < m_stageSelectTexture.size(); i++)
	{
		m_stageSelectTexture[i] = textureManager.LoadTexture(stageTexFileName[i].c_str());
	}
	

}

/// <summary>
/// �X�e�[�W�Z���N�g�̍X�V
/// </summary>
void TitleScene::StageSelectUpdate()
{
	//�{�^������������
	bool isKeyPush = false;

	//�X�e�[�W�I��
	if (m_keyboardStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Left))
	{
		//��������true�ɂ���
		isKeyPush = true;

		m_stageNum -= 1;
		m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);

	}
	else if (m_keyboardStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Right))
	{
		//��������true�ɂ���
		isKeyPush = true;

		m_stageNum += 1;

		m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);
	}

	//�X�e�[�W�I���̒l���X�e�[�W�̐����Ⴍ������X�e�[�W�P�ɂ���
	if (m_stageNum < static_cast<int>(StageManager::StageSelect::Stage1))
	{
		m_stageNum = static_cast<int>(StageManager::StageSelect::Stage1);
	}
	//�X�e�[�W�I���̒l���X�e�[�W�̐����傫���Ȃ�����X�e�[�W�R�ɂ���
	else if (m_stageNum > static_cast<int>(StageManager::StageSelect::Stage3))
	{
		m_stageNum = static_cast<int>(StageManager::StageSelect::Stage3);
	}

	//��������X�e�[�W�}�l�[�W���[�̃X�e�[�W��ς���
	if (isKeyPush)
	{
		m_stageManager->Initialize( static_cast<StageManager::StageSelect>(m_stageNum));
	}

	//�X�y�[�X�L�[�������ƃ��[�h�Z���N�g�Ɉړ�
	if (m_keyboardStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Space))
	{
		m_titleSelect = TitleScene::TitleState::MODESELECT;
		m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);

	}
}

/// <summary>
/// ���[�h�Z���N�g�̍X�V
/// </summary>
void TitleScene::ModeSelectUpdate()
{
	//���[�h�I��
	if (m_keyboardStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Left))
	{
		m_playerMode = GameMain::PlayerMode::Player1;
		m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);

	}
	if (m_keyboardStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Right))
	{
		m_playerMode = GameMain::PlayerMode::Player2;
		m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);

	}

	//�X�y�[�X�L�[�������ƃt�F�[�h�A�E�g���v���C�V�[���Ɉڍs����
	if (m_keyboardStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Space))
	{

		m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);
		m_titleSelect = TitleState::FADEOUT;

	}
}
