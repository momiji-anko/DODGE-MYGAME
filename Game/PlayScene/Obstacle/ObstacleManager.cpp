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
const int ObstacleManeger::OBSTACLE_MAX_NUM = 100;
//スポーンクールタイム
const int ObstacleManeger::EFFECT_MAX_NUM = 5;
//炎エフェクトの最大数
const float ObstacleManeger::SPAWN_COOL_TIME_S = 3.0f;

/// <summary>
/// コンストラクタ
/// </summary>
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

/// <summary>
/// デストラクタ
/// </summary>
ObstacleManeger::~ObstacleManeger()
{
	
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="commonState">コモンステートの生ポインタ</param>
/// <param name="stage">ステージの番号</param>
void ObstacleManeger::Initialize(DirectX::CommonStates* commonState, StageManager::StageSelect stage)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	m_commonState = commonState;


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
void ObstacleManeger::Update(const DX::StepTimer& timer)
{
	//経過時間
	float elapsedtime = timer.GetElapsedSeconds();
	//タイムを経過時間で足す
	m_time_s += elapsedtime;

	//スポーンタイムを経過時間で引く
	m_spawneTime_s -= elapsedtime;

	//スポーンタイムが０になったら障害物を生成する
	if (m_spawneTime_s <= 0.0f)
	{
		
		//出現させる障害物をランダムで決める
		int type = MyRandom::GetIntRange(0, 3);

		//角度
		DirectX::SimpleMath::Vector3 rot = DirectX::SimpleMath::Vector3::Zero;
		//角度設定		
		rot.y = (DirectX::XM_PI / 2.0f) * type;

		static const float LEVEL1_CREATE_OBSTACLE_TIME_S = 50.0f;




		//タイムがLEVEL1_CREATE_OBSTACLE_TIME_S以下であればノーマルの炎と棒の障害物わランダムで生成する
		if (m_time_s <= LEVEL1_CREATE_OBSTACLE_TIME_S)
		{
			//０から１００の乱数を取る
			int createObsRand = MyRandom::GetIntRange(0, 100);

			//乱数が80以下であれば普通の炎を生成する
			if(createObsRand <= 80)
			{
				//出現させる位置からプレイヤーのベクトルから向かうを設定する
				rot.y = atan2(m_normalSpawnePosition[type].x - m_playerPosition.x, m_normalSpawnePosition[type].z - m_playerPosition.z);
				//普通の炎生成
				CreateObstacle(m_normalSpawnePosition[type], Obstacle::ObstacleType::NORMAL, rot);
			}
			//乱数が８１以上１００以下であれば棒の障害物を作成
			else if (createObsRand <= 100)
			{
				//棒の障害物生成
				CreateObstacle(m_stickSpawnePosition[type], Obstacle::ObstacleType::STICK, rot);
			}
		}
		else 
		{

			//０から１００の乱数を取る
			int createObsRand = MyRandom::GetIntRange(0, 100);
			//乱数が３０以下であれば普通の炎を生成する
			if (createObsRand <= 30)
			{
				//出現させる位置からプレイヤーのベクトルから向かうを設定する
				rot.y = atan2(m_normalSpawnePosition[type].x - m_playerPosition.x, m_normalSpawnePosition[type].z - m_playerPosition.z);
				//普通の炎生成
				CreateObstacle(m_normalSpawnePosition[type], Obstacle::ObstacleType::NORMAL, rot);

			}
			//乱数が３１以上４５以下であれば棒の障害物を作成
			else if (createObsRand <= 45)
			{
				//棒の障害物生成
				CreateObstacle(m_stickSpawnePosition[type], Obstacle::ObstacleType::STICK, rot);
			}
			//乱数が４６以上５０以下であれば鳥の障害物を作成
			else if (createObsRand <= 50)
			{
				//鳥の障害物を作成
				CreateObstacle(m_birdSpawnPosition[type], Obstacle::ObstacleType::BIRD, rot);
			
			}
			//乱数が５１以上１００以下であれば蛇行する炎の障害物を作成
			else if (createObsRand <= 100)
			{
				//蛇行する炎の障害物を作成
				CreateObstacle(m_normalSpawnePosition[type], Obstacle::ObstacleType::MEANDERING, rot);
			}
		}
		

		//スポーンタイムにスポーンクールタイムを入れる
		m_spawneTime_s = m_spawneCoolTime_s;
		//だんだんスポーンクールタイムを減らしてスポーンタイムを早くする
		m_spawneCoolTime_s -= 0.003f;
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
void ObstacleManeger::Draw(Camera* camera)
{
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		if (obstacle->IsActive() == false)
			continue;

		obstacle->Draw(camera);
	}

}

/// <summary>
/// 終了処理
/// </summary>
void ObstacleManeger::Finalize()
{

}

/// <summary>
/// スポナー作成
/// </summary>
void ObstacleManeger::CreateSpawner()
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
void ObstacleManeger::CreateBehavior()
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
void ObstacleManeger::SetPlayerPosition(DirectX::SimpleMath::Vector3 position)
{
	//プレイヤーの座標設定
	m_playerPosition = position;

	//全ての障害物にプレイヤーの座標設定
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		//Actor型からObstacle型にダイナミックキャスト
		Obstacle* obs = dynamic_cast<Obstacle*> (obstacle.get());
		//NULLチェック（NULLであれば実行しない）
		if (obs == nullptr)
			continue;

		//プレイヤーの座標設定
		obs->SetPlayerPosition(m_playerPosition);
	}

}

/// <summary>
/// プレイヤーと障害物のカプセルの当たり判定
/// </summary>
/// <param name="player">プレイヤー</param>
/// <returns>true = 当たった , false = 当っていない</returns>
bool ObstacleManeger::PlayerHitCheck(AABBFor3D* playerAABB)
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
void ObstacleManeger::ObstacleShadow( ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	//全ての障害物の影を生成する
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		//障害物がアクティブ状態でなければ処理しない
		if (obstacle->IsActive() == false)
			continue;


		obstacle->CreateShadow(shadowMap, view, projection);
	}

}

/// <summary>
/// プレイヤーと障害物のカプセルの当たり判定
/// </summary>
/// <param name="player">プレイヤー</param>
/// <returns>true = 当たった , false = 当っていない</returns>
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
