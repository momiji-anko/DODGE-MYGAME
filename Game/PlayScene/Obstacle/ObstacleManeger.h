#pragma once

#include<map>
#include<vector>
#include"Obstacle.h"
#include<Model.h>
#include"../ShadowMap.h"

#include"../../Shader/FireEffectManager.h"
#include"Game/PlayScene/Stage/StageManager.h"

//ビヘイビアーインクルード
#include"ObstacelBhaviors/BirdBehavior.h"
#include"ObstacelBhaviors/MeanderingeBehavior.h"
#include"ObstacelBhaviors/MeteoriteBehavior.h"
#include"ObstacelBhaviors/NormalBehavior.h"
#include"ObstacelBhaviors/ReverseRotateStickBehavior.h"
#include"ObstacelBhaviors/RotateStickBehavior.h"
#include"ObstacelBhaviors/StickBehavior.h"

//スポナーインクルード
#include"ObstacleSpawners/BirdObstacleSpawner.h"
#include"ObstacleSpawners/MeanderingObstacleSpawner.h"
#include"ObstacleSpawners/MeteoriteObstacleSpawner.h"
#include"ObstacleSpawners/NormalObstacleSpawner.h"
#include"ObstacleSpawners/ReverseRotateStickObstacleSpawner.h"
#include"ObstacleSpawners/RotateStickObstacleSpawner.h"
#include"ObstacleSpawners/StickObstacleSpawner.h"


class Player;

class ObstacleManeger
{
public:
	static const int OBSTACLE_MAX_NUM;
	static const float SPANE_COOL_TIME_S;
	static const int EFFECT_MAX_NUM;
private:
	std::vector< std::unique_ptr<Actor>>                               m_obstacles;
	std::map< Obstacle::ObstacleType, std::unique_ptr<ISpawner>>       m_spawners;
	std::map< Obstacle::ObstacleType, std::unique_ptr<DirectX::Model>> m_models;
	std::map< Obstacle::ObstacleType, std::unique_ptr<IBehavior>>      m_behavior;
	DirectX::CommonStates*                                             m_commonState;
	
	std::map< int, DirectX::SimpleMath::Vector3>                       m_normalSpawnePosition;
	std::map< int, DirectX::SimpleMath::Vector3>                       m_stickSpawnePosition;
	std::map< int, DirectX::SimpleMath::Vector3>                       m_birdSpawnPosition;

	DirectX::SimpleMath::Vector3                                       m_playerPosition;

	std::vector<std::unique_ptr<FireEffectManager>>                       m_effectlist;

	DirectX::SimpleMath::Vector3                                       m_hitvel;
	DirectX::SimpleMath::Vector3                                       m_capsuleHitC1;
	DirectX::SimpleMath::Vector3                                       m_capsuleHitC2;

	float                                                              m_time_s;
	float                                                              m_spawneTime_s;
	float                                                              m_spawneCoolTime;


public:
	//コンストラクタ
	ObstacleManeger();
	//デストラクタ
	~ObstacleManeger();

	// 初期化
	void Initialize(DirectX::CommonStates* commonState, StageManeger::StageSelect stage );

	// 更新
	void Update(const DX::StepTimer& timer);

	// 描画
	void Draw(Camera* camera);

	// 終了処理
	void Finalize();

	void CreateSpawner();
	void CreateBehavior();

	void SetPlayerPosition(DirectX::SimpleMath::Vector3 position) { m_playerPosition = position; }

	bool PlayerHitCheck(AABBFor3D* playerAABB);

