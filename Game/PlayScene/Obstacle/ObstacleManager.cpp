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

//障害物最大数
const int ObstacleManager::OBSTACLE_MAX_NUM = 100;
//スポーンクールタイム
const int ObstacleManager::EFFECT_MAX_NUM = 5;
//炎エフェクトの最大数
const float ObstacleManager::SPAWN_COOL_TIME_S = 3.0f;
//スポーンクールタイムの減る数
const float ObstacleManager::SPAWNE_COOL_TIME_DECREASE_AMOUNT = 0.003f;

/// <summary>
/// コンストラクタ
/// </summary>
ObstacleManager::ObstacleManager()
	:
	m_obstacles{},
	m_spawners{},
	m_models{},
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
	m_spawneTime_s(0.0f),
	m_hitType(Obstacle::ObstacleType::NONE)
{
}

/// <summary>
/// デストラクタ
/// </summary>
ObstacleManager::~ObstacleManager()
{
	
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="stage">ステージの番号</param>
void ObstacleManager::Initialize(StageManager::StageSelect stage)
{


	//エフェクト配列をエフェクトの最大数でリサイズ
	m_effectlist.resize(EFFECT_MAX_NUM);
	//エフェクトの作成
	for (int i = 0; i < m_effectlist.size(); i++)
	{
		m_effectlist[i] = std::make_unique<FireEffectManager>();
		m_effectlist[i]->Create();
		//エフェクトの初期化
		m_effectlist[i]->Initialize(2.0f, DirectX::SimpleMath::Vector3::Zero);
	}

	//障害物の配列を障害物の最大数でリサイズ
	m_obstacles.resize(OBSTACLE_MAX_NUM);
	//障害物の作成
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		//障害物の作成
		std::unique_ptr<Obstacle> obs = std::make_unique<Obstacle>();
		//ランダムで炎のエフェクトを障害物に渡す
		int num = MyRandom::GetIntRange(0, EFFECT_MAX_NUM - 1);
		//エフェクトを渡す
		obs->SetEffect(m_effectlist[num].get());
		//障害物型からアクター型にする
		obstacle = std::move(obs);
		
	}
	//障害物スポナ作成
	CreateSpawner();
	//障害物ビヘイビアー作成
	CreateBehavior();
	
	//スポーンクールタイム設定
	m_spawneCoolTime_s = SPAWN_COOL_TIME_S;
	//スポーンタイム設定
	m_spawneTime_s = SPAWN_COOL_TIME_S;

	//障害物モデル作成
	CreateModel();
	
	//Stage1とStage2であれば回転する棒を生成する
	if (stage == StageManager::StageSelect::Stage1 || stage == StageManager::StageSelect::Stage2)
	{
		//回転する棒の生成
		CreateObstacle(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), Obstacle::ObstacleType::ROTATESTICK, DirectX::SimpleMath::Vector3::Zero);
		CreateObstacle(DirectX::SimpleMath::Vector3(0.0f, 3.0f, 0.0f), Obstacle::ObstacleType::REVERSE_ROTATESTICK, DirectX::SimpleMath::Vector3::Zero);
	}

}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void ObstacleManager::Update(const DX::StepTimer& timer)
{
	if (m_hitType != Obstacle::ObstacleType::NONE)
	{
		return;
	}

	//経過時間
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	//タイムを経過時間で足す
	m_time_s += elapsedTime;

	//スポーンタイムを経過時間で引く
	m_spawneTime_s -= elapsedTime;

	//スポーンタイムが０になったら障害物を生成する
	if (m_spawneTime_s <= 0.0f)
	{
		static const float LEVEL1_CREATE_OBSTACLE_TIME_S = 50.0f;

		//障害物の角度と出現位置を決める
		int obstacleRand = MyRandom::GetIntRange(2, 3);

		//角度
		DirectX::SimpleMath::Vector3 rot = DirectX::SimpleMath::Vector3::Zero;
		//角度設定		
		rot.y = (DirectX::XM_PI / 2.0f) * obstacleRand;



		//０から１００の乱数を取る
		int createObsRand = MyRandom::GetIntRange(0, 100);

		//タイムが50以下であればノーマルの炎と棒の障害物をランダムで生成する
		if (m_time_s <= LEVEL1_CREATE_OBSTACLE_TIME_S)
		{
			

			//乱数で出現させる障害物のボーダーライン
			static const int NOERMAL_FIRE_SPAWNE_NUM = 80;
			static const int STICK_SPAWNE_NUM = 100;

			//乱数が80以下であれば普通の炎を生成する
			if(createObsRand <= NOERMAL_FIRE_SPAWNE_NUM)
			{
				//出現させる位置からプレイヤーのベクトルから向かうを設定する
				rot.y = atan2(m_normalSpawnePosition[obstacleRand].x - m_playerPosition.x, m_normalSpawnePosition[obstacleRand].z - m_playerPosition.z);
				//普通の炎生成
				CreateObstacle(m_normalSpawnePosition[obstacleRand], Obstacle::ObstacleType::NORMAL, rot);
			}
			//乱数が８１以上１００以下であれば棒の障害物を作成
			else if (createObsRand <= STICK_SPAWNE_NUM)
			{
				//棒の障害物生成
				CreateObstacle(m_stickSpawnePosition[obstacleRand], Obstacle::ObstacleType::STICK, rot);
			}
		}
		//タイムが50より大きなければノーマルの炎と棒と鳥と蛇行する炎の障害物をランダムで生成する
		else if(m_time_s > LEVEL1_CREATE_OBSTACLE_TIME_S)
		{
			//乱数で出現させる障害物のボーダーライン
			static const int NOERMAL_FIRE_SPAWNE_NUM = 30;
			static const int STICK_SPAWNE_NUM = 45;
			static const int BIRD_SPAWNE_NUM = 60;
			static const int MEANDERING_FIRE_SPAWNE_NUM = 100;

			//乱数が３０以下であれば普通の炎を生成する
			if (createObsRand <= NOERMAL_FIRE_SPAWNE_NUM)
			{
				//出現させる位置からプレイヤーのベクトルから向かうを設定する
				rot.y = atan2(m_normalSpawnePosition[obstacleRand].x - m_playerPosition.x, m_normalSpawnePosition[obstacleRand].z - m_playerPosition.z);
				//普通の炎生成
				CreateObstacle(m_normalSpawnePosition[obstacleRand], Obstacle::ObstacleType::NORMAL, rot);

			}
			//乱数が３１以上４５以下であれば棒の障害物を作成
			else if (createObsRand <= STICK_SPAWNE_NUM)
			{
				//棒の障害物生成
				CreateObstacle(m_stickSpawnePosition[obstacleRand], Obstacle::ObstacleType::STICK, rot);
			}
			//乱数が４６以上５０以下であれば鳥の障害物を作成
			else if (createObsRand <= BIRD_SPAWNE_NUM)
			{
				//鳥の障害物を作成
				CreateObstacle(m_birdSpawnPosition[obstacleRand], Obstacle::ObstacleType::BIRD, rot);
			
			}
			//乱数が５１以上１００以下であれば蛇行する炎の障害物を作成
			else if (createObsRand <= MEANDERING_FIRE_SPAWNE_NUM)
			{
				//蛇行する炎の障害物を作成
				CreateObstacle(m_normalSpawnePosition[obstacleRand], Obstacle::ObstacleType::MEANDERING, rot);
			}
		}
		

		//スポーンタイムにスポーンクールタイムを入れる
		m_spawneTime_s = m_spawneCoolTime_s;
		//だんだんスポーンクールタイムを減らしてスポーンタイムを早くする
		m_spawneCoolTime_s -= SPAWNE_COOL_TIME_DECREASE_AMOUNT;
	}

	
	//障害物の更新
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		//非アクティブ状態であれば更新しない
		if (obstacle->IsActive() == false)
			continue;
		
		//更新
		obstacle->Update(timer);
	}
}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラの生ポインタ</param>
void ObstacleManager::Draw(Camera* camera)
{
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		//非アクティブ状態であれば描画しない
		if (obstacle->IsActive() == false)
			continue;

		//障害物の描画
		obstacle->Draw(camera);
	}

}

