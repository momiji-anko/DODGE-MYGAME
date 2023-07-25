/*
プレイシーン
*/
#pragma once
#include<Keyboard.h>
#include <CommonStates.h>
#include <Model.h>
#include <GeometricPrimitive.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

#include "../IScene.h"
#include "Game/GameMain.h"
#include"Player/Player.h"
#include"Player/PlayerManager.h"
#include"Actor/Actor.h"
#include"Obstacle/ObstacleManager.h"
#include"Stage/Stage.h"
#include"Item/ItemManager.h"
#include"ShadowMap.h"
#include"Stage/StageManager.h"
#include"AliveTimer.h"

#include"Libraries/AXD2/Adx2.h"
#include"../../Helpers/CueSheet_0.h"
#include"../../Helpers/DODGESound_acf.h"
#include"Objects.h"
#include"Game/Shader/Fade.h"

class GridFloor;
class Camera;
class GameMain;

/// <summary>
/// プレイシーン
/// </summary>
class PlayScene : public IScene
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayScene(GameMain* parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	void Update(const DX::StepTimer& timer) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// リソース読み込み
	/// </summary>
	void LoadResources() override;


	/// <summary>
	/// ステージ番号
	/// </summary>
	/// <param name="stageNum">ステージ番号</param>
	void SetStageNum(int stageNum)override { m_stageNum = static_cast<StageManager::StageSelect>(stageNum); }

private:

	/// <summary>
	/// カウントダウンの表示
	/// </summary>
	void DrawCountDown();

	/// <summary>
	/// 影生成
	/// </summary>
	void CreateShadow();
private:
	//カウントダウンタイム
	static const float COUNT_DOWN_TIME_S;

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	//天球
	DirectX::Model* m_pModel;

	//障害物マネージャー
	std::unique_ptr<ObstacleManager>                 m_obstacleManager;
	//アイテムマネージャー
	std::unique_ptr<ItemManager>                     m_itemManager;
	//ステージマネージャー
	std::unique_ptr<StageManager>                    m_stageManager;

	//カメラ
	std::unique_ptr<Camera>                          m_pCamera;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch>            m_spriteBatch;
	//カウントダウンテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_countDownTexture;
	//カウントダウン
	float                                            m_countDownTime;

	//キーボードステートトラッカー
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardStateTracker;

	//シャドウマップ
	std::unique_ptr<ShadowMap>                       m_shadowMap;

	//音関連
	ADX2* m_pAdx2;
	//BGMのID
	int  m_musicID;

	//フェード
	std::unique_ptr<Fade> m_fadeInOut;

	//アライブタイム
	AliveTimer* m_aliveTime;
	//プレイヤーマネージャー
	std::unique_ptr<PlayerManager> m_playerManager;
	//プレイヤーモード
	GameMain::PlayerMode m_playerMode;
	//ステージ
	StageManager::StageSelect m_stageNum;

	//プレイヤーの操作方法画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_playerMoveKey;
	bool m_isTabKey;

	GameMain* m_parent;
};