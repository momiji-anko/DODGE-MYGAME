#include"pch.h"
#include"ObstacleSpawner.h"
#include"../Actor/Actor.h"
#include"Obstacle.h"
#include"../MyRandom.h"

bool NormalObstacleSpawner::Create(std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//使用中でなければ、敵を初期化する
		if (actor->IsActive())
			continue;


		Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
		if (obstale != nullptr)
		{
			obstale->SetType(Obstacle::ObstacleType::NORMAL);
			
		}
		DirectX::SimpleMath::Vector3 velocity;
		velocity.x = -sin(angle) * 0.1;
		velocity.z = cos(angle) * 0.1;
		//アイテムの初期化
		actor->Initialize(velocity, position, true, angle, behavior, model, commonState);




		//アイテムの生成に成功
		return true;

	}
	//すべてアイテムが使用中
	return false;
}

bool StickObstacleSpawner::Create(std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//使用中でなければ、敵を初期化する
		if (actor->IsActive())
			continue;


		Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
		if (obstale != nullptr)
		{
			obstale->SetType(Obstacle::ObstacleType::STICK);
			
		}

		//アイテムの初期化
		actor->Initialize(DirectX::SimpleMath::Vector3(-3.0f,0.0f,0.0f), position, true, angle, behavior, model, commonState);

		


		//アイテムの生成に成功
		return true;

	}
	//すべてアイテムが使用中
	return false;
}

bool MeteoriteObstacleSpawner::Create(std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//使用中でなければ、敵を初期化する
		if (actor->IsActive())
			continue;


		Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
		if (obstale != nullptr)
		{
			obstale->SetType(Obstacle::ObstacleType::METEORITE);
			
		}

		//アイテムの初期化
		actor->Initialize(DirectX::SimpleMath::Vector3(-3.0f,0.0f,0.0f), position, true, angle, behavior, model, commonState);

		


		//アイテムの生成に成功
		return true;

	}
	//すべてアイテムが使用中
	return false;
}

bool RotateStickObstacleSpawner::Create(std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//使用中でなければ、敵を初期化する
		if (actor->IsActive())
			continue;


		Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
		if (obstale != nullptr)
		{
			obstale->SetType(Obstacle::ObstacleType::ROTATESTICK);
			obstale->SetRotSpeed(0.01f);
		}

		//障害物初期化
		actor->Initialize(DirectX::SimpleMath::Vector3(0.0f, 10.0f, 0.0f), position, true, angle, behavior, model, commonState);




		//障害物の生成に成功
		return true;

	}
	//すべて障害物が使用中
	return false;
}


bool ReverseRotateStickObstacleSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//使用中でなければ、敵を初期化する
		if (actor->IsActive())
			continue;


		Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
		if (obstale != nullptr)
		{
			obstale->SetType(Obstacle::ObstacleType::REVERSE_ROTATESTICK);
			obstale->SetRotSpeed(-0.005f);
		}

		//障害物の初期化
		actor->Initialize(DirectX::SimpleMath::Vector3(0.0f, 10.0f, 0.0f), position, true, angle, behavior, model, commonState);




		//障害物の生成に成功
		return true;

	}
	//すべて障害物が使用中
	return false;
}


bool MeanderingObstacleSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//使用中でなければ、敵を初期化する
		if (actor->IsActive())
			continue;


		Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
		if (obstale != nullptr)
		{
			obstale->SetType(Obstacle::ObstacleType::MEANDERING);

		}
		DirectX::SimpleMath::Vector3 velocity;
		velocity.x = 1;
		velocity.z = 1;
		//アイテムの初期化
		actor->Initialize(velocity, position, true, angle, behavior, model, commonState);

		if (obstale != nullptr)
		{

		
		}


		//アイテムの生成に成功
		return true;

	}
	//すべてアイテムが使用中
	return false;
}

bool StraightMoveRightObstacleSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	
	int isCreate = 0;
	for (int i = 0; i < 6; i++)
	{
		for (std::unique_ptr<Actor>& actor : actors)
		{
			//使用中でなければ、敵を初期化する
			if (actor->IsActive())
				continue;


			Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
			if (obstale != nullptr)
			{
				obstale->SetType(Obstacle::ObstacleType::STRAIGHT_MOVE_RIGHT_LEFT);

			}

			DirectX::SimpleMath::Vector3 pos = { 0.0f,0.0f,4.0f * (i - 3) };



			//アイテムの初期化
			actor->Initialize(DirectX::SimpleMath::Vector3(-10.0f,0.0f,0.0f), position + pos, true, angle, behavior, model, commonState);




			//アイテムの生成に成功
			isCreate++;
			break;

		}
	}
	//アイテムの生成に成功
	if(isCreate>=6)
		return true;

	//すべてアイテムが使用中
	return false;
}

bool StraightMoveLeftObstacleSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	int isCreate = 0;
	for (int i = 0; i < 6; i++)
	{
		for (std::unique_ptr<Actor>& actor : actors)
		{
			//使用中でなければ、敵を初期化する
			if (actor->IsActive())
				continue;


			Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
			if (obstale != nullptr)
			{
				obstale->SetType(Obstacle::ObstacleType::STRAIGHT_MOVE_RIGHT_LEFT);

			}

			DirectX::SimpleMath::Vector3 pos = { 0.0f,0.0f,-4.0f * (i - 3) };



			//アイテムの初期化
			actor->Initialize(DirectX::SimpleMath::Vector3(10.0f, 0.0f, 0.0f), position + pos, true, angle, behavior, model, commonState);




			//アイテムの生成に成功
			isCreate++;
			break;

		}
	}
	//アイテムの生成に成功
	if (isCreate >= 6)
		return true;

	//すべてアイテムが使用中
	return false;
}

bool StraightMoveTopObstacleSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	int isCreate = 0;
	for (int i = 0; i < 6; i++)
	{
		for (std::unique_ptr<Actor>& actor : actors)
		{
			//使用中でなければ、敵を初期化する
			if (actor->IsActive())
				continue;


			Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
			if (obstale != nullptr)
			{
				obstale->SetType(Obstacle::ObstacleType::STRAIGHT_MOVE_TOP_BOTTOM);

			}

			DirectX::SimpleMath::Vector3 pos = { 4.0f * (i - 3),0.0f,0.0f };



			//アイテムの初期化
			actor->Initialize(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 10.0f), position + pos, true, angle, behavior, model, commonState);




			//アイテムの生成に成功
			isCreate++;
			break;

		}
	}
	//アイテムの生成に成功
	if (isCreate >= 6)
		return true;

	//すべてアイテムが使用中
	return false;
}

bool StraightMoveBottomObstacleSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	int isCreate = 0;
	for (int i = 0; i < 6; i++)
	{
		for (std::unique_ptr<Actor>& actor : actors)
		{
			//使用中でなければ、敵を初期化する
			if (actor->IsActive())
				continue;


			Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
			if (obstale != nullptr)
			{
				obstale->SetType(Obstacle::ObstacleType::STRAIGHT_MOVE_BOTTOM_TOP);

			}

			DirectX::SimpleMath::Vector3 pos = { -4.0f * (i - 3),0.0f,0.0f };



			//アイテムの初期化
			actor->Initialize(DirectX::SimpleMath::Vector3(0.0f, 0.0f, -10.0f), position + pos, true, angle, behavior, model, commonState);




			//アイテムの生成に成功
			isCreate++;
			break;

		}
	}
	//アイテムの生成に成功
	if (isCreate >= 6)
		return true;

	//すべてアイテムが使用中
	return false;
}

bool BirdObstacleSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	int spawnCount = 0;
	for (int i = 0; i < 6; i++)
	{
		for (std::unique_ptr<Actor>& actor : actors)
		{
			//使用中でなければ、敵を初期化する
			if (actor->IsActive())
				continue;


			Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
			if (obstale != nullptr)
			{
				obstale->SetType(Obstacle::ObstacleType::BIRD);

			}
			DirectX::SimpleMath::Vector3 velocity = DirectX::SimpleMath::Vector3(-10.0f, 0.0f, 0.0f);

		//	float angle;
			//障害物を回転させる（発射角度を計算する行列を作る）
			DirectX::SimpleMath::Matrix rotetion = DirectX::SimpleMath::Matrix::CreateRotationY(angle);
			//速度を計算する（発射角度を加味した移動速度を計算する）
			velocity = DirectX::SimpleMath::Vector3::TransformNormal(velocity, rotetion);

			DirectX::SimpleMath::Vector3 pos;
			pos = position;
			if (pos.x == 0.0f)
			{
				pos.x = (i - 2) * 4.0f;
			}
			if (pos.z == 0.0f)
			{
				pos.z = (i - 2) * 4.0f;
			}


			//障害物の初期化
			actor->Initialize(velocity, pos, true, angle, behavior, model, commonState);
			actor->SetRotation(DirectX::SimpleMath::Vector3(0.0f, angle+DirectX::XM_PI , 0.0f));


			//アイテムの生成に成功
			spawnCount++;
			break;

		}

	}

	if (spawnCount >= 6)
	{
		return true;
	}

	//すべてアイテムが使用中
	return false;
}
