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

// シーンの列挙
enum class GAME_SCENE : int
{
	NONE,

	TITLE,
	PLAY,
	RESULT,
};




// 前方宣言
class DebugCamera;
class IScene;

/// <summary>
/// Gameクラスから、ユーザ処理部分を抜き出したクラス
/// </summary>
class GameMain
{
public:
	/// <summary>
	/// プレイシーンの取得
	/// </summary>
	/// <returns>プレイシーンの生ポインタ</returns>
	IScene* GetPlayScene();

	/// <summary>
	/// タイトルシーンの取得
	/// </summary>
	/// <returns>タイトルシーンの生ポインタ</returns>
	IScene* GetTitleScene();

	/// <summary>
	/// リザルトシーンの取得
	/// </summary>
	/// <returns>リザルトシーンの生ポインタ</returns>
	IScene* GetResultScene();

public:
	GameMain();
	~GameMain();

	void Initialize();
	void Update(const DX::StepTimer& timer);
	void Render();
	void Finalize();

	// シーン関係処理

	/// <summary>
	/// シーンを変える
	/// </summary>
	/// <param name="scene">次のシーン</param>
	void SceneChange(IScene* scene);


private:

	void LoadResources(bool useLoadingScreen = true);

public:

	//プレイヤーのモード（数）
	enum class PlayerMode
	{
		NONE = 0,
		Player1 = 1,
		Player2 = 2
	};

private:

	// キーボード
	std::unique_ptr<DirectX::Keyboard> m_keybord;

	// マウス
	std::unique_ptr<DirectX::Mouse> m_mouse;


	// シーン
	IScene* m_pScene;
	//プレイシーン
	std::unique_ptr<PlayScene>   m_playScene;
	//タイトルシーン
	std::unique_ptr<TitleScene>  m_titleScene;
	//リザルトシーン
	std::unique_ptr<ResultScene> m_resultScene;
		

	// ロード画面 
	std::unique_ptr<ILoadingScreen> m_loadingScreen;

	int m_stageNum;

};
