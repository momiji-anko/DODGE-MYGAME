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

// ���O��Ԃ̗��p
using namespace DirectX;

/*--------------------------------------------------
�R���X�g���N�^
--------------------------------------------------*/
PlayScene::PlayScene()
	:
	m_fade(1.0f),
	m_flag(false),
	m_flagFadeIn(false),
	m_waitTime(0.0f),
	m_timer(0.0f),
	m_stratFlag(true),
	m_musicID(0),
	m_pAdx2()
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	m_pCamera = new DebugCamera();
	//	�O���b�h��10�����~10�����ō��
	m_pGridFloor = new GridFloor(device, context, 10, 10);
}

/*--------------------------------------------------
�f�X�g���N�^
--------------------------------------------------*/
PlayScene::~PlayScene()
{
	delete m_pCamera;
	delete m_pGridFloor;
	
	m_pAdx2->Finalize();
}

/*--------------------------------------------------
������
--------------------------------------------------*/
void PlayScene::Initialize()
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	m_fadeInOut = std::make_unique<Fade>();
	m_fadeInOut->Create();
	m_fadeInOut->Initialize(DirectX::SimpleMath::Vector3::Zero, 1.0f);
	m_fadeInOut->FadeIn();


	m_obstacleManeger = std::make_unique< ObstacleManeger>();
	m_obstacleManeger->Initialize(m_commonState.get(), m_stageNum);


	m_shadowMap = std::make_unique<ShadowMap>();
	m_shadowMap->Initialize(device, L"Resources/Shaders");

	m_stageManeger = std::make_unique<StageManager>();
	m_stageManeger->Initialize(m_commonState.get(), m_stageNum);
	m_stageManeger->SetShadow(m_shadowMap.get());



	m_itemManeger = std::make_unique< ItemManager>();
	m_itemManeger->Initialize(m_commonState.get());
	m_itemManeger->SetStageManeger(m_stageManeger.get());

	m_waitTime = 4.0f;
	m_timer = 0.0f;

	m_pAdx2 = &ADX2::GetInstance();

	m_pAdx2->Initialize("Resources/Sounds/DODGESound.acf",
		"Resources/Sounds/CueSheet_0.acb");

	
	
	m_musicID = m_pAdx2->Play(CRI_CUESHEET_0_PLAY);

	for (int i = 0; i < static_cast<int>(m_playerMode); i++)
	{
		m_players.push_back(std::make_unique<Player>());
		m_players[i]->SetStageManeger(m_stageManeger.get());
		m_players[i]->SetIteManeger(m_itemManeger.get());
	}
	DirectX::SimpleMath::Vector3 playersStartPos[2] =
	{
		DirectX::SimpleMath::Vector3{3.0f,0.0f,6.0f} ,
		DirectX::SimpleMath::Vector3{-3.0f,0.0f,6.0f}
	};

	if (m_playerMode == GameMain::PlayerMode::Player1)
	{
		playersStartPos[0] = DirectX::SimpleMath::Vector3(0.0f, 7.0f, 6.0f);
	}

	//�v���C���[�̃L�[�f�[�^
	std::vector<std::vector<DirectX::Keyboard::Keys>> playerKeyData = 
	{ 
		//�v���C���[�P�̃L�[�f�[�^(�E�A���A�O�A���A�W�����v)
		{DirectX::Keyboard::Keys::Right,DirectX::Keyboard::Keys::Left,DirectX::Keyboard::Keys::Up,DirectX::Keyboard::Keys::Down,DirectX::Keyboard::Keys::Space},
		//�v���C���[�Q�̃L�[�f�[�^(�E�A���A�O�A���A�W�����v)
		{DirectX::Keyboard::Keys::D,DirectX::Keyboard::Keys::A,DirectX::Keyboard::Keys::W,DirectX::Keyboard::Keys::S,DirectX::Keyboard::Keys::Z}
	};

	//�v���C���[�̃��f���t�@�C���p�X
	std::vector<std::vector<std::wstring>>playerModelFile = {
		//�v���C���[�P�̃��f���t�@�C���p�X�i�A�C�h����ԁA�����o���Ă����ԁA�E���o���Ă����ԁA�W�����v���Ă����ԁj
		{ L"Resources/Models/playeraidoru.cmo",L"Resources/Models/playerhidari.cmo",L"Resources/Models/playermigiasi.cmo",L"Resources/Models/playerjanp.cmo"},
		//�v���C���[�Q�̃��f���t�@�C���p�X�i�A�C�h����ԁA�����o���Ă����ԁA�E���o���Ă����ԁA�W�����v���Ă����ԁj
		{L"Resources/Models/Player2idoru.cmo",L"Resources/Models/Player2hidari.cmo",L"Resources/Models/Player2Migi.cmo",L"Resources/Models/Player2Janp.cmo"}
	};


	for (int i = 0; i < static_cast<int>(m_playerMode); i++)
	{
		m_players[i]->SetPlayerModelFile(playerModelFile[i]);
		m_players[i]->SetKeys(playerKeyData[i]);
		m_players[i]->SetID(i + 1);
		m_players[i]->Initialize(DirectX::SimpleMath::Vector3::Zero, playersStartPos[i],DirectX::SimpleMath::Vector3(2.0f,2.0f,2.0f), true, 0.0f, nullptr, nullptr, m_commonState.get());
	}

	m_aliveTime = &AliveTimer::GetInstance();
	m_aliveTime->Initialize(m_commonState.get());

	m_obstacleManeger = std::make_unique< ObstacleManeger>();
	m_obstacleManeger->Initialize(m_commonState.get(), m_stageNum);

}

