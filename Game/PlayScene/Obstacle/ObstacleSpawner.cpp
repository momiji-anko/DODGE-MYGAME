#include"pch.h"
#include"ObstacleSpawner.h"
#include"../Actor/Actor.h"
#include"Obstacle.h"
#include"../MyRandom.h"

bool NormalObstacleSpawner::Create(std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//�g�p���łȂ���΁A�G������������
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
		//�A�C�e���̏�����
		actor->Initialize(velocity, position, true, angle, behavior, model, commonState);




		//�A�C�e���̐����ɐ���
		return true;

	}
	//���ׂăA�C�e�����g�p��
	return false;
}

bool StickObstacleSpawner::Create(std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//�g�p���łȂ���΁A�G������������
		if (actor->IsActive())
			continue;


		Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
		if (obstale != nullptr)
		{
			obstale->SetType(Obstacle::ObstacleType::STICK);
			
		}

		//�A�C�e���̏�����
		actor->Initialize(DirectX::SimpleMath::Vector3(-3.0f,0.0f,0.0f), position, true, angle, behavior, model, commonState);

		


		//�A�C�e���̐����ɐ���
		return true;

	}
	//���ׂăA�C�e�����g�p��
	return false;
}

bool MeteoriteObstacleSpawner::Create(std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//�g�p���łȂ���΁A�G������������
		if (actor->IsActive())
			continue;


		Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
		if (obstale != nullptr)
		{
			obstale->SetType(Obstacle::ObstacleType::METEORITE);
			
		}

		//�A�C�e���̏�����
		actor->Initialize(DirectX::SimpleMath::Vector3(-3.0f,0.0f,0.0f), position, true, angle, behavior, model, commonState);

		


		//�A�C�e���̐����ɐ���
		return true;

	}
	//���ׂăA�C�e�����g�p��
	return false;
}

bool RotateStickObstacleSpawner::Create(std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//�g�p���łȂ���΁A�G������������
		if (actor->IsActive())
			continue;


		Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
		if (obstale != nullptr)
		{
			obstale->SetType(Obstacle::ObstacleType::ROTATESTICK);
			obstale->SetRotSpeed(0.01f);
		}

		//��Q��������
		actor->Initialize(DirectX::SimpleMath::Vector3(0.0f, 10.0f, 0.0f), position, true, angle, behavior, model, commonState);




		//��Q���̐����ɐ���
		return true;

	}
	//���ׂď�Q�����g�p��
	return false;
}


bool ReverseRotateStickObstacleSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//�g�p���łȂ���΁A�G������������
		if (actor->IsActive())
			continue;


		Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
		if (obstale != nullptr)
		{
			obstale->SetType(Obstacle::ObstacleType::REVERSE_ROTATESTICK);
			obstale->SetRotSpeed(-0.005f);
		}

		//��Q���̏�����
		actor->Initialize(DirectX::SimpleMath::Vector3(0.0f, 10.0f, 0.0f), position, true, angle, behavior, model, commonState);




		//��Q���̐����ɐ���
		return true;

	}
	//���ׂď�Q�����g�p��
	return false;
}


bool MeanderingObstacleSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//�g�p���łȂ���΁A�G������������
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
		//�A�C�e���̏�����
		actor->Initialize(velocity, position, true, angle, behavior, model, commonState);

		if (obstale != nullptr)
		{

		
		}


		//�A�C�e���̐����ɐ���
		return true;

	}
	//���ׂăA�C�e�����g�p��
	return false;
}

bool StraightMoveRightObstacleSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	
	int isCreate = 0;
	for (int i = 0; i < 6; i++)
	{
		for (std::unique_ptr<Actor>& actor : actors)
		{
			//�g�p���łȂ���΁A�G������������
			if (actor->IsActive())
				continue;


			Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
			if (obstale != nullptr)
			{
				obstale->SetType(Obstacle::ObstacleType::STRAIGHT_MOVE_RIGHT_LEFT);

			}

			DirectX::SimpleMath::Vector3 pos = { 0.0f,0.0f,4.0f * (i - 3) };



			//�A�C�e���̏�����
			actor->Initialize(DirectX::SimpleMath::Vector3(-10.0f,0.0f,0.0f), position + pos, true, angle, behavior, model, commonState);




			//�A�C�e���̐����ɐ���
			isCreate++;
			break;

		}
	}
	//�A�C�e���̐����ɐ���
	if(isCreate>=6)
		return true;

	//���ׂăA�C�e�����g�p��
	return false;
}

bool StraightMoveLeftObstacleSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	int isCreate = 0;
	for (int i = 0; i < 6; i++)
	{
		for (std::unique_ptr<Actor>& actor : actors)
		{
			//�g�p���łȂ���΁A�G������������
			if (actor->IsActive())
				continue;


			Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
			if (obstale != nullptr)
			{
				obstale->SetType(Obstacle::ObstacleType::STRAIGHT_MOVE_RIGHT_LEFT);

			}

			DirectX::SimpleMath::Vector3 pos = { 0.0f,0.0f,-4.0f * (i - 3) };



			//�A�C�e���̏�����
			actor->Initialize(DirectX::SimpleMath::Vector3(10.0f, 0.0f, 0.0f), position + pos, true, angle, behavior, model, commonState);




			//�A�C�e���̐����ɐ���
			isCreate++;
			break;

		}
	}
	//�A�C�e���̐����ɐ���
	if (isCreate >= 6)
		return true;

	//���ׂăA�C�e�����g�p��
	return false;
}

bool StraightMoveTopObstacleSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	int isCreate = 0;
	for (int i = 0; i < 6; i++)
	{
		for (std::unique_ptr<Actor>& actor : actors)
		{
			//�g�p���łȂ���΁A�G������������
			if (actor->IsActive())
				continue;


			Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
			if (obstale != nullptr)
			{
				obstale->SetType(Obstacle::ObstacleType::STRAIGHT_MOVE_TOP_BOTTOM);

			}

			DirectX::SimpleMath::Vector3 pos = { 4.0f * (i - 3),0.0f,0.0f };



			//�A�C�e���̏�����
			actor->Initialize(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 10.0f), position + pos, true, angle, behavior, model, commonState);




			//�A�C�e���̐����ɐ���
			isCreate++;
			break;

		}
	}
	//�A�C�e���̐����ɐ���
	if (isCreate >= 6)
		return true;

	//���ׂăA�C�e�����g�p��
	return false;
}

bool StraightMoveBottomObstacleSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	int isCreate = 0;
	for (int i = 0; i < 6; i++)
	{
		for (std::unique_ptr<Actor>& actor : actors)
		{
			//�g�p���łȂ���΁A�G������������
			if (actor->IsActive())
				continue;


			Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
			if (obstale != nullptr)
			{
				obstale->SetType(Obstacle::ObstacleType::STRAIGHT_MOVE_BOTTOM_TOP);

			}

			DirectX::SimpleMath::Vector3 pos = { -4.0f * (i - 3),0.0f,0.0f };



			//�A�C�e���̏�����
			actor->Initialize(DirectX::SimpleMath::Vector3(0.0f, 0.0f, -10.0f), position + pos, true, angle, behavior, model, commonState);




			//�A�C�e���̐����ɐ���
			isCreate++;
			break;

		}
	}
	//�A�C�e���̐����ɐ���
	if (isCreate >= 6)
		return true;

	//���ׂăA�C�e�����g�p��
	return false;
}

bool BirdObstacleSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	int spawnCount = 0;
	for (int i = 0; i < 6; i++)
	{
		for (std::unique_ptr<Actor>& actor : actors)
		{
			//�g�p���łȂ���΁A�G������������
			if (actor->IsActive())
				continue;


			Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
			if (obstale != nullptr)
			{
				obstale->SetType(Obstacle::ObstacleType::BIRD);

			}
			DirectX::SimpleMath::Vector3 velocity = DirectX::SimpleMath::Vector3(-10.0f, 0.0f, 0.0f);

		//	float angle;
			//��Q������]������i���ˊp�x���v�Z����s������j
			DirectX::SimpleMath::Matrix rotetion = DirectX::SimpleMath::Matrix::CreateRotationY(angle);
			//���x���v�Z����i���ˊp�x�����������ړ����x���v�Z����j
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


			//��Q���̏�����
			actor->Initialize(velocity, pos, true, angle, behavior, model, commonState);
			actor->SetRotation(DirectX::SimpleMath::Vector3(0.0f, angle+DirectX::XM_PI , 0.0f));


			//�A�C�e���̐����ɐ���
			spawnCount++;
			break;

		}

	}

	if (spawnCount >= 6)
	{
		return true;
	}

	//���ׂăA�C�e�����g�p��
	return false;
}
