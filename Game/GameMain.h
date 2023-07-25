//
// GameMain.h
//
#pragma once

#include <Keyboard.h>
#include <Mouse.h>
#include "StepTimer.h"
#include"ILoadingScreen.h"

class ResultScene;
class PlayScene;
class TitleScene;

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

/// <summary>
/// Game�N���X����A���[�U���������𔲂��o�����N���X
/// </summary>
class GameMain
{
public:
	/// <summary>
	/// �v���C�V�[���̎擾
	/// </summary>
	/// <returns>�v���C�V�[���̐��|�C���^</returns>
	IScene* GetPlayScene();

	/// <summary>
	/// �^�C�g���V�[���̎擾
	/// </summary>
	/// <returns>�^�C�g���V�[���̐��|�C���^</returns>
	IScene* GetTitleScene();

	/// <summary>
	/// ���U���g�V�[���̎擾
	/// </summary>
	/// <returns>���U���g�V�[���̐��|�C���^</returns>
	IScene* GetResultScene();

public:
	GameMain();
	~GameMain();

	void Initialize();
	void Update(const DX::StepTimer& timer);
	void Render();
	void Finalize();

	// �V�[���֌W����

	/// <summary>
	/// �V�[����ς���
	/// </summary>
	/// <param name="scene">���̃V�[��</param>
	void SceneChange(IScene* scene);


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


	// �V�[��
	IScene* m_pScene;
	//�v���C�V�[��
	std::unique_ptr<PlayScene>   m_playScene;
	//�^�C�g���V�[��
	std::unique_ptr<TitleScene>  m_titleScene;
	//���U���g�V�[��
	std::unique_ptr<ResultScene> m_resultScene;
		

	// ���[�h��� 
	std::unique_ptr<ILoadingScreen> m_loadingScreen;

	int m_stageNum;

};
