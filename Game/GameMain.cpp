//
// Scene.cpp
//
#include "pch.h"

#include "DeviceResources.h"
#include "GameMain.h"

#include "Libraries/MyLibraries/DebugCamera.h"
#include "Libraries/MyLibraries/GridFloor.h"

#include "Game/TitleScene/TitleScene.h"
#include "Game/PlayScene/PlayScene.h"
#include"Game/ResultScene/ResultScene.h"

#include "Game/LoadingScreen/LoadingScreen.h"

#include"PlayScene/Stage/StageManager.h"
#include"PlayScene/Player/Player.h"
#include"PlayScene/Obstacle/ObstacleManeger.h"
#include"PlayScene/Item/ItemManeger.h"

// ���O��Ԃ̗��p
using namespace DirectX;

//-------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------
GameMain::GameMain()
	: m_nextScene(GAME_SCENE::TITLE)		// �����V�[���̐ݒ�
	, m_pScene(nullptr)
	,m_playerMode(GameMain::PlayerMode::NONE)
	, m_stageNum(0)
{
}

//-------------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------------
GameMain::~GameMain()
{
	Finalize();
}

//-------------------------------------------------------------------
// ����������
//-------------------------------------------------------------------
void GameMain::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// �L�[�{�[�h�֘A
	m_keybord             = std::make_unique<DirectX::Keyboard>();

	// �}�E�X�֘A
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(pDR->GetHwnd());

	// �V�[���쐬
	CreateScene();
}

//-------------------------------------------------------------------
// �X�V����
//-------------------------------------------------------------------
void GameMain::Update(const DX::StepTimer& timer)
{
	// �L�[���͏����擾����
	DirectX::Keyboard::State keyState = m_keybord->GetState();

	// ESC�L�[�ŏI��
	if (keyState.Escape)
	{
		PostQuitMessage(0);
	}

	// ���̃V�[�����ݒ肳��Ă�����V�[���؂�ւ�
	if (m_nextScene != GAME_SCENE::NONE)
	{
		// �V�[���폜
		DeleteScene();
		
		// �V�[���쐬
		CreateScene();
	}
	// ���[�h��ʂ̎��̂�����΍X�V
	if (m_loadingScreen != nullptr)
	{
		m_loadingScreen->Update(timer);

		if (m_loadingScreen->IsLoading() == true)
		{
			return;
		}
		else
		{
			m_loadingScreen->Finalize();
			m_loadingScreen.reset();
		}
	}

	// ���Ԃ�����΍X�V
	if (m_pScene != nullptr)
	{
		m_nextScene = m_pScene->Update(timer);
	}
}

//-------------------------------------------------------------------
// �`�揈��
//-------------------------------------------------------------------
void GameMain::Render()
{

	// ���[�h��ʂ̎��̂�����Ε`��
	if (m_loadingScreen != nullptr)
	{
		m_loadingScreen->Draw();
		return;
	}
	// ���Ԃ�����Ε`��
	if (m_pScene != nullptr)
	{
		m_pScene->Draw();
	}
}

//-------------------------------------------------------------------
// ��n��
//-------------------------------------------------------------------
void GameMain::Finalize()
{
	DeleteScene();
}

/*--------------------------------------------------
�V�[���̍쐬
--------------------------------------------------*/
void GameMain::CreateScene()
{
	// �V�[�����쐬����Ă���Ƃ��͏������Ȃ�
	if (m_pScene != nullptr)
	{
		return;
	}

	// ���V�[���̍쐬
	switch (m_nextScene)
	{
	case GAME_SCENE::TITLE:
	{
		m_pScene = new TitleScene();
		break;
	}
	case GAME_SCENE::PLAY:
	{
		m_pScene = new PlayScene();

		PlayScene* playScene = dynamic_cast<PlayScene*> (m_pScene);

		if (playScene != nullptr)
		{
			playScene->SetPlayerMode(m_playerMode);
			playScene->SetStageNum(m_stageNum);
		}

		break;
	}
	case GAME_SCENE::RESULT:
	{
		m_pScene = new ResultScene();

		ResultScene* resultScene = dynamic_cast<ResultScene*> (m_pScene);

	

		break;
	}
	default:
	{
		// ��O�Ȃ̂ŏ����𒆒f
		return;
	}
	}
	// ���\�[�X�̓ǂݍ���
	LoadResources(true);

	// �쐬�����V�[����������
	m_pScene->Initialize();



	// �V�[����؂�ւ����̂Ń��Z�b�g
	m_nextScene = GAME_SCENE::NONE;
}

/*--------------------------------------------------
�V�[���̍폜
--------------------------------------------------*/
void GameMain::DeleteScene()
{
	// �V�[�����쐬����Ă��Ȃ���Ώ������Ȃ�
	if (m_pScene == nullptr)
	{
		return;
	}
	
	switch (m_nextScene)
	{
	case GAME_SCENE::TITLE:
	{
	

		break;
	}
	case GAME_SCENE::PLAY:
	{
		TitleScene* titleScene = dynamic_cast<TitleScene*> (m_pScene);

		if (titleScene != nullptr)
		{
			m_playerMode = titleScene->GetPlayerMode();
			m_stageNum   = titleScene->GetStageNum();
		}

		break;
	}
	case GAME_SCENE::RESULT:
	{


		break;
	}
	}

	// ���V�[���̏I������
	m_pScene->Finalize();

	// ���V�[���̍폜
	delete m_pScene;
	m_pScene = nullptr;
}
/*--------------------------------------------------
���\�[�X�̓ǂݍ���
--------------------------------------------------*/
void GameMain::LoadResources(bool useLoadingScreen)
{
	if (useLoadingScreen)
	{
		m_loadingScreen = std::make_unique<LoadingScreen>();
		m_loadingScreen->Initialize(m_pScene);
	}
	else
	{
		m_pScene->LoadResources();
	}
}