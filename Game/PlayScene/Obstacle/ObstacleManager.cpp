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
		CreateObstacle(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), Obstacle::ObstacleType::ROTATESTICK, 0);
		CreateObstacle(DirectX::SimpleMath::Vector3(0.0f, 3.0f, 0.0f), Obstacle::ObstacleType::REVERSE_ROTATESTICK, 0);
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
		float rad = atan2(m_normalSpawnePosition[type].x - m_playerPosition.x, m_normalSpawnePosition[type].z - m_playerPosition.z);
		float  birdangle = ((DirectX::XM_PI / 2.0f) * type) ;


		float  angle = (DirectX::XM_PI / 2.0f) * type;
		if (m_time_s <= 50)
		{
			int createObs = MyRandom::GetIntRange(0, 5);

			if(createObs <= 4)
			{
				CreateObstacle(m_normalSpawnePosition[type], Obstacle::ObstacleType::NORMAL, rad);

			}
			else if (createObs == 5)
			{
				CreateObstacle(m_stickSpawnePosition[type], Obstacle::ObstacleType::STICK, angle);
			}

			
		}
		else 
		{
			int random = MyRandom::GetIntRange(0, 100);

			if (random <= 30)
			{
				CreateObstacle(m_normalSpawnePosition[type], Obstacle::ObstacleType::NORMAL, rad);

			}
			else if (random <= 45)
			{
				

				CreateObstacle(m_stickSpawnePosition[type], Obstacle::ObstacleType::STICK, angle);
			}
			else if (random <= 50)
			{

				CreateObstacle(m_birdSpawnPosition[type], Obstacle::ObstacleType::BIRD, birdangle);
			
			}else if (random <= 100)
			{

				CreateObstacle(m_normalSpawnePosition[type], Obstacle::ObstacleType::MEANDERING, rad);
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

bool ObstacleManeger::PlayerCapsuleHitCheck(Capsule* playerCapsule)
{
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		if (obstacle->IsActive() == false)
			continue;

		Capsule* cap = obstacle->GetCapsule();

		if (HitCheck_Capsule2Capsule(*cap, *playerCapsule))
		{
			return true;
		}

	}
	return false;
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
			DirectX::SimpleMath::Vector3 rot = obstacle->GetRotation();
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
/// <param name="angle">アングル</param>
/// <returns>true = 生成成功 , false = 生成失敗</returns>
bool ObstacleManeger::CreateObstacle(const DirectX::SimpleMath::Vector3& position, Obstacle::ObstacleType type, float angle)
{
	return m_spawners[type]->Create(m_obstacles, position, angle, m_behavior[type].get(), m_models[type], m_commonState);
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
