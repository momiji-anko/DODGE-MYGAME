/*
* 2023/04/15
* ObstacleManeger.cpp
* 障害物管理クラス
* 麻生　楓
*/
#include"pch.h"
#include"ObstacleManager.h"
#include<Effects.h>
#include"DeviceResources.h"
#include"../MyRandom.h"
#include"../Player/Player.h"
#include"../../GameMain.h"
#include"Libraries/MyLibraries/ModelManager.h"

const int ObstacleManeger::OBSTACLE_MAX_NUM = 100;
const int ObstacleManeger::EFFECT_MAX_NUM = 5;
const float ObstacleManeger::SPAWN_COOL_TIME_S = 3.0f;

//コンストラクタ
ObstacleManeger::ObstacleManeger() 
	:
	m_obstacles{},
	m_spawners{},
	m_models{},
	m_commonState(nullptr),
	m_normalSpawnePosition{ 
		{DirectX::SimpleMath::Vector3(19,0.0f,-19) },
		{DirectX::SimpleMath::Vector3(19,0.0f,19) },
		{DirectX::SimpleMath::Vector3(-19,0.0f,19) },
		{DirectX::SimpleMath::Vector3(-19,0.0f,-19) },
	},
	m_stickSpawnePosition{ 
		{DirectX::SimpleMath::Vector3(19.0f,0.0f,0.0f) },
		{DirectX::SimpleMath::Vector3(0.0f,0.0f,-19.0f) },
		{DirectX::SimpleMath::Vector3(-19.0f,0.0f,0.0f) },
		{DirectX::SimpleMath::Vector3(0.0f,0.0f,19) },
	},
	m_birdSpawnPosition{
		{DirectX::SimpleMath::Vector3(19.0f ,  3.0f ,0.0f) },
		{DirectX::SimpleMath::Vector3(0.0f  ,  3.0f ,-19.0f) },
		{DirectX::SimpleMath::Vector3(-19.0f,  3.0f ,0.0f) },
		{DirectX::SimpleMath::Vector3(0.0f  ,  3.0f ,19.0f) },
	},
	m_hitvel{0,0,0},
	m_time_s(0.0f),
	m_spawneCoolTime_s(0.0f),
	m_spawneTime_s(0.0f)
{
}

//デストラクタ
ObstacleManeger::~ObstacleManeger()
{
	
}

// 初期化
void ObstacleManeger::Initialize(DirectX::CommonStates* commonState, StageManager::StageSelect stage)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	m_commonState = commonState;

	m_effectlist.resize(EFFECT_MAX_NUM);



	for (int i = 0; i < m_effectlist.size(); i++)
	{
		m_effectlist[i] = std::make_unique<FireEffectManager>();
		m_effectlist[i]->Create();
		m_effectlist[i]->Initialize(2.0f, DirectX::SimpleMath::Vector3::Zero);
	}

	m_obstacles.resize(OBSTACLE_MAX_NUM);

	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		obstacle = std::make_unique<Obstacle>();

		Obstacle* o = dynamic_cast<Obstacle*>(obstacle.get());
		if (o != nullptr)
		{
			int num = MyRandom::GetIntRange(0, EFFECT_MAX_NUM - 1);
			o->SetEffect(m_effectlist[num].get());
			
		}
	}
	//障害物スポナ作成
	CreateSpawner();
	//障害物ビヘイビアー作成
	CreateBehavior();
	

	m_spawneCoolTime_s = SPAWN_COOL_TIME_S;
	m_spawneTime_s = SPAWN_COOL_TIME_S;

	//障害物モデル作成
	CreateModel();
	
	//Stage1とStage2であれば回転する棒を生成する
	if (stage == StageManager::StageSelect::Stage1 || stage == StageManager::StageSelect::Stage2)
	{
		CreateObstacle(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), Obstacle::ObstacleType::ROTATESTICK, DirectX::SimpleMath::Vector3::Zero);
		CreateObstacle(DirectX::SimpleMath::Vector3(0.0f, 3.0f, 0.0f), Obstacle::ObstacleType::REVERSE_ROTATESTICK, DirectX::SimpleMath::Vector3::Zero);
	}

}

