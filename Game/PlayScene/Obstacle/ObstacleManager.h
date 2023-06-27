/*
* 2023/04/11
* ObstacleManeger.h
* 障害物管理クラス
* 麻生　楓
*/
#pragma once

#include<map>
#include<vector>
#include"Obstacle.h"
#include<Model.h>
#include"../ShadowMap.h"

#include"../../Shader/FireEffectManager.h"
#include"Game/PlayScene/Stage/StageManager.h"

//ビヘイビアーインクルード
#include"ObstacelBehaviors/BirdBehavior.h"
#include"ObstacelBehaviors/MeanderingeBehavior.h"
#include"ObstacelBehaviors/MeteoriteBehavior.h"
#include"ObstacelBehaviors/NormalBehavior.h"
#include"ObstacelBehaviors/ReverseRotateStickBehavior.h"
#include"ObstacelBehaviors/RotateStickBehavior.h"
#include"ObstacelBehaviors/StickBehavior.h"

//スポナーインクルード
#include"ObstacleSpawners/BirdObstacleSpawner.h"
#include"ObstacleSpawners/MeanderingObstacleSpawner.h"
#include"ObstacleSpawners/MeteoriteObstacleSpawner.h"
#include"ObstacleSpawners/NormalObstacleSpawner.h"
#include"ObstacleSpawners/ReverseRotateStickObstacleSpawner.h"
#include"ObstacleSpawners/RotateStickObstacleSpawner.h"
#include"ObstacleSpawners/StickObstacleSpawner.h"

//前方宣言
class Player;

/// <summary>
/// 障害物管理クラス
/// </summary>
class ObstacleManager
{
public:
	/// <summary>
	/// プレイヤーの座標設定
	/// </summary>
	/// <param name="position">プレイヤーの座標</param>
	void SetPlayerPosition(const DirectX::SimpleMath::Vector3& position);

	/// <summary>
	/// プレイヤーと当たった障害物のタイプ
	/// </summary>
	/// <returns>当たった障害物のタイプ</returns>
	Obstacle::ObstacleType GetHitType() { return m_hitType; }

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ObstacleManager();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ObstacleManager();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="stage">ステージの番号</param>
	void Initialize(StageManager::StageSelect stage );

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
	/// スポナー作成
	/// </summary>
	void CreateSpawner();
	
	/// <summary>
	/// ビヘイビアー作成
	/// </summary>
	void CreateBehavior();
	
	/// <summary>
	/// プレイヤーと障害物のAABBの当たり判定
	/// </summary>
	/// <param name="playerAABB">プレイヤーのAABB</param>
	/// <returns>true = 当たった , false = 当っていない</returns>
	bool PlayerHitCheck(AABBFor3D* playerAABB);

	/// <summary>
	/// 障害物の影作成
	/// </summary>
	/// <param name="shadowMap">シャドウマップの生ポインタ</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="projection">プロジェクション行列</param>
	void ObstacleShadow( ShadowMap* shadowMap, const DirectX::SimpleMath::Matrix& view,const DirectX::SimpleMath::Matrix& projection);

	
	
	
	/// <summary>
	/// プレイヤーと障害物のカプセルの当たり判定
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="flyVelocity">プレイヤーを吹き飛ばすベロシティ</param>
	/// <returns>true = 当たった , false = 当っていない</returns>
	bool PlayerCapsuleHitCheck(Actor* player, DirectX::SimpleMath::Vector3* flyVelocity);

private:
	/// <summary>
	/// 障害物の生成
	/// </summary>
	/// <param name="position">出現座標</param>
	/// <param name="type">障害のタイプ</param>
	/// <param name="rot">角度</param>
	/// <returns>true = 生成成功 , false = 生成失敗</returns>
	bool CreateObstacle(const DirectX::SimpleMath::Vector3& position, Obstacle::ObstacleType type, const DirectX::SimpleMath::Vector3& rot);

	/// <summary>
	/// 障害物のモデル作成
	/// </summary>
	void CreateModel();
	


	/// <summary>
	/// プレイヤーを吹き飛ばす	
	/// </summary>
	/// <param name="rotSpeed">障害物の回転速度</param>
	/// <param name="player">プレイヤー</param>
	DirectX::SimpleMath::Vector3 FlyPlayer(float rotSpeed);
	
	/// <summary>
	/// プレイヤーのめり込み処理
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="cupseleToCupseVector">当たったカプセルとカプセルのベクトル（プレイヤーカプセルから障害物カプセルのベクトル）</param>
	/// <param name="playerCapsleRadius">プレイヤーのカプセルの半径</param>
	/// <param name="obstacleCupsleRadius">障害物のカプセルの半径</param>
	void PlayerCapuslePenetration(Actor* player,const DirectX::SimpleMath::Vector3& cupseleToCupseVector, float playerCapsleRadius, float obstacleCupsleRadius);

	/// <summary>
	/// 回転する棒に当たった時の処理
	/// </summary>
	/// <param name="obstacle">回転する棒の障害物</param>
	/// <param name="player">プレイヤー</param>
	DirectX::SimpleMath::Vector3 RotaStickHit(Actor* obstacle, Actor* player);

	/// <summary>
	/// 反対回転する棒に当たった時の処理
	/// </summary>
	/// <param name="obstacle">回転する棒の障害物</param>
	/// <param name="player">プレイヤー</param>	
	DirectX::SimpleMath::Vector3 ReverseRotaStickHit(Actor* obstacle, Actor* player);

public:
	//障害物最大数
	static const int OBSTACLE_MAX_NUM;
	//スポーンクールタイム
	static const float SPAWN_COOL_TIME_S;
	//炎エフェクトの最大数
	static const int EFFECT_MAX_NUM;
	//スポーンクールタイムの減る数
	static const float SPAWNE_COOL_TIME_DECREASE_AMOUNT;

private:
	//障害物
	std::vector< std::unique_ptr<Actor>>                         m_obstacles;
	//障害物のスポナー
	std::map< Obstacle::ObstacleType, std::unique_ptr<ISpawner>> m_spawners;
	//障害物のモデル
	std::map< Obstacle::ObstacleType, DirectX::Model*>           m_models;
	//障害物のビヘイビアー
	std::map< Obstacle::ObstacleType, std::unique_ptr<IBehavior>>m_behavior;
	//炎の障害物の出現位置
	std::vector<DirectX::SimpleMath::Vector3>                    m_normalSpawnePosition;
	//棒の障害物の出現位置
	std::vector<DirectX::SimpleMath::Vector3>                    m_stickSpawnePosition;
	//鳥の障害物の出現位置
	std::vector<DirectX::SimpleMath::Vector3>                    m_birdSpawnPosition;
	//プレイヤーのポジション
	DirectX::SimpleMath::Vector3                                 m_playerPosition;

	//炎エフェクトの配列
	std::vector<std::unique_ptr<FireEffectManager>>              m_effectlist;

	//プレイヤーを回転する棒に当たった時のプレイヤーから回転する棒の距離
	DirectX::SimpleMath::Vector3                                 m_hitvel;
	//当たったカプセルの場所（障害物）
	DirectX::SimpleMath::Vector3                                 m_capsuleHitC1;
	//当たったカプセルの場所（プレイヤー）
	DirectX::SimpleMath::Vector3                                 m_capsuleHitC2;

	//タイマー
	float                                                        m_time_s;
	//障害物のスポーンタイマー
	float                                                        m_spawneTime_s;
	//障害物のスポーンクールタイム
	float                                                        m_spawneCoolTime_s;

	//当たった障害物のタイプ
	Obstacle::ObstacleType m_hitType;

	
};