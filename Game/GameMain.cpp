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
#include"Libraries/MyLibraries/MemoryLeakDetector.h"

#include"Game/PlayScene/PlayScene.h"
#include"Game/TitleScene/TitleScene.h"
#include"Game/ResultScene/ResultScene.h"

/// <summary>
/// �v���C�V�[���̎擾
/// </summary>
/// <returns>�v���C�V�[���̐��|�C���^</returns>
IScene* GameMain::GetPlayScene() 
{
	return m_playScene.get();
}

/// <summary>
/// �^�C�g���V�[���̎擾
/// </summary>
/// <returns>�^�C�g���V�[���̐��|�C���^</returns>
IScene* GameMain::GetTitleScene()
{
	return m_titleScene.get();
}

/// <summary>
/// ���U���g�V�[���̎擾
/// </summary>
/// <returns>���U���g�V�[���̐��|�C���^</returns>
IScene* GameMain::GetResultScene()
{
	return m_resultScene.get();
}

//-------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------
GameMain::GameMain()
	: 
	m_pScene(nullptr),
	m_stageNum(0),
	m_keybord{}, 
	m_mouse{},
	m_loadingScreen{},
	m_playScene{},
	m_titleScene{},
	m_resultScene{}
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
	m_keybord = std::make_unique<DirectX::Keyboard>();

	// �}�E�X�֘A
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(pDR->GetHwnd());

	//�v���C�V�[���̍쐬
	m_playScene = std::make_unique<PlayScene>(this);
	//�^�C�g���V�[���̍쐬
	m_titleScene = std::make_unique<TitleScene>(this);
	//���U���g�V�[���̍쐬
	m_resultScene = std::make_unique<ResultScene>(this);

	//�^�C�g���V�[����ς���
	SceneChange(m_titleScene.get());

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
		m_pScene->Update(timer);
	}

	if (m_titleScene != nullptr)
	{
		m_stageNum = m_titleScene->GetStageNum();
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
	
}

/// <summary>
/// �V�[����ς���
/// </summary>
/// <param name="scene">���̃V�[��</param>
void GameMain::SceneChange(IScene* scene)
{
	//���Ԃ�����ΏI������������
	if(m_pScene!=nullptr)
		m_pScene->Finalize();

	//�V�[����ύX
	m_pScene = scene;

	//���[�\�X�̓ǂݍ���
	LoadResources();
	
	m_pScene->SetStageNum(m_stageNum);

	//�V�[���̏�����
	m_pScene->Initialize();
	

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