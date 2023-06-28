/*
* 2023/06/12
* PlayerManagerh
* プレイヤーマネージャー
* 麻生　楓
*/
#pragma once
#include"Player.h"
#include"Game/PlayScene/Obstacle/ObstacleManager.h"
#include"../Item/ItemManager.h"
#include"../Stage/StageManager.h"


/// <summary>
/// プレイヤーマネージャー
/// </summary>
class PlayerManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="playerMode">プレイヤーモード</param>
	/// <param name="stageManager">ステージマネージャー</param>
	/// <param name="itemManager">アイテムマネージャー</param>
	/// <param name="obstacleManager">障害物マネージャー</param>
	PlayerManager(GameMain::PlayerMode playerMode,StageManager* stageManager, ItemManager* itemManager, ObstacleManager* obstacleManager);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	void Update(const DX::StepTimer& timer);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラの生ポインタ</param>
	void Draw(Camera* camera);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// １ｐか２ｐのどちらかの座標を取得
	/// </summary>
	/// <returns>プレイヤー座標</returns>
	DirectX::SimpleMath::Vector3 GetRandomPlayerPosition();

	/// <summary>
	/// プレイヤーが全員死亡している確認
	/// </summary>
	/// <returns>true=全員死亡,false=全員死亡していない</returns>
	bool GetPlayerIsAllDaed();

	/// <summary>
	/// プレイヤーの影作成
	/// </summary>
	/// <param name="shadowMap">シャドウマップの生ポインタ</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="projection">プロジェクション行列</param>
	void PlayerShadow(ShadowMap* shadowMap, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);

private:
	/// <summary>
	/// プレイヤー作成
	/// </summary>
	void CreatePlayer();


	/// <summary>
	/// プレイヤーのめり込み処理
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="cupseleToCupseVector">当たったカプセルとカプセルのベクトル（プレイヤーカプセルから障害物カプセルのベクトル）</param>
	/// <param name="playerCapsleRadius">プレイヤーのカプセルの半径</param>
	/// <param name="obstacleCupsleRadius">プレイヤー２のカプセルの半径</param>
	void PlayerCapuslePenetration(Actor* player, const DirectX::SimpleMath::Vector3& cupseleToCupseVector, float playerCapsleRadius, float obstacleCupsleRadius);

	/// <summary>
	/// タイマーを止めるか確認する
	/// </summary>
	void TimerStopCheck();

private:
	//プレイヤー
	std::vector<std::unique_ptr<Player>> m_players;

	//プレイヤーモード
	GameMain::PlayerMode m_playerMode;
	
	//ステージマネージャー
	StageManager*    m_stageManager;
	//アイテムマネージャー
	ItemManager*     m_itemManager;
	//障害物マネージャー
	ObstacleManager* m_obstacleManager;

};