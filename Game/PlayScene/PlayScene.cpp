/*
�v���C�V�[��
*/
#include "pch.h"

#include "DeviceResources.h"
#include "../GameMain.h"
#include "Libraries/MyLibraries/DebugCamera.h"
#include "Libraries/MyLibraries/GridFloor.h"
#include "PlayScene.h"
#include"Game/PlayScene/MyRandom.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Libraries/MyLibraries/TextureManager.h"
#include"Game/PlayScene/GameContext/GameContext.h"

const float PlayScene::COUNT_DOWN_TIME_S = 4.0f;

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayScene::PlayScene(GameMain* parent)
	:
	m_commonState{},
	m_countDownTime(0.0f),
	m_musicID(0),
	m_pAdx2(),
	m_pModel{},
	m_obstacleManager{},
	m_itemManager{},
	m_stageManager{},
	m_pCamera{},
	m_spriteBatch{},
	m_countDownTexture{},
	m_shadowMap{},
	m_fadeInOut{},
	m_aliveTime{},
	m_playerMode(),
	m_stageNum(),
	m_playerManager{},
	m_parent{parent}
{

	m_pCamera = std::make_unique<Camera>();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayScene::~PlayScene()
{
	
	
}

/// <summary>
/// ������
/// </summary>
void PlayScene::Initialize()
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();

	//�t�F�[�h�쐬
	m_fadeInOut = std::make_unique<Fade>();
	m_fadeInOut->Create();
	m_fadeInOut->Initialize(DirectX::SimpleMath::Vector3::Zero);
	m_fadeInOut->FadeIn();

	//��Q���}�l�[�W���[�쐬
	m_obstacleManager = std::make_unique< ObstacleManager>();
	m_obstacleManager->Initialize(m_stageNum);

	//�V���h�E�}�b�v�쐬
	m_shadowMap = std::make_unique<ShadowMap>();
	m_shadowMap->Initialize(device, L"Resources/Shaders");

	//�X�e�[�W�}�l�[�W���[�쐬
	m_stageManager = std::make_unique<StageManager>();
	m_stageManager->Initialize(m_stageNum);
	m_stageManager->SetShadowMap(m_shadowMap.get());

	//�A�C�e���}�l�[�W���[�쐬
	m_itemManager = std::make_unique< ItemManager>();
	m_itemManager->Initialize();
	m_itemManager->SetStageManeger(m_stageManager.get());

	//�J�E���g�_�E��������	
	m_countDownTime = COUNT_DOWN_TIME_S;

	//Adx�擾
	m_pAdx2 = &ADX2::GetInstance();
	//������	
	m_pAdx2->Initialize(
		"Resources/Sounds/DODGESound.acf",
		"Resources/Sounds/CueSheet_0.acb"
	);


	//BGM��炷
	m_musicID = m_pAdx2->Play(CRI_CUESHEET_0_PLAY);

	//�L�[�{�[�h�X�e�[�g�g���b�J�[�̐���
	m_keyboardStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	GameContext& gameContext = GameContext::GetInstance();
	//�R�����X�e�[�g���Q�[���R���e�L�X�g�ɃZ�b�g
	gameContext.SetCommonState(m_commonState.get());
	//�L�[�{�[�h�X�e�[�g�g���b�J�[���Q�[���R���e�L�X�g�ɃZ�b�g
	gameContext.SetKeyboardStateTracker(m_keyboardStateTracker.get());

	//�v���C���[�}�l�[�W���[�̍쐬
	m_playerManager = std::make_unique<PlayerManager>(m_playerMode,m_stageManager.get(),m_itemManager.get(),m_obstacleManager.get());
	//������
	m_playerManager->Initialize();

	//�A���C�u�^�C���擾
	m_aliveTime = &AliveTimer::GetInstance();
	//������	
	m_aliveTime->Initialize();
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>

void PlayScene::Update(const DX::StepTimer& timer)
{
	//�o�ߎ���
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	//�L�[�{�[�h�X�e�[�g�g���b�J�[�̍X�V
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_keyboardStateTracker->Update(keyState);

	//tab�L�[�������Ă����Ԃ��m�F
	m_isTabKey = keyState.IsKeyDown(DirectX::Keyboard::Keys::Tab);

	//�����Ă���ꍇ����ȍ~�̏������s��Ȃ�
	if (m_isTabKey)
	{
		return ;
	}

	//�t�F�[�h�X�V
	m_fadeInOut->Update(timer);
	
	//�t�F�[�h�A�E�g�����Ȃ烊�U���g�V�[���ֈڍs
	if (m_fadeInOut->ISClose())
	{
		m_parent->SceneChange(m_parent->GetResultScene());
	}

	//�t�F�[�h�C������܂ŏ������Ȃ�
	if (m_fadeInOut->ISOpen() == false)
	{
		return ;
	}
	
	//�J�E���g�_�E�����o�ߎ��Ԃň���
	m_countDownTime -= elapsedTime;

	//�J�E���g�_�E�����O�ɂȂ�܂ŏ������Ȃ�
	if (m_countDownTime > 0)
	{
		return ;
	}
	
	//�A���C�u�^�C���X�V
	m_aliveTime->Update(timer);

	//�v���C���[�X�V
	m_playerManager->Update(timer);

	//��Q���}�l�[�W���[�X�V
	m_obstacleManager->Update(timer);
		
	//��Q���}�l�[�W���[�Ƀv���C���[�̍��W�𑗂�
	m_obstacleManager->SetPlayerPosition(m_playerManager->GetRandomPlayerPosition());

	//�A�C�e���}�l�[�W���[�X�V
	m_itemManager->Update(timer);

	//�X�e�[�W�}�l�[�W���[�X�V
	m_stageManager->Update(timer);
	

	//�S�ăv���C���[������ł���΃t�F�[�h�A�E�g����
	if (m_playerManager->GetPlayerIsAllDaed())
	{
		m_fadeInOut->FadeOut();
	}
	
}

/// <summary>
/// �`��
/// </summary>
void PlayScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_commonState->DepthRead(), 0);


	// �V�����f���`��
	m_pModel->Draw(context, *m_commonState.get(), DirectX::SimpleMath::Matrix::Identity, m_pCamera->GetViewMatrix(),m_pCamera->GetProjectionMatrix());

	//�X�e�[�W�}�l�[�W���[�`��
	m_stageManager->Draw(m_pCamera.get());

	//��Q���}�l�[�W���[�`��
	m_obstacleManager->Draw(m_pCamera.get());

	//�A�C�e���}�l�[�W���[�`��
	m_itemManager->Draw(m_pCamera.get());
	
	
	//�v���C���[�̕`��
	m_playerManager->Draw(m_pCamera.get());

	//�摜�̕`��
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	//�^�C���̕\��
	m_aliveTime->Draw();

	//�J�E���g�_�E���`��
	DrawCountDown();
	
	
	//tab�L�[�������Ă����Ԃł���Α�����@��\������
	if (m_isTabKey)
	{
		m_spriteBatch->Draw(m_playerMoveKey.Get(), DirectX::SimpleMath::Vector2::Zero, nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero, 1.0f);
	}

	//�摜�`��I��
	m_spriteBatch->End();
	
	//�t�F�[�h�`��
	m_fadeInOut->Render();

	//�e����
	CreateShadow();
}