/// <summary>
/// 終了処理
/// </summary>
void ObstacleManager::Finalize()
{

}

/// <summary>
/// スポナー作成
/// </summary>
void ObstacleManager::CreateSpawner()
{
	//普通の炎
	m_spawners[Obstacle::ObstacleType::NORMAL] = std::make_unique<NormalObstacleSpawner>();
	//棒
	m_spawners[Obstacle::ObstacleType::STICK] = std::make_unique<StickObstacleSpawner>();
	//隕石
	m_spawners[Obstacle::ObstacleType::METEORITE] = std::make_unique<MeteoriteObstacleSpawner>();
	//回転する棒
	m_spawners[Obstacle::ObstacleType::ROTATESTICK] = std::make_unique<RotateStickObstacleSpawner>();
	//
	m_spawners[Obstacle::ObstacleType::REVERSE_ROTATESTICK] = std::make_unique<ReverseRotateStickObstacleSpawner>();
	//蛇行する炎
	m_spawners[Obstacle::ObstacleType::MEANDERING] = std::make_unique<MeanderingObstacleSpawner>();
	//鳥
	m_spawners[Obstacle::ObstacleType::BIRD] = std::make_unique<BirdObstacleSpawner>();
}


/// <summary>
/// ビヘイビアー作成
/// </summary>
void ObstacleManager::CreateBehavior()
{
	//普通の炎
	m_behavior[Obstacle::ObstacleType::NORMAL] = std::make_unique<NormalBehavior>();
	//棒
	m_behavior[Obstacle::ObstacleType::STICK] = std::make_unique<StickBehavior>();
	//隕石
	m_behavior[Obstacle::ObstacleType::METEORITE] = std::make_unique<MeteoriteBehavior>();
	//回転する棒
	m_behavior[Obstacle::ObstacleType::ROTATESTICK] = std::make_unique<RotateStickBehavior>();
	//時計回りに回転する棒
	m_behavior[Obstacle::ObstacleType::REVERSE_ROTATESTICK] = std::make_unique<ReverseRotateStickBehavior>();
	//蛇行する炎
	m_behavior[Obstacle::ObstacleType::MEANDERING] = std::make_unique<MeanderingBehavior>();
	//鳥
	m_behavior[Obstacle::ObstacleType::BIRD] = std::make_unique<BirdBehavior>();

}