// 更新
void ObstacleManeger::Update(const DX::StepTimer& timer)
{
	float time = timer.GetElapsedSeconds();
	m_time_s += time;
	m_spawneTime_s -= time;
	if (m_spawneTime_s <= 0.0f)
	{
		m_spawneTime_s = m_spawneCoolTime_s;
		int type = MyRandom::GetIntRange(0, 3);
		DirectX::SimpleMath::Vector3 rot = DirectX::SimpleMath::Vector3::Zero;
		
		


		rot.y = (DirectX::XM_PI / 2.0f) * type;
		if (m_time_s <= 50)
		{
			int createObs = MyRandom::GetIntRange(0, 5);

			if(createObs <= 4)
			{
				rot.y = atan2(m_normalSpawnePosition[type].x - m_playerPosition.x, m_normalSpawnePosition[type].z - m_playerPosition.z);
				CreateObstacle(m_normalSpawnePosition[type], Obstacle::ObstacleType::NORMAL, rot);
				


			}
			else if (createObs == 5)
			{
				
				CreateObstacle(m_stickSpawnePosition[type], Obstacle::ObstacleType::STICK, rot);
			}

			
		}
		else 
		{
			int random = MyRandom::GetIntRange(0, 100);

			if (random <= 30)
			{
				rot.y = atan2(m_normalSpawnePosition[type].x - m_playerPosition.x, m_normalSpawnePosition[type].z - m_playerPosition.z);
				CreateObstacle(m_normalSpawnePosition[type], Obstacle::ObstacleType::NORMAL, rot);

			}
			else if (random <= 45)
			{
				

				CreateObstacle(m_stickSpawnePosition[type], Obstacle::ObstacleType::STICK, rot);
			}
			else if (random <= 50)
			{
				rot.y = ((DirectX::XM_PI / 2.0f) * type);
				CreateObstacle(m_birdSpawnPosition[type], Obstacle::ObstacleType::BIRD, rot);
			
			}else if (random <= 100)
			{

				CreateObstacle(m_normalSpawnePosition[type], Obstacle::ObstacleType::MEANDERING, rot);
			}

		}
		

		
		m_spawneCoolTime_s -= 0.003f;

		
	}
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		Obstacle* obs = dynamic_cast<Obstacle*> (obstacle.get());

		obs->SetPlayerPosition(m_playerPosition);
	}

	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		if (obstacle->IsActive() == false)
			continue;

		obstacle->Update(timer);
	}
}

// 描画
void ObstacleManeger::Draw(Camera* camera)
{
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		if (obstacle->IsActive() == false)
			continue;

		obstacle->Draw(camera);

		

	}

}

// 終了処理
void ObstacleManeger::Finalize()
{

}

void ObstacleManeger::CreateSpawner()
{

	m_spawners[Obstacle::ObstacleType::NORMAL] = std::make_unique<NormalObstacleSpawner>();
	m_spawners[Obstacle::ObstacleType::STICK] = std::make_unique<StickObstacleSpawner>();
	m_spawners[Obstacle::ObstacleType::METEORITE] = std::make_unique<MeteoriteObstacleSpawner>();
	m_spawners[Obstacle::ObstacleType::ROTATESTICK] = std::make_unique<RotateStickObstacleSpawner>();
	m_spawners[Obstacle::ObstacleType::REVERSE_ROTATESTICK] = std::make_unique<ReverseRotateStickObstacleSpawner>();
	m_spawners[Obstacle::ObstacleType::MEANDERING] = std::make_unique<MeanderingObstacleSpawner>();
	m_spawners[Obstacle::ObstacleType::BIRD] = std::make_unique<BirdObstacleSpawner>();
}

void ObstacleManeger::CreateBehavior()
{
	m_behavior[Obstacle::ObstacleType::NORMAL] = std::make_unique<NormalBehavior>();
	m_behavior[Obstacle::ObstacleType::STICK] = std::make_unique<StickBehavior>();
	m_behavior[Obstacle::ObstacleType::METEORITE] = std::make_unique<MeteoriteBehavior>();
	m_behavior[Obstacle::ObstacleType::ROTATESTICK] = std::make_unique<RotateStickBehavior>();
	m_behavior[Obstacle::ObstacleType::REVERSE_ROTATESTICK] = std::make_unique<ReverseRotateStickBehavior>();
	m_behavior[Obstacle::ObstacleType::MEANDERING] = std::make_unique<MeanderingBehavior>();
	m_behavior[Obstacle::ObstacleType::BIRD] = std::make_unique<BirdBehavior>();

}