/// <summary>
/// �I������
/// </summary>
void PlayScene::Finalize()
{
	ModelManager::GetInstance().Reset();

	m_pAdx2->Finalize();
}

/// <summary>
/// ���\�[�X�ǂݍ���
/// </summary>
void PlayScene::LoadResources()
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// �X�v���C�g�o�b�`::�f�o�b�O���̕\���ɕK�v
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	//�X�v���C�g�o�b�`���Q�[���R���e�L�X�g�ɃZ�b�g
	GameContext::GetInstance().SetSpriteBatcth(m_spriteBatch.get());

	//	�R�����X�e�[�g::D3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	//���f���}�l�[�W���[�擾
	ModelManager& modelManager = ModelManager::GetInstance();

	//�V�����f���ǂݍ���
	m_pModel = modelManager.LoadModel(L"Resources/Models/tenkyuu.cmo");
	
	//�V�����f���̈Â��Ƃ��������
	m_pModel->UpdateEffects([&](DirectX::IEffect* effect)
		{
			DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
			if (lights)
			{
				// ���C�g�̉e�����Ȃ���
				lights->SetAmbientLightColor(DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.0f));
				lights->SetLightEnabled(0, false);
				lights->SetLightEnabled(1, false);
				lights->SetLightEnabled(2, false);
			}
			DirectX::BasicEffect* basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			if (basicEffect)
			{
				// �G�~�b�V�����F�𔒂ɐݒ肷��
				basicEffect->SetEmissiveColor(DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
			}
		});

	//�e�N�X�`���}�l�[�W���[�I��
	TextureManager& textureManager = TextureManager::GetInstance();
	//�J�E���g�_�E���̉摜
	m_countDownTexture = textureManager.LoadTexture(L"Resources/Textures/num.png");

	//�v���C���[���[�h�̎擾
	m_playerMode = GameContext::GetInstance().GetPlayerMode();


	//�P�����[�h�ƂQ�����[�h�̑�����@�摜�t�@�C���p�X
	std::wstring playerMoveKeyTex[2] = { 
		{L"Resources/Textures/1playerMoveKey.png"},
		{L"Resources/Textures/2playerMoveKey.png"}
	};

	int tes = static_cast<int>(m_playerMode) - 1;

	//���[�h�ɂ���ēǂݍ��މ摜��ς���
	m_playerMoveKey = textureManager.LoadTexture(playerMoveKeyTex[tes]);

}