/// <summary>
/// プレイヤーの座標設定
/// </summary>
/// <param name="position">プレイヤーの座標</param>
void ObstacleManager::SetPlayerPosition(const DirectX::SimpleMath::Vector3& position)
{
	//プレイヤーの座標設定
	m_playerPosition = position;

	//全ての障害物にプレイヤーの座標設定
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{

		//プレイヤーの座標設定
		obstacle->SetPlayerPosition(m_playerPosition);
	}

}

/// <summary>
/// プレイヤーと障害物のカプセルの当たり判定
/// </summary>
/// <param name="player">プレイヤー</param>
/// <returns>true = 当たった , false = 当っていない</returns>
bool ObstacleManager::PlayerHitCheck(AABBFor3D* playerAABB)
{
	//全ての障害物にプレイヤー当たり判定
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		//障害物がアクティブ状態でなければ処理しない
		if (obstacle->IsActive() == false)
			continue;

		//障害物をプレイヤーと当たり判定を取る
		if (obstacle->GetAABB()->HitCheck(playerAABB))
		{

			//プレイヤーに当たった障害物のタイプ設定
			m_hitType = static_cast<Obstacle::ObstacleType>(obstacle->GetTypeInt());
			
			//当たっている
			return true;
		}
		
	}
	//全ての障害物を当っていない
	return false;
}


/// <summary>
/// 障害物の影作成
/// </summary>
/// <param name="shadowMap">シャドウマップの生ポインタ</param>
/// <param name="view">ビュー行列</param>
/// <param name="projection">プロジェクション行列</param>
void ObstacleManager::ObstacleShadow( ShadowMap* shadowMap, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	//全ての障害物の影を生成する
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		//障害物がアクティブ状態でなければ処理しない
		if (obstacle->IsActive() == false)
			continue;

		//影の生成
		obstacle->CreateShadow(shadowMap, view, projection);
	}

}