/*--------------------------------------------------
�X�V
�߂�l	:���̃V�[���ԍ�
--------------------------------------------------*/
GAME_SCENE PlayScene::Update(const DX::StepTimer& timer)
{
	float time = timer.GetElapsedSeconds();

	
	m_fadeInOut->Update(timer);
	
	if (m_flag && m_fadeInOut->ISClose())
	{
		return GAME_SCENE::RESULT;
	}

	if (m_flag)
	{
		return GAME_SCENE::NONE;
	}


	if (m_fadeInOut->ISOpen())
		m_flagFadeIn = true;

	if (m_fadeInOut->ISOpen() == false)
		return GAME_SCENE::NONE;


	if (m_waitTime > 0)
	{
		m_waitTime -= time;
		return GAME_SCENE::NONE;
	}
	
	m_stratFlag = false;

	m_aliveTime->Update(timer);


	for (std::vector<std::unique_ptr<Player>>::iterator player = m_players.begin(); player != m_players.end(); ++player)
	{
		(*player)->Update(timer);
	}

	
	m_obstacleManeger->Update(timer);
	m_obstacleManeger->SetPlayerPosition(m_players[MyRandom::GetIntRange(0 , m_players.size() - 1)]->GetPosition());

	m_itemManeger->Update(timer);
	


	m_stageManeger->Update(timer);
	
	//�v���C���[�̃f�X�J�E���g
	int  deathCount = 0;
	for (std::vector<std::unique_ptr<Player>>::iterator player = m_players.begin(); player != m_players.end(); ++player)
	{
		//����ł����ꍇ�J�E���g��i�߂�
		if (m_obstacleManeger->PlayerHitCheck((*player)->GetAABB()) && (*player)->GetNowPlayerState() == Player::PlayerState::NORMAL)
		{
			if ((*player)->GetInvalidCount() <= -1)
			{
				deathCount++;
			}
			else
			{
				(*player)->ShieldCountDown();
			}

		}
	}

	//�v���C���[���S������ł����ꍇ�V�[���J�ڃt���O��ON�ɂ���
	if (deathCount == m_players.size())
	{
		m_flag = true;
	}



	int count = 0;
	for (std::vector<std::unique_ptr<Player>>::iterator player = m_players.begin(); player != m_players.end(); ++player)
	{
		if (!(*player)->IsActive())
		{
			count++;

		}
	}
	if (count == m_players.size())
	{
		m_fadeInOut->FadeOut();
		m_flag = true;
	}

	for (std::vector<std::unique_ptr<Player>>::iterator player = m_players.begin(); player != m_players.end(); ++player)
	{
		if ((*player)->IsActive())
		{
			m_obstacleManeger->PlayerCapsuleHitCheck((*player).get());

		}
	}

	return GAME_SCENE::NONE;
}