	void ObstacleShadow( ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	bool PlayerCapsuleHitCheck(Capsule* playerCapsule);
	bool PlayerCapsuleHitCheck(Player* player);

private:
	//アイテムの生成
	bool CreateObstacle(const DirectX::SimpleMath::Vector3& position, Obstacle::ObstacleType type, float angle);

	// クランプ関数
// n：丸め処理を行いたい数値
// min：丸め処理を行う最小値
// max：丸め処理を行う最大値
// 戻り値
// 丸め処理を行った結果となる数値
	float Clamp(float n, float min, float max)
	{
		return std::min(std::max(min, n), max);
	}


	// ２つの線分の最短距離の平方を返す関数
	// p1: 線分 1 の始点 q1: 線分 1 の終点
	// p2: 線分 2 の始点 q2: 線分 2 の終点
	// s: 線分１上の最短位置を表す係数 t: 線分 2 上の最短位置を表す係数
	// c1: 線分１上の最短距離の位置 c2: 線分 2 上の最短距離の位置
	// 返り値: ２つの線分の最短距離の平方
	float ClosestPtSegmentSegment(DirectX::SimpleMath::Vector3 p1, DirectX::SimpleMath::Vector3 q1,
		DirectX::SimpleMath::Vector3 p2, DirectX::SimpleMath::Vector3 q2, float& s, float& t,
		DirectX::SimpleMath::Vector3& c1, DirectX::SimpleMath::Vector3& c2)
	{
		// p1→q1 のベクトルを算出
		DirectX::SimpleMath::Vector3 d1 = q1 - p1;
		// p2→q2 のベクトルを算出
		DirectX::SimpleMath::Vector3 d2 = q2 - p2;
		// 二つのカプセルが持つ線分の、始点間のベクトルを半径の合計として算出
		DirectX::SimpleMath::Vector3 r = p1 - p2;
		// p1→q1 のベクトルの長さの 2 乗を算出
		float a = d1.Dot(d1);
		// p2→q2 のベクトルの長さの 2 乗を算出
		float e = d2.Dot(d2);
		// p1→q1 のベクトルと半径の合計となるベクトルを内積する
		float f = d2.Dot(r);
		if (a <= FLT_EPSILON && e <= FLT_EPSILON)
		{
			// a と e の両方がほぼ 0 だったら、二つのカプセルは平行である
			s = t = 0.0f;
			// 仮として各始点を獲得する
			c1 = p1;
			c2 = p2;
			// 仮取得した点同士の距離を求める
			return (c1 - c2).Dot(c1 - c2);
		}
		if (a <= FLT_EPSILON)
		{
			// a がほぼ 0 だった場合は、f/e を t として計算する
			// ただし、0～1 の間に補正する
			s = 0.0f;
			t = f / e;
			t = Clamp(t, 0.0f, 1.0f);
		}
		else
		{
			// p1→q1 のベクトルと半径の合計を内積する
			float c = d1.Dot(r);
			if (e <= FLT_EPSILON)
			{
				// e がほぼ 0 だった場合は、s として-c/a を計算する。
				// ただし、0～1 の間に補正する
				t = 0.0f;
				s = Clamp(-c / a, 0.0f, 1.0f);
			}
			else
			{
				// 特に 0 除算が起きる危険はないので、式通りの計算を行う
				// 各カプセルのベクトルで内積する
				float b = d1.Dot(d2);
				// それぞれのカプセルの長さの 2 乗を掛けた値と、
				// 各カプセルのベクトルで内積した結果の 2 乗を比べる
				float denom = a * e - b * b;
				if (denom != 0.0f)
				{
					// 長さが違っていれば、式通りの計算を行う
					s = Clamp((b * f - c * e) / denom, 0.0f, 1.0f);
				}
				else
				{
					// 二つのカプセルの向きがほぼ 90 度の場合、s を 0 とする
					s = 0.0f;
				}
				// b * s + f を計算して検証する
				float tnom = (b * s + f);
				if (tnom < 0.0f)
				{
					// マイナスの値になっていたら、-c / a の値を s とする
					t = 0.0f;
					s = Clamp(-c / a, 0.0f, 1.0f);
				}
				else if (tnom > e)
				{
					// e より大きければ、(b - c) / a の値を s とする
					t = 1.0f;
					s = Clamp((b - c) / a, 0.0f, 1.0f);
				}
				else
				{
					// 式通りの計算を行う
					t = tnom / e;
				}
			}
		}
		// それぞれのカプセルにおける、一番近い点を算出する
		c1 = p1 + d1 * s;
		c2 = p2 + d2 * t;

		m_hitvel = (c1 - c2);
		
		m_capsuleHitC1 = c1;
		m_capsuleHitC2 = c2;

		// 一番近い二点の距離の二乗を返す
		return (c1 - c2).Dot(c1 - c2);
	}


	// カプセルとカプセルの衝突判定関数
	// capsule1：当たり判定を取りたいカプセル 1 つ目
	// capsule2：当たり判定を取りたいカプセル 2 つ目
	bool HitCheck_Capsule2Capsule(Capsule capsule1, Capsule capsule2)
	{
		float s, t;
		DirectX::SimpleMath::Vector3 c1, c2;
		// カプセルの中心の線分間の距離の平方を計算
		// s, t, c1, c2 は戻り値
		float dist2 = ClosestPtSegmentSegment(capsule1.a, capsule1.b, capsule2.a, capsule2.b, s, t, c1, c2);
		float radius = capsule1.r + capsule2.r;
		return dist2 <= radius * radius;
	}



};