bool ObstacleManeger::PlayerHitCheck(AABBFor3D* playerAABB)
{
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		if (obstacle->IsActive() == false)
			continue;

		if (obstacle->GetAABB()->HitCheck(playerAABB))
		{
			return true;
		}
		
	}

	return false;
}

void ObstacleManeger::ObstacleShadow( ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		if (obstacle->IsActive() == false)
			continue;

		Obstacle* o = dynamic_cast<Obstacle*> (obstacle.get());
		if (o != nullptr)
		{
			o->ObstacleShadow(shadowMap, view, projection);
		}

	}

}


bool ObstacleManeger::PlayerCapsuleHitCheck(Player* player)
{
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		if (obstacle->IsActive() == false)
			continue;

		Capsule* cap = obstacle->GetCapsule();

		if (HitCheck_Capsule2Capsule(*cap, *player->GetCapsule()))
		{
			DirectX::SimpleMath::Vector3 rot = obstacle->GetRotation().ToEuler();
			Obstacle* ob = dynamic_cast<Obstacle*>(obstacle.get());

			bool isRoteStick = ob->GetObstacleType() == Obstacle::ObstacleType::ROTATESTICK;

			if (ob != nullptr && isRoteStick)
			{
				
				m_hitvel = m_capsuleHitC2 - m_capsuleHitC1;

				m_hitvel.Normalize();

				float playerR = player->GetCapsule()->r;
				float obstacleR = cap->r;
				DirectX::SimpleMath::Vector3 vec = m_capsuleHitC1 - m_capsuleHitC2;
				float len = vec.Length();
				if (len < playerR + obstacleR)
				{
					float a = len - (playerR + obstacleR);
					vec.Normalize();
					vec *= a;
					player->SetPosition(player->GetPosition() + vec);
				}
				float dot = 0;;

				if (ob->GetObstacleType() == Obstacle::ObstacleType::REVERSE_ROTATESTICK)
				{
					dot = m_hitvel.Dot(DirectX::SimpleMath::Vector3(cos(rot.y + (-90)), 0, -sin(rot.y + (-90))));
				}
				else
				{
					dot = m_hitvel.Dot(DirectX::SimpleMath::Vector3(cos(rot.y + (90)), 0, -sin(rot.y + (90))));
				}
				 
				float speed = ob->GetRotSpeed();

				if (speed <	 0)
				{
					speed * -1;
				}

				if (dot > 0.0f)
				{
					player->SetFlyVelocity(DirectX::SimpleMath::Vector3(m_hitvel) * (0.5f + (speed * 2.0f)));

				}
			}

			if (ob != nullptr && ob->GetObstacleType() == Obstacle::ObstacleType::REVERSE_ROTATESTICK)
			{
				m_hitvel = m_capsuleHitC2 + m_capsuleHitC1;

				m_hitvel.Normalize();
				m_hitvel *= cap->r;

				DirectX::SimpleMath::Vector3 targetVector;
				DirectX::SimpleMath::Vector3 playerVector;
				targetVector = m_hitvel + player->GetCapsule()->b;
				playerVector = player->GetCapsule()->a + player->GetCapsule()->b;

				targetVector.Normalize();
				playerVector.Normalize();
				
				float dot = targetVector.Dot(playerVector);
				float sita = acos(dot);
				if (sita < 0)
				{
					sita *= -1.0f;
				}

				if (sita >= 0.0877f)
				{
					 DirectX::SimpleMath::Vector3 playerVec = player->GetVelocity();
					 player->SetVelocity(DirectX::SimpleMath::Vector3(playerVec.x, -0.1f, playerVec.z));
				}
				else 
				{
					m_hitvel = m_capsuleHitC2 - m_capsuleHitC1;

					m_hitvel.Normalize();

					float speed = ob->GetRotSpeed();
					if (speed < 0)
					{
						speed *= -1;
					}

					player->SetFlyVelocity(DirectX::SimpleMath::Vector3(m_hitvel) * (0.5f + (speed * 2.0f)));
				}

				float playerR = player->GetCapsule()->r;
				float obstacleR = cap->r;
				DirectX::SimpleMath::Vector3 vec = m_capsuleHitC1 - m_capsuleHitC2;
				float length = vec.Length();
				if (length < playerR + obstacleR)
				{
					float a = length - (playerR + obstacleR);
					vec.Normalize();
					vec *= a;
					player->SetPosition(player->GetPosition() + vec);
				}

			}
			return true;
		}

	}
	return false;
}


