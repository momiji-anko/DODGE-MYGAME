//
// GameMain.h
//
#pragma once

#include <Keyboard.h>
#include <Mouse.h>
#include "StepTimer.h"
#include"ILoadingScreen.h"



// �V�[���̗�
enum class GAME_SCENE : int
{
	NONE,

	TITLE,
	PLAY,
	RESULT,
};




// �O���錾
class DebugCamera;
class IScene;
class TitleScene;

/// <summary>
/// Game�N���X����A���[�U���������𔲂��o�����N���X
/// </summary>
class GameMain
{

public:
	GameMain();
	~GameMain();

	void Initialize();
	void Update(const DX::StepTimer& timer);
	void Render();
	void Finalize();

	// �V�[���֌W����

	// �V�[���̍쐬
	void CreateScene();

	// �V�[���̍폜
	void DeleteScene();

private:
	void LoadResources(bool useLoadingScreen = true);

public:

	//�v���C���[�̃��[�h�i���j
	enum class PlayerMode
	{
		NONE = 0,
		Player1 = 1,
		Player2 = 2
	};

private:

	// �L�[�{�[�h
	std::unique_ptr<DirectX::Keyboard> m_keybord;

	// �}�E�X
	std::unique_ptr<DirectX::Mouse> m_mouse;

	// ���̃V�[���ԍ�
	GAME_SCENE m_nextScene;

	// �V�[��
	std::unique_ptr<IScene> m_pScene;
	

	// ���[�h��� 
	std::unique_ptr<ILoadingScreen> m_loadingScreen;

	PlayerMode m_playerMode;
	int m_stageNum;

	TitleScene* m_pTitleScene;


};
