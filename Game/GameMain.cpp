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
/// プレイシーンの取得
/// </summary>
/// <returns>プレイシーンの生ポインタ</returns>
IScene* GameMain::GetPlayScene() 
{
	return m_playScene.get();
}

/// <summary>
/// タイトルシーンの取得
/// </summary>
/// <returns>タイトルシーンの生ポインタ</returns>
IScene* GameMain::GetTitleScene()
{
	return m_titleScene.get();
}

/// <summary>
/// リザルトシーンの取得
/// </summary>
/// <returns>リザルトシーンの生ポインタ</returns>
IScene* GameMain::GetResultScene()
{
	return m_resultScene.get();
}

//-------------------------------------------------------------------
// コンストラクタ
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
	m_keybord = std::make_unique<DirectX::Keyboard>();

	// マウス関連
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(pDR->GetHwnd());

	//プレイシーンの作成
	m_playScene = std::make_unique<PlayScene>(this);
	//タイトルシーンの作成
	m_titleScene = std::make_unique<TitleScene>(this);
	//リザルトシーンの作成
	m_resultScene = std::make_unique<ResultScene>(this);

	//タイトルシーンを変える
	SceneChange(m_titleScene.get());

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
		m_pScene->Update(timer);
	}

	if (m_titleScene != nullptr)
	{
		m_stageNum = m_titleScene->GetStageNum();
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
	
}

/// <summary>
/// シーンを変える
/// </summary>
/// <param name="scene">次のシーン</param>
void GameMain::SceneChange(IScene* scene)
{
	//実態があれば終了処理をする
	if(m_pScene!=nullptr)
		m_pScene->Finalize();

	//シーンを変更
	m_pScene = scene;

	//リーソスの読み込み
	LoadResources();
	
	m_pScene->SetStageNum(m_stageNum);

	//シーンの初期化
	m_pScene->Initialize();
	

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