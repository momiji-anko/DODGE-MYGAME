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
#include"Libraries/MyLibraries/FileLoadManager.h"

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
	m_titleSelect(TitleState::FADEIN),
	m_stageNum(0),
	m_stageManager{},
	m_playerMode(GameMain::PlayerMode::Player1),
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
	

	//�t�F�[�h�̍쐬
	m_fadeInOut = std::make_unique<Fade>();
	m_fadeInOut->Create();
	m_fadeInOut->Initialize(DirectX::SimpleMath::Vector3::Zero);
	
	//�L�[�{�[�h�X�e�[�g�g���b�J�[�擾
	m_keyboardStateTracker = std::make_unique < DirectX::Keyboard::KeyboardStateTracker >();

	GameContext::GetInstance().SetCommonState(m_commonState.get());
	GameContext::GetInstance().SetKeyboardStateTracker(m_keyboardStateTracker.get());
	GameContext::GetInstance().SetSpriteBatcth(m_spriteBatch.get());

	m_stageSelect = std::make_unique<StageSelect>(m_stageManager.get());
	m_stageSelect->Initialize();
	
	m_modeSelect = std::make_unique<ModeSelect>();
	m_modeSelect->Initialize();

	m_titleText = std::make_unique<TitleText>();
	m_titleText->Initialize();
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
	TitleStateUpdate(timer);

	//�^�C�g�������̓���
	m_titleText->Update(timer);

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

	//�^�C�g���̏�Ԃ̕`��
	TitleStateDraw();

	//�^�C�g�������`��
	m_titleText->Draw();


	//CRIWARE�̍��W
	DirectX::SimpleMath::Vector2 CRIWARETexPos{ static_cast<float>(size.right) - 110.0f, static_cast<float>(size.bottom) - 110.0f };
	//CRIWARE�̊g�嗦
	float CRIWAREScale = 0.1f;
	//CRIWARE�̕\��
	m_spriteBatch->Draw(m_CRIWARETexture.Get(), CRIWARETexPos, nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero, CRIWAREScale);
	
	//PUSH_SPASE_KEY�̍��W
	DirectX::SimpleMath::Vector2 pushSpaseKeyPos(0, 0);


	m_spriteBatch->End();

	//�t�F�[�h�̕`��
	m_fadeInOut->Render();

}

/// <summary>
/// �^�C�g���̏�Ԃ̍X�V
/// </summary>
void TitleScene::TitleStateUpdate(const DX::StepTimer& timer)
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
		if(m_stageSelect->Update(timer))
		{
			m_titleSelect = TitleScene::TitleState::MODESELECT;
			m_stageNum = m_stageSelect->GetSelectStageNum();
		}


		break;
	case TitleScene::TitleState::MODESELECT:

		//���[�h�Z���N�g�̍X�V
		if (m_modeSelect->Update(timer))
		{
			m_titleSelect = TitleScene::TitleState::FADEOUT;
			m_playerMode = m_modeSelect->GetPlayerMode();
		}

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
/// �^�C�g���̏�Ԃ̕`��
/// </summary>
void TitleScene::TitleStateDraw()
{

	//PUSH_SPASE_KEY�̍��W
	DirectX::SimpleMath::Vector2 pushSpaseKeyPos(0, 0);

	//PUSH_SPASE_KEY�̃J���[
	DirectX::SimpleMath::Vector4 pushColor{ 1.0f,1.0f,1.0f,m_alpha };

	switch (m_titleSelect)
	{
	case TitleScene::TitleState::TITLE:

		//PUSH_SPASE_KEY�̕`��
		m_spriteBatch->Draw(m_pushTexture.Get(), pushSpaseKeyPos, nullptr, pushColor, 0.0f, DirectX::SimpleMath::Vector2::Zero);

		break;
	case TitleScene::TitleState::STAGESELECT:

		// �X�e�[�W�I���̉摜��`��
		m_stageSelect->Draw();

		break;

	case TitleScene::TitleState::MODESELECT:

		// ���[�h�I���̉摜��`��
		m_modeSelect->Draw();

		break;
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

	//PUSH_SPACE_KEY�̉摜�̓ǂݍ���
	m_pushTexture = textureManager.LoadTexture(L"Resources/Textures/PushSpaceKey.png");
	//CRIWARE�摜�̓ǂݍ���
	m_CRIWARETexture = textureManager.LoadTexture(L"Resources/Textures/CRIWARELOGO_1.png");


	//�X�e�[�W�}�l�[�W���[
	m_stageManager = std::make_unique<StageManager>();
	//������
	m_stageManager->Initialize(StageManager::StageSelect::Stage1);

}