/// <summary>
/// 障害物の生成
/// </summary>
/// <param name="position">生成座標</param>
/// <param name="type">障害物のタイプ</param>
/// <param name="rot">アングル</param>
/// <returns>true = 生成成功 , false = 生成失敗</returns>
bool ObstacleManeger::CreateObstacle(const DirectX::SimpleMath::Vector3& position, Obstacle::ObstacleType type, DirectX::SimpleMath::Vector3 rot)
{
	return m_spawners[type]->Create(m_obstacles, position, rot, m_behavior[type].get(), m_models[type], m_commonState);
}

/// <summary>
/// 障害物のモデル生成
/// 障害物のモデル生成
/// </summary>
void ObstacleManeger::CreateModel()
{
	//モデルマネージャー取得
	ModelManager& modelManager = ModelManager::GetInstance();

	//	隕石モデル
	m_models[Obstacle::ObstacleType::METEORITE] = modelManager.LoadModel(L"Resources/Models/star.cmo");

	//	棒モデル
	m_models[Obstacle::ObstacleType::STICK] = modelManager.LoadModel(L"Resources/Models/stickObstale.cmo"); 
	
	//　回る棒モデル
	m_models[Obstacle::ObstacleType::ROTATESTICK] = modelManager.LoadModel(L"Resources/Models/roll.cmo"); 

	//	鳥モデル
	m_models[Obstacle::ObstacleType::BIRD] = modelManager.LoadModel(L"Resources/Models/bule.cmo"); 

	//	反対に回る棒モデル
	m_models[Obstacle::ObstacleType::REVERSE_ROTATESTICK] = modelManager.LoadModel(L"Resources/Models/roll2.cmo"); 
}

/// <summary>
/// クランプ関数
/// </summary>
/// <param name="n">丸め処理を行いたい数値</param>
/// <param name="min">丸め処理を行う最小値</param>
/// <param name="max">丸め処理を行う最大値</param>
/// <returns>丸め処理を行った結果となる数値</returns>
float ObstacleManeger::Clamp(float n, float min, float max)
{
	return std::min(std::max(min, n), max);
}

/// <summary>
/// ２つの線分の最短距離の平方を返す関数
/// </summary>
/// <param name="p1">線分 1 の始点</param>
/// <param name="q1">線分 1 の終点</param>
/// <param name="p2">線分 2 の始点</param>
/// <param name="q2">線分 2 の終点</param>
/// <param name="s">線分１上の最短位置を表す係数</param>
/// <param name="t">線分 2 上の最短位置を表す係数</param>
/// <param name="c1">線分１上の最短距離の位置 </param>
/// <param name="c2">線分 2 上の最短距離の位置</param>
/// <returns>２つの線分の最短距離の平方</returns>
float ObstacleManeger::ClosestPtSegmentSegment(DirectX::SimpleMath::Vector3 p1, DirectX::SimpleMath::Vector3 q1, DirectX::SimpleMath::Vector3 p2, DirectX::SimpleMath::Vector3 q2, float& s, float& t, DirectX::SimpleMath::Vector3& c1, DirectX::SimpleMath::Vector3& c2)
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

/// <summary>
/// カプセルとカプセルの衝突判定関数
/// </summary>
/// <param name="capsule1">当たり判定を取りたいカプセル 1 つ目</param>
/// <param name="capsule2">当たり判定を取りたいカプセル 2 つ目</param>
/// <returns>true= 当たった、false=当たってない</returns>
bool ObstacleManeger::HitCheck_Capsule2Capsule(Capsule capsule1, Capsule capsule2)
{
	float s, t;
	DirectX::SimpleMath::Vector3 c1, c2;
	// カプセルの中心の線分間の距離の平方を計算
	// s, t, c1, c2 は戻り値
	float dist2 = ClosestPtSegmentSegment(capsule1.a, capsule1.b, capsule2.a, capsule2.b, s, t, c1, c2);
	float radius = capsule1.r + capsule2.r;
	return dist2 <= radius * radius;
}
