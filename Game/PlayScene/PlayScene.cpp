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
PlayScene::PlayScene()
	:
	m_commonState{},
	m_countDownTime(0.0f),
	m_musicID(0),
	m_pAdx2(),
	m_pModel{},
	m_obstacleManager{},
	m_itemManeger{},
	m_stageManeger{},
	m_pCamera{},
	m_spriteBatch{},
	m_countDownTexture{},
	m_shadowMap{},
	m_fadeInOut{},
	m_aliveTime{},
	m_players{},
	m_playerMode(),
	m_stageNum()
{

	m_pCamera = std::make_unique<Camera>();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayScene::~PlayScene()
{
	
	m_pAdx2->Finalize();
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
	m_obstacleManager->Initialize( m_stageNum);

	//�V���h�E�}�b�v�쐬
	m_shadowMap = std::make_unique<ShadowMap>();
	m_shadowMap->Initialize(device, L"Resources/Shaders");

	//�X�e�[�W�}�l�[�W���[�쐬
	m_stageManeger = std::make_unique<StageManager>();
	m_stageManeger->Initialize( m_stageNum);
	m_stageManeger->SetShadowMap(m_shadowMap.get());

	//�A�C�e���}�l�[�W���[�쐬
	m_itemManeger = std::make_unique< ItemManager>();
	m_itemManeger->Initialize();
	m_itemManeger->SetStageManeger(m_stageManeger.get());

	//�J�E���g�_�E��������	
	m_countDownTime = COUNT_DOWN_TIME_S;

	//Adx�擾
	m_pAdx2 = &ADX2::GetInstance();
	//������	
	m_pAdx2->Initialize("Resources/Sounds/DODGESound.acf",
		"Resources/Sounds/CueSheet_0.acb");

	//�v���C���[�쐬
	CreatePlayer();

	//BGM��炷
	m_musicID = m_pAdx2->Play(CRI_CUESHEET_0_PLAY);

	//�A���C�u�^�C���擾
	m_aliveTime = &AliveTimer::GetInstance();
	//������	
	m_aliveTime->Initialize();

	//�L�[�{�[�h�X�e�[�g�g���b�J�[�̐���
	m_keyboardStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	GameContext& gameContext = GameContext::GetInstance();
	//�R�����X�e�[�g���Q�[���R���e�L�X�g�ɃZ�b�g
	gameContext.SetCommonState(m_commonState.get());
	//�X�v���C�g�o�b�`���Q�[���R���e�L�X�g�ɃZ�b�g
	gameContext.SetSpriteBatcth(m_spriteBatch.get());
	//�L�[�{�[�h�X�e�[�g�g���b�J�[���Q�[���R���e�L�X�g�ɃZ�b�g
	gameContext.SetKeyboardStateTracker(m_keyboardStateTracker.get());

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
/// <returns>���̃V�[��</returns>
GAME_SCENE PlayScene::Update(const DX::StepTimer& timer)
{
	//�o�ߎ���
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	//�L�[�{�[�h�X�e�[�g�g���b�J�[�̍X�V
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_keyboardStateTracker->Update(keyState);

	m_isTabKey = keyState.IsKeyDown(DirectX::Keyboard::Keys::Tab);

	if (m_isTabKey)
	{
		return GAME_SCENE::NONE;
	}

	//�t�F�[�h�X�V
	m_fadeInOut->Update(timer);
	
	//�t�F�[�h�A�E�g�����Ȃ烊�U���g�V�[���ֈڍs
	if (m_fadeInOut->ISClose())
	{
		return GAME_SCENE::RESULT;
	}

	//�t�F�[�h�C������܂ŏ������Ȃ�
	if (m_fadeInOut->ISOpen() == false)
	{
		return GAME_SCENE::NONE;
	}
	
	//�J�E���g�_�E�����o�ߎ��Ԃň���
	m_countDownTime -= elapsedTime;

	//�J�E���g�_�E�����O�ɂȂ�܂ŏ������Ȃ�
	if (m_countDownTime > 0)
	{
		return GAME_SCENE::NONE;
	}
	
	//�A���C�u�^�C���X�V
	m_aliveTime->Update(timer);

	//�v���C���[�X�V
	for (std::vector<std::unique_ptr<Player>>::iterator player = m_players.begin(); player != m_players.end(); ++player)
	{
		(*player)->Update(timer);
	}

	//��Q���}�l�[�W���[�X�V
	m_obstacleManager->Update(timer);
	
	//��Q���ɓn���v���C���[�̔ԍ�
	size_t randomPlayer = static_cast<size_t>(MyRandom::GetIntRange(0, m_players.size() - 1));
	//��Q���}�l�[�W���[�Ƀv���C���[�̍��W�𑗂�
	m_obstacleManager->SetPlayerPosition(m_players[randomPlayer]->GetPosition());

	//�A�C�e���}�l�[�W���[�X�V
	m_itemManeger->Update(timer);

	//�X�e�[�W�}�l�[�W���[�X�V
	m_stageManeger->Update(timer);
	

	//�S�ăv���C���[������ł���΃t�F�[�h�A�E�g����
	if (AllPlayerIsDead())
	{
		m_fadeInOut->FadeOut();
		
	}

	return GAME_SCENE::NONE;
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
	m_stageManeger->Draw(m_pCamera.get());

	//��Q���}�l�[�W���[�`��
	m_obstacleManager->Draw(m_pCamera.get());

	//�A�C�e���}�l�[�W���[�`��
	m_itemManeger->Draw(m_pCamera.get());
	
	
	//�v���C���[�̕`��
	for (std::vector<std::unique_ptr<Player>>::iterator player = m_players.begin(); player != m_players.end(); ++player)
	{
		(*player)->Draw(m_pCamera.get());
	}

	//�摜�̕`��
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	//�^�C���̕\��
	m_aliveTime->Draw();

	//�J�E���g�_�E���`��
	DrawCountDown();
	
	//�v���C���[�̏��`��
	for (std::vector<std::unique_ptr<Player>>::iterator player = m_players.begin(); player != m_players.end(); ++player)
	{
		(*player)->TextureDraw(m_spriteBatch.get());
	}
	
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

	std::wstring playerMoveKeyTex[2] = { 
		{L"Resources/Textures/1playerMoveKey.png"},
		{L"Resources/Textures/2playerMoveKey.png"}
	};

	m_playerMoveKey = textureManager.LoadTexture(playerMoveKeyTex[static_cast<int>(m_playerMode) - 1]);

}

/// <summary>
/// �v���C���[�̍쐬
/// </summary>
void PlayScene::CreatePlayer()
{


	//�v���C���[�̃L�[�f�[�^
	std::vector<std::vector<DirectX::Keyboard::Keys>> playerKeyData =
	{
		//�v���C���[�P�̃L�[�f�[�^(�E�A���A�O�A���A�W�����v)
		{DirectX::Keyboard::Keys::Right,DirectX::Keyboard::Keys::Left,DirectX::Keyboard::Keys::Up,DirectX::Keyboard::Keys::Down,DirectX::Keyboard::Keys::RightShift},	
		//�v���C���[�Q�̃L�[�f�[�^(�E�A���A�O�A���A�W�����v)
		{DirectX::Keyboard::Keys::D,DirectX::Keyboard::Keys::A,DirectX::Keyboard::Keys::W,DirectX::Keyboard::Keys::S,DirectX::Keyboard::Keys::Space}
	};

	//�v���C���[�̃��f���t�@�C���p�X
	std::vector<std::vector<std::wstring>>playerModelFile = {
		//�v���C���[�P�̃��f���t�@�C���p�X�i�A�C�h����ԁA�����o���Ă����ԁA�E���o���Ă����ԁA�W�����v���Ă����ԁj
		{ L"Resources/Models/playeraidoru.cmo",L"Resources/Models/playerhidari.cmo",L"Resources/Models/playermigiasi.cmo",L"Resources/Models/playerjanp.cmo"},
		//�v���C���[�Q�̃��f���t�@�C���p�X�i�A�C�h����ԁA�����o���Ă����ԁA�E���o���Ă����ԁA�W�����v���Ă����ԁj
		{L"Resources/Models/Player2idoru.cmo",L"Resources/Models/Player2hidari.cmo",L"Resources/Models/Player2Migi.cmo",L"Resources/Models/Player2Janp.cmo"}
	};

	//��l�p�v���C���[�X�^�[�g���W
	DirectX::SimpleMath::Vector3 playersStartPos[2] =
	{
		DirectX::SimpleMath::Vector3{3.0f,0.0f,6.0f} ,
		DirectX::SimpleMath::Vector3{-3.0f,0.0f,6.0f}
	};

	//�v���C���[���[�h��1�l���[�h�Ȃ��l�p�̃X�^�[�g���W�A�L�[�f�[�^�ɂ���
	if (m_playerMode == GameMain::PlayerMode::Player1)
	{
		playersStartPos[0] = DirectX::SimpleMath::Vector3(0.0f, 7.0f, 6.0f);
		playerKeyData[0] = { DirectX::Keyboard::Keys::Right,DirectX::Keyboard::Keys::Left,DirectX::Keyboard::Keys::Up,DirectX::Keyboard::Keys::Down,DirectX::Keyboard::Keys::Space };
	}
	//�v���C���[���[�h�̐��v���C���[���쐬
	for (int i = 0; i < static_cast<int>(m_playerMode); i++)
	{
		m_players.push_back(std::make_unique<Player>());

		//�X�e�[�W�}�l�[�W���[�ݒ�
		m_players[i]->SetStageManeger(m_stageManeger.get());
		//�A�C�e���}�l�[�W���[�ݒ�
		m_players[i]->SetIteManeger(m_itemManeger.get());
		//��Q���}�l�[�W���[�ݒ�
		m_players[i]->SetObstacleManager(m_obstacleManager.get());
		
		//�v���C���[���f���t�@�C���p�X�ݒ�
		m_players[i]->SetPlayerModelFile(playerModelFile[i]);
		//�v���C���[�̈ړ��L�[�ݒ�
		m_players[i]->SetKeys(playerKeyData[i]);
		//�v���C���[ID�ݒ�
		m_players[i]->SetTypeInt(i);
		
		//������
		m_players[i]->Initialize(DirectX::SimpleMath::Vector3::Zero, playersStartPos[i], DirectX::SimpleMath::Vector3(2.0f, 2.0f, 2.0f), DirectX::SimpleMath::Vector3::Zero, true, nullptr, nullptr);
	}


}

/// <summary>
/// �S�ăv���C���[������ł��邩�m�F
/// </summary>
/// <returns>true=�S�Ď���ł���Afalse=�S�Ď���ł��Ȃ�</returns>
bool PlayScene::AllPlayerIsDead()
{
	//�v���C���[�̃f�X�J�E���g
	int  deathCount = 0;

	//�S�Ẵv���C���[������ł��邩�m�F
	for (std::vector<std::unique_ptr<Player>>::iterator player = m_players.begin(); player != m_players.end(); ++player)
	{
		//�A�N�e�B�u��ԏ�Ԃł͂Ȃ���΃J�E���g�𑝂₷
		if (!(*player)->IsActive())
		{
			deathCount++;
		}
	}

	//�f�X�J�E���g���v���C���[�̓������ł���ΑS�Ẵv���C���[������ł���
	if (deathCount == m_players.size())
	{
		//�S�Ẵv���C���[������ł���
		return true;
	}

	//�S�Ẵv���C���[������ł��Ȃ�
	return false;
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
	for (std::vector<std::unique_ptr<Player>>::iterator player = m_players.begin(); player != m_players.end(); ++player)
	{
		(*player)->CreateShadow(m_shadowMap.get(), lightView, projection);
	}

	//�A�C�e���̉e
	m_itemManeger->Shadow(m_shadowMap.get(), lightView, projection);

	
	m_shadowMap->End(context, lightView * projection);

}
