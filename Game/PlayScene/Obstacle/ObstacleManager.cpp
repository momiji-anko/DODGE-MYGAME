#include"pch.h"
#include"ObstacleManager.h"
#include<Effects.h>
#include"DeviceResources.h"
#include"../MyRandom.h"
#include"../Player/Player.h"
#include"../../GameMain.h"


const int ObstacleManeger::OBSTACLE_MAX_NUM = 100;
const int ObstacleManeger::EFFECT_MAX_NUM = 5;
const float ObstacleManeger::SPANE_COOL_TIME_S = 3.0f;

//コンストラクタ
ObstacleManeger::ObstacleManeger() 
	:
	m_obstacles{},
	m_spawners{},
	m_models{},
	m_commonState(nullptr),
	m_normalSpawnePosition{ 
		{0,DirectX::SimpleMath::Vector3(19,0.0f,-19) },
		{1,DirectX::SimpleMath::Vector3(19,0.0f,19) },
		{2,DirectX::SimpleMath::Vector3(-19,0.0f,19) },
		{3,DirectX::SimpleMath::Vector3(-19,0.0f,-19) },
	},
	m_stickSpawnePosition{ 
		{0,DirectX::SimpleMath::Vector3(19.0f,0.0f,0.0f) },
		{1,DirectX::SimpleMath::Vector3(0.0f,0.0f,-19.0f) },
		{2,DirectX::SimpleMath::Vector3(-19.0f,0.0f,0.0f) },
		{3,DirectX::SimpleMath::Vector3(0.0f,0.0f,19) },
	},
	m_birdSpawnPosition{
		{0,DirectX::SimpleMath::Vector3(19.0f ,  3.0f ,0.0f) },
		{1,DirectX::SimpleMath::Vector3(0.0f  ,  3.0f ,-19.0f) },
		{2,DirectX::SimpleMath::Vector3(-19.0f,  3.0f ,0.0f) },
		{3,DirectX::SimpleMath::Vector3(0.0f  ,  3.0f ,19.0f) },
	},
	m_hitvel{0,0,0},
	m_time_s(0.0f),
	m_spawneCoolTime(0.0f),
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

	CreateSpawner();

	CreateBehavior();
	

	m_spawneCoolTime = SPANE_COOL_TIME_S;
	m_spawneTime_s = 0.0f;

	//	エフェクトファクトリの作成
	DirectX::EffectFactory* meteoriteFactory = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//	テクスチャの読み込みパス指定
	meteoriteFactory->SetDirectory(L"Resources/Models");

	//	ファイルを指定してモデルデータ読み込み
	m_models[Obstacle::ObstacleType::METEORITE] = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/star.cmo",
		*meteoriteFactory
	);


	delete meteoriteFactory;
		//	エフェクトファクトリの作成
	DirectX::EffectFactory* stickFactory = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//	テクスチャの読み込みパス指定
	stickFactory->SetDirectory(L"Resources/Models");

	//	ファイルを指定してモデルデータ読み込み
	m_models[Obstacle::ObstacleType::STICK] = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/stickObstale.cmo",
		*stickFactory
	);

	delete stickFactory;

	//	エフェクトファクトリの作成
	DirectX::EffectFactory* rotateStickFactory = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//	テクスチャの読み込みパス指定
	rotateStickFactory->SetDirectory(L"Resources/Models");

	//	ファイルを指定してモデルデータ読み込み
	m_models[Obstacle::ObstacleType::ROTATESTICK] = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/roll.cmo",
		*rotateStickFactory
	);
	


	delete rotateStickFactory;
	//	エフェクトファクトリの作成
	DirectX::EffectFactory* birdFactory = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//	テクスチャの読み込みパス指定
	birdFactory->SetDirectory(L"Resources/Models");

	//	ファイルを指定してモデルデータ読み込み
	m_models[Obstacle::ObstacleType::BIRD] = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/bule.cmo",
		*birdFactory
	);

	delete birdFactory;
	//	エフェクトファクトリの作成
	DirectX::EffectFactory* reverceRotateStickFactory = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//	テクスチャの読み込みパス指定
	reverceRotateStickFactory->SetDirectory(L"Resources/Models");

	//	ファイルを指定してモデルデータ読み込み
	m_models[Obstacle::ObstacleType::REVERSE_ROTATESTICK] = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/roll2.cmo",
		*reverceRotateStickFactory
	);

	delete reverceRotateStickFactory;

	
	CreateObstacle(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), Obstacle::ObstacleType::ROTATESTICK, 0);
	CreateObstacle(DirectX::SimpleMath::Vector3(0.0f, 3.0f, 0.0f), Obstacle::ObstacleType::REVERSE_ROTATESTICK, 0);


	
}

// 更新
void ObstacleManeger::Update(const DX::StepTimer& timer)
{
	float time = timer.GetElapsedSeconds();
	m_time_s += time;
	m_spawneTime_s -= time;
	if (m_spawneTime_s <= 0.0f)
	{
		m_spawneTime_s = m_spawneCoolTime;
		int type = MyRandom::GetIntRange(0, 3);
		float rad = atan2(m_normalSpawnePosition[type].x - m_playerPosition.x, m_normalSpawnePosition[type].z - m_playerPosition.z);
		float  birdangle = ((DirectX::XM_PI / 2.0f) * type) ;

		if (m_time_s <= 50)
		{
			float  angle = (DirectX::XM_PI / 2.0f) * type;
			switch (MyRandom::GetIntRange(0, 5))
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
				



				CreateObstacle(m_normalSpawnePosition[type], Obstacle::ObstacleType::NORMAL, rad);
				//CreateObstacle(m_normalSpawnePosition[type], Obstacle::ObstacleType::MEANDERING, rad);
				//CreateObstacle(m_playerPosition+DirectX::SimpleMath::Vector3(0.0f,10.0f,0.0f), Obstacle::ObstacleType::METEORITE, rad);
				break;
			case 5:




				CreateObstacle(m_stickSpawnePosition[type], Obstacle::ObstacleType::STICK, angle);

				break;

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
				float  angle = (DirectX::XM_PI / 2.0f) * type;

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
		

		
		m_spawneCoolTime -= 0.003f;

		if (m_spawneCoolTime <= 1.0f)
		{
			//m_spawneCoolTime = 1.0f;
		}
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

					//m_hitvel.y += 1.1f;
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


	//障害物の生成
bool ObstacleManeger::CreateObstacle(const DirectX::SimpleMath::Vector3& position, Obstacle::ObstacleType type, float angle)
{
	return m_spawners[type]->Create(m_obstacles, position, angle, m_behavior[type].get(), m_models[type].get(), m_commonState);
}