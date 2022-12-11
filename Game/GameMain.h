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

//�X�e�[�W
enum class StageSelect
{
    Stage1,
    Stage2,
    Stage3,
};

// �O���錾
class DebugCamera;

class IScene;

class StageManeger;
class Player;
class ObstacleManeger;
class ItemManeger;

/// <summary>
/// Game�N���X����A���[�U���������𔲂��o�����N���X
/// </summary>
class GameMain
{
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
	IScene* m_pScene;


    // ���[�h��� 
    std::unique_ptr<ILoadingScreen> m_loadingScreen;

    PlayerMode m_playerMode;

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
};