/// <summary>
/// プレイヤーと障害物のカプセルの当たり判定
/// </summary>
/// <param name="player">プレイヤー</param>
/// <returns>true = 当たった , false = 当っていない</returns>
bool ObstacleManager::PlayerCapsuleHitCheck(Actor* player, DirectX::SimpleMath::Vector3* flyVelocity)
{
	//障害物とプレイヤーのカプセルの当たり判定
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		//非アクティブ状態であれば実行しない
		if (obstacle->IsActive() == false)
			continue;

		//障害物の角度取得
		Capsule* cap = obstacle->GetCapsule();
		//当たっていれば実行する
		if (CapsuleHit::HitCheck_Capsule2Capsule(*cap, *player->GetCapsule(),m_capsuleHitC1,m_capsuleHitC2))
		{
			bool isRoteStick = obstacle->GetTypeInt() == static_cast<int>(Obstacle::ObstacleType::ROTATESTICK);

			//下の回転棒に当たった時の処理
			if ( isRoteStick)
			{
				*flyVelocity = RotaStickHit(obstacle.get(), player);
			}

			bool isReveseRoteStick = obstacle->GetTypeInt() == static_cast<int>(Obstacle::ObstacleType::REVERSE_ROTATESTICK);

			//上の回転する棒障害物に当たった時の処理
			if ( isReveseRoteStick)
			{
				*flyVelocity = ReverseRotaStickHit(obstacle.get(), player);

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
bool ObstacleManager::CreateObstacle(const DirectX::SimpleMath::Vector3& position, Obstacle::ObstacleType type, const DirectX::SimpleMath::Vector3& rot)
{
	return m_spawners[type]->Create(m_obstacles, position, rot, m_behavior[type].get(), m_models[type]);
}

/// <summary>
/// 障害物のモデル生成
/// 障害物のモデル生成
/// </summary>
void ObstacleManager::CreateModel()
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
/// プレイヤーを吹き飛ばす	
/// </summary>
/// <param name="rotSpeed">障害物の回転速度</param>
/// <param name="player">プレイヤー</param>
DirectX::SimpleMath::Vector3 ObstacleManager::FlyPlayer(float rotSpeed)
{

	//飛ばすためにスピードに２掛ける
	float speedTwice = 2.0f;
	//飛ばすための補正
	float speedCorrection = 0.5f;
	//飛ぶ量
	float flyAmount = speedCorrection + (std::abs(rotSpeed) * speedTwice);

	return m_hitvel * flyAmount;

}

/// <summary>
/// プレイヤーのめり込み処理
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="cupseleToCupseVector">当たったカプセルとカプセルのベクトル</param>
/// <param name="playerCapsleRadius">プレイヤーのカプセルの半径</param>
/// <param name="obstacleCupsleRadius">障害物のカプセルの半径</param>
void ObstacleManager::PlayerCapuslePenetration(Actor* player, const DirectX::SimpleMath::Vector3& cupseleToCupseVector, float playerCapsleRadius, float obstacleCupsleRadius)
{
	//当たったカプセルとカプセルの距離
	float cupseleToCupselLenth = cupseleToCupseVector.Length();
	//プレイヤーと障害物の半径の合計
	float radius = playerCapsleRadius + obstacleCupsleRadius;

	DirectX::SimpleMath::Vector3 capToCapVector= cupseleToCupseVector;

	//当たったカプセルとカプセルの距離がプレイヤーと障害物の半径の合計より小さければめり込み処理をする
	if (cupseleToCupselLenth < radius)
	{
		//どれだけめり込んでいるか計算
		float capToCapLengthToRasiusDifference = cupseleToCupselLenth - radius;
		//当たったカプセルとカプセルのベクトルを正規化
		capToCapVector.Normalize();
		//当たったカプセルとカプセルのベクトルにめり込んだ量をかける		
		DirectX::SimpleMath::Vector3 playerPenetrationVelocity = capToCapVector * capToCapLengthToRasiusDifference;
		//プレイヤーのめり込んだ量を押し出す
		player->SetPosition(player->GetPosition() + playerPenetrationVelocity);
	}


}

/// <summary>
/// 回転する棒に当たった時の処理
/// </summary>
/// <param name="obstacle">回転する棒の障害物</param>
/// <param name="player">プレイヤー</param>
DirectX::SimpleMath::Vector3 ObstacleManager::RotaStickHit(Actor* obstacle, Actor* player)
{
	//障害物の角度取得
	DirectX::SimpleMath::Vector3 rot = obstacle->GetRotation().ToEuler();

	//障害物カプセルからプレイヤーカプセルのベクトル
	m_hitvel = m_capsuleHitC2 - m_capsuleHitC1;

	//プレイヤーカプセルの半径
	float playerR = player->GetCapsule()->r;
	//障害物カプセルの半径
	float obstacleR = obstacle->GetCapsule()->r;
	//めり込み処理
	PlayerCapuslePenetration(player, -m_hitvel, playerR, obstacleR);

	DirectX::SimpleMath::Vector3 flyVel = DirectX::SimpleMath::Vector3::Zero;

	//直角
	static const float NINETY_ANGLE = DirectX::XMConvertToDegrees(DirectX::XM_PI / 2.0f);

	//障害物の回転方向の計算
	DirectX::SimpleMath::Vector3 rotDir = DirectX::SimpleMath::Vector3(cos(rot.y + (NINETY_ANGLE)), 0, -sin(rot.y + (NINETY_ANGLE)));

	//障害物カプセルからプレイヤーカプセルのベクトル正規化する
	m_hitvel.Normalize();

	//障害物カプセルからプレイヤーカプセルのベクトルと障害物の回転方向で内積を取る
	float dot = m_hitvel.Dot(rotDir);

	//内積が０以上であればプレイヤーを吹き飛ばす
	if (dot > 0.0f)
	{
		flyVel = FlyPlayer(obstacle->GetRotSpeed());
	}

	return flyVel;

}

/// <summary>
/// 反対回転する棒に当たった時の処理
/// </summary>
/// <param name="obstacle">回転する棒の障害物</param>
/// <param name="player">プレイヤー</param>	
DirectX::SimpleMath::Vector3 ObstacleManager::ReverseRotaStickHit(Actor* obstacle, Actor* player)
{

	//当たったカプセル同士の座標を足す
	m_hitvel = m_capsuleHitC2 + m_capsuleHitC1;

	//正規化
	m_hitvel.Normalize();
	//障害物のカプセルの半径を足しプレイヤーと障害物の接触した場所を計算
	m_hitvel *= obstacle->GetCapsule()->r;

	//プレイヤーと障害物の接触座標にプレイヤーの足元の座標を足す
	DirectX::SimpleMath::Vector3 targetVector = targetVector = m_hitvel + player->GetCapsule()->a;
	//プレイヤーの足元の座標のプレイヤーの頭座標を足す
	DirectX::SimpleMath::Vector3 playerVector = player->GetCapsule()->b + player->GetCapsule()->a;

	DirectX::SimpleMath::Vector3 flyVel = DirectX::SimpleMath::Vector3::Zero;


	//正規化
	targetVector.Normalize();
	playerVector.Normalize();

	//角度を割り出す
	float dot = targetVector.Dot(playerVector);
	float sita = acos(std::abs(dot));

	//頭打ち角度
	float headAngle = 0.0877f;

	//角度が頭打ち角度より大きかったら頭を打ったことにし、プレイヤーを即時落下させる
	if (sita >= headAngle)
	{
		DirectX::SimpleMath::Vector3 playerVec = player->GetVelocity();
		player->SetVelocity(DirectX::SimpleMath::Vector3(playerVec.x, 0.0f, playerVec.z));
	}
	else
	{
		//角度が頭打ち角度より小さかったらプレイヤーを吹き飛ばす					
		m_hitvel = m_capsuleHitC2 - m_capsuleHitC1;

		//プレイヤーカプセルの半径
		float playerR = player->GetCapsule()->r;
		//障害物カプセルの半径
		float obstacleR = obstacle->GetCapsule()->r;

		//めり込み処理
		PlayerCapuslePenetration(player, -m_hitvel, playerR, obstacleR);

		//正規化
		m_hitvel.Normalize();

		//障害物の回転速度取得
		float rotSpeed = obstacle->GetRotSpeed();

		//プレイヤーを吹き飛ばす
		flyVel = FlyPlayer(rotSpeed);
	}

	return  flyVel;

}