/*--------------------------------------------------
�`��
--------------------------------------------------*/
void PlayScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_commonState->DepthRead(), 0);
	//	�E�B���h�E�T�C�Y�̎擾
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);


	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3::Zero);
	DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::CreateRotationY(0.0f);
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(0.005);


	// ���f���`��
	m_pModel->Draw(context, *m_commonState.get(), DirectX::SimpleMath::Matrix::Identity, m_pCamera->GetViewMatrix(),m_pCamera->GetProjectionMatrix());

	m_stageManeger->Draw(m_pCamera);


	m_obstacleManeger->Draw(m_pCamera);

	
	m_itemManeger->Draw(m_pCamera);
	
	
	//�v���C���[�̕`��
	for (std::vector<std::unique_ptr<Player>>::iterator player = m_players.begin(); player != m_players.end(); ++player)
	{
		(*player)->Draw(m_pCamera);
	}

	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonState->NonPremultiplied());



	//�^�C���̕\��
	m_aliveTime->Draw();


	RECT numRect[6] = {
		{0	,70,180,130},//0
		{52	,0	,97	,64},//1
		{96	,0	,140,64},//2
		{140,0	,190,64},//3
		{190,0	,243,64},//4
		{243,0	,293,64},//5
	};
	DirectX::SimpleMath::Vector2 numPos{ width / 2 - 52, height / 2 - 120 };

	if (m_waitTime >= 0)
	{
		if (static_cast<int>(m_waitTime) == 0)
		{
			numPos.x -= 170 / 2;
		}

		m_spriteBatch->Draw(m_numTexture.Get(), numPos, &numRect[static_cast<int>(m_waitTime)], Colors::White, 0.0f, { 0.0f,0.0f }, 2.0f);

	}

	

	for (std::vector<std::unique_ptr<Player>>::iterator player = m_players.begin(); player != m_players.end(); ++player)
	{
		(*player)->TextureDraw(m_spriteBatch.get());
	}
	

	m_spriteBatch->End();
	
	m_fadeInOut->Render();

	
	//�V���h�E�}�b�v
	m_shadowMap->Begin(context);


	DirectX::SimpleMath::Vector3 lightPos(-10.1f, 15.0f,10.1f);

	DirectX::SimpleMath::Matrix lightView = DirectX::SimpleMath::Matrix::CreateLookAt(lightPos, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector3::Up);
	DirectX::SimpleMath::Matrix projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(90.0f), 1.0f, 1.0f, 400.0f);

	m_obstacleManeger->ObstacleShadow(m_shadowMap.get(), lightView, projection);

	for (std::vector<std::unique_ptr<Player>>::iterator player = m_players.begin(); player != m_players.end(); ++player)
	{
		(*player)->PlayerShadow(m_shadowMap.get(), lightView, projection);
	}	
	
	m_itemManeger->Shadow(m_shadowMap.get(),lightView,projection);
	m_shadowMap->End(context, lightView * projection);


}

/*--------------------------------------------------
�I������
--------------------------------------------------*/
void PlayScene::Finalize()
{
}

/*--------------------------------------------------
���\�[�X�̓ǂݍ���
--------------------------------------------------*/
void PlayScene::LoadResources()
{



	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// �X�v���C�g�o�b�`::�f�o�b�O���̕\���ɕK�v
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");


	//	�R�����X�e�[�g::D3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	//
	ModelManager& modelManager = ModelManager::GetInstance();
	//�V�����f���ǂݍ���
	m_pModel = modelManager.LoadModel(L"Resources/Models/tenkyuu.cmo");
	

	m_pModel->UpdateEffects([&](IEffect* effect)
		{
			IEffectLights* lights = dynamic_cast<IEffectLights*>(effect);
			if (lights)
			{
				// ���C�g�̉e�����Ȃ���
				lights->SetAmbientLightColor(DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.0f));
				lights->SetLightEnabled(0, false);
				lights->SetLightEnabled(1, false);
				lights->SetLightEnabled(2, false);
			}
			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				// �G�~�b�V�����F�𔒂ɐݒ肷��
				basicEffect->SetEmissiveColor(DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
			}
		});



	m_model[0] = modelManager.LoadModel(L"Resources/Models/roll.cmo");

	m_model[1] = modelManager.LoadModel(L"Resources/Models/roll2.cmo");

	TextureManager& textureManager = TextureManager::GetInstance();

	m_numTexture = textureManager.LoadTexture(L"Resources/Textures/num.png");

	m_blackTexture = textureManager.LoadTexture(L"Resources/Textures/black.png");

}