/// <summary>
/// �J�E���g�_�E���̕\��
/// </summary>
void PlayScene::DrawCountDown()
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//	�E�B���h�E�T�C�Y�̎擾
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	//�J�E���g�_�E���摜�̐؂���ʒu
	RECT countDownRect[] = {
	{0	,70,180,130},//START
	{52	,0	,97	,64},//1
	{96	,0	,140,64},//2
	{140,0	,190,64},//3
	{190,0	,243,64},//4

	};

	//�J�E���g�_�E���̍��W
	DirectX::SimpleMath::Vector2 countDownPos{ width / 2, height / 2 };

	//�摜�̒��S���W
	DirectX::SimpleMath::Vector2 centerPos{ 26,60 };

	//�J�E���g�_�E���̂��O�ȏ�ł���Ε\��
	if (m_countDownTime >= 0)
	{
		//�J�E���g�_�E�����O�̏ꍇ���S���W��ς���
		if (static_cast<int>(m_countDownTime) == 0)
		{
			centerPos.x += countDownRect[0].right / 2;
		}

		m_spriteBatch->Draw(m_countDownTexture.Get(), countDownPos, &countDownRect[static_cast<int>(m_countDownTime)], DirectX::Colors::White, 0.0f, centerPos, 2.0f);

	}

}

/// <summary>
/// �e����
/// </summary>
void PlayScene::CreateShadow()
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//�V���h�E�}�b�v
	m_shadowMap->Begin(context);

	//���C�g�̍��W
	DirectX::SimpleMath::Vector3 lightPos(-10.1f, 15.0f, 10.1f);
	
	//�r���[�s��
	DirectX::SimpleMath::Matrix lightView = DirectX::SimpleMath::Matrix::CreateLookAt(lightPos, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector3::Up);
	//�v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(90.0f), 1.0f, 1.0f, 400.0f);
	
	//��Q���̉e
	m_obstacleManager->ObstacleShadow(m_shadowMap.get(), lightView, projection);

	//�v���C���[�̉e
	m_playerManager->PlayerShadow(m_shadowMap.get(), lightView, projection);

	//�A�C�e���̉e
	m_itemManager->Shadow(m_shadowMap.get(), lightView, projection);

	
	m_shadowMap->End(context, lightView * projection);

}
