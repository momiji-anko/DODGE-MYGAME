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

// 名前空間の利用
using namespace DirectX;

//-------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------
GameMain::GameMain()
	: m_nextScene(GAME_SCENE::TITLE)		// 初期シーンの設定
	, m_pScene(nullptr)
	,m_playerMode(GameMain::PlayerMode::NONE)
	, m_stageNum(0)
{
}

//-------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------
GameMain::~GameMain()
{
	Finalize();
}

//-------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------
void GameMain::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// キーボード関連
	m_keybord             = std::make_unique<DirectX::Keyboard>();

	// マウス関連
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(pDR->GetHwnd());

	// シーン作成
	CreateScene();
}

//-------------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------------
void GameMain::Update(const DX::StepTimer& timer)
{
	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = m_keybord->GetState();

	// ESCキーで終了
	if (keyState.Escape)
	{
		PostQuitMessage(0);
	}

	// 次のシーンが設定されていたらシーン切り替え
	if (m_nextScene != GAME_SCENE::NONE)
	{
		// シーン削除
		DeleteScene();
		
		// シーン作成
		CreateScene();
	}
	// ロード画面の実体があれば更新
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

	// 実態があれば更新
	if (m_pScene != nullptr)
	{
		m_nextScene = m_pScene->Update(timer);
	}
}

//-------------------------------------------------------------------
// 描画処理
//-------------------------------------------------------------------
void GameMain::Render()
{

	// ロード画面の実体があれば描画
	if (m_loadingScreen != nullptr)
	{
		m_loadingScreen->Draw();
		return;
	}
	// 実態があれば描画
	if (m_pScene != nullptr)
	{
		m_pScene->Draw();
	}
}

//-------------------------------------------------------------------
// 後始末
//-------------------------------------------------------------------
void GameMain::Finalize()
{
	DeleteScene();
}

/*--------------------------------------------------
シーンの作成
--------------------------------------------------*/
void GameMain::CreateScene()
{
	// シーンが作成されているときは処理しない
	if (m_pScene != nullptr)
	{
		return;
	}

	// 次シーンの作成
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


	

		break;
	}
	default:
	{
		// 例外なので処理を中断
		return;
	}
	}
	// リソースの読み込み
	LoadResources(true);

	// 作成したシーンを初期化
	m_pScene->Initialize();



	// シーンを切り替えたのでリセット
	m_nextScene = GAME_SCENE::NONE;
}

/*--------------------------------------------------
シーンの削除
--------------------------------------------------*/
void GameMain::DeleteScene()
{
	// シーンが作成されていなければ処理しない
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

	// 現シーンの終了処理
	m_pScene->Finalize();

	// 現シーンの削除
	delete m_pScene;
	m_pScene = nullptr;
}
/*--------------------------------------------------
リソースの読み込み
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