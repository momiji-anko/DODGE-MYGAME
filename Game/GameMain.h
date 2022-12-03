//
// GameMain.h
//
#pragma once

#include <Keyboard.h>
#include <Mouse.h>
#include "StepTimer.h"
#include"ILoadingScreen.h"



// シーンの列挙
enum class GAME_SCENE : int
{
    NONE,

    TITLE,
    PLAY,
    RESULT,
};

//ステージ
enum class StageSelect
{
    Stage1,
    Stage2,
    Stage3,
};

// 前方宣言
class DebugCamera;

class IScene;

class StageManeger;
class Player;
class ObstacleManeger;
class ItemManeger;

/// <summary>
/// Gameクラスから、ユーザ処理部分を抜き出したクラス
/// </summary>
class GameMain
{
private:

    // キーボード
    std::unique_ptr<DirectX::Keyboard> m_keybord;
    
    // マウス
    std::unique_ptr<DirectX::Mouse> m_mouse;

	// 次のシーン番号
	GAME_SCENE m_nextScene;

	// シーン
	IScene* m_pScene;

    float m_time;

    // ロード画面 
    std::unique_ptr<ILoadingScreen> m_loadingScreen;

    std::unique_ptr<StageManeger> m_stageManeger;
    std::unique_ptr<Player> m_player;
    std::unique_ptr<ObstacleManeger> m_obstacleManeger;
    std::unique_ptr<ItemManeger> m_itemManeger;

public:
    GameMain();
    ~GameMain();

    void Initialize();
    void Update(const DX::StepTimer& timer);
    void Render();
    void Finalize();

	// シーン関係処理

	// シーンの作成
	void CreateScene();

	// シーンの削除
	void DeleteScene();

private:
    void LoadResources(bool useLoadingScreen = true);
};
