#include"pch.h"
#include"ObstacleBehavior.h"
#include"../Actor/Actor.h"
#include"Obstacle.h"

void NormalBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();



	actor->SetPosition(position + (velocity * timer.GetElapsedSeconds()));



}

void StickBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();



	actor->SetPosition(position + (velocity * timer.GetElapsedSeconds()));

}

void RotateStickBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();
	
	DirectX::SimpleMath::Matrix world = actor->GetWorld();
	world *=  DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0,0,0));
	actor->SetWorld(world);



	DirectX::SimpleMath::Vector3 rot = actor->GetRotation();
	Obstacle* obstacle = dynamic_cast<Obstacle*>(actor);
	float speed = obstacle->GetRotSpeed();
	speed += 0.000001f;

	//if(speed)
	
	rot.y += speed;
	actor->SetRotation(rot);
	obstacle->SetRotSpeed(speed);

	Capsule* cap = actor->GetCapsule();

	cap->a = DirectX::SimpleMath::Vector3(0.0f, position.y, 0.0f);
	cap->b = DirectX::SimpleMath::Vector3(cos(rot.y) * 15, position.y, -sin(rot.y) * 15);
	cap->r = 1.0f;
	if (position.y == 0.0f)
	{


	}

	/*if (position.y != 0.0f)
	{
		actor->SetPosition(position + (velocity * timer.GetElapsedSeconds()));

	}

	if (position.y >= 0.0f)
	{
		actor->SetPosition(DirectX::SimpleMath::Vector3(position.x, 0.0f, position.z));
	}*/


}

void MeteoriteBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{

	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();

	AABBFor3D* aabb = actor->GetAABB();

	//aabb->SetData(DirectX::SimpleMath::Vector3(), DirectX::SimpleMath::Vector3());

	actor->SetPosition(position + (velocity * timer.GetElapsedSeconds()));

}

void MeanderingeBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	const float MOVE_SPEED = 1.0f;
	float elapsedTime = timer.GetElapsedSeconds();
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();
	DirectX::SimpleMath::Vector3 rot = actor->GetRotation();
	float angle = actor->GetAngle();
	actor->SetVelocity(DirectX::SimpleMath::Vector3(1, 0, 1));
	Obstacle* obstacle = dynamic_cast<Obstacle*>(actor);
	
	actor->GetAABB()->SetData(DirectX::SimpleMath::Vector3(position.x - 0.3f, position.y - 0.5f, position.z - 0.3f), DirectX::SimpleMath::Vector3(position.x + 0.3f, position.y + 0.5f, position.z + 0.3f));
	DirectX::SimpleMath::Vector3 seekVlocity = obstacle->Seek(obstacle->GetPlayerPosition());
	DirectX::SimpleMath::Vector3 wanderVelocity = obstacle->Wander();
	
	seekVlocity *= elapsedTime;
	wanderVelocity *= 0.1;
	actor->SetPosition(DirectX::SimpleMath::Vector3(position.x+ seekVlocity.x+ wanderVelocity.x, position.y, position.z+ seekVlocity.z + wanderVelocity.z));

}

void StraightMoveBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();
	AABBFor3D* aabb = actor->GetAABB();

	aabb->SetData(DirectX::SimpleMath::Vector3(position.x - 0.5f, position.y - 0.5f, position.z - 0.5f), DirectX::SimpleMath::Vector3(position.x + 0.5f, position.y + 0.5f, position.z + 0.5f));

	actor->SetPosition(position + (velocity * timer.GetElapsedSeconds()));
}

void BirdBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();
	float angle = actor->GetAngle();
	

	AABBFor3D* aabb = actor->GetAABB();

	//aabb->SetData(DirectX::SimpleMath::Vector3(position.x - 0.5f, position.y - 0.0f, position.z - 0.5f), DirectX::SimpleMath::Vector3(position.x + 0.5f, position.y + 1.5f, position.z + 0.5f));


	if (angle == 0.0f || angle == DirectX::XM_PI)
	{
		aabb->SetData(DirectX::SimpleMath::Vector3(position.x - 0.5f, position.y - 0.5f, position.z - 0.75f), DirectX::SimpleMath::Vector3(position.x + 0.5f, position.y + 0.5f, position.z + 0.75f));

	}
	else if (angle == DirectX::XM_PI / 2.0f || angle == DirectX::XM_PI + (DirectX::XM_PI / 2.0f))
	{
		aabb->SetData(DirectX::SimpleMath::Vector3(position.x - 0.5f, position.y - 0.5f, position.z - 0.4f), DirectX::SimpleMath::Vector3(position.x + 0.5f, position.y + 0.5f, position.z + 0.4f));

	}
	


	actor->SetPosition(position + (velocity * timer.GetElapsedSeconds()));

}

void ReverseRotateStickBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();

	DirectX::SimpleMath::Matrix world = actor->GetWorld();
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0, 0, 0));
	actor->SetWorld(world);



	DirectX::SimpleMath::Vector3 rot = actor->GetRotation();
	Obstacle* obstacle = dynamic_cast<Obstacle*>(actor);
	float speed = obstacle->GetRotSpeed();
	speed -= 0.000001f;

	//if(speed)

	rot.y += speed;
	actor->SetRotation(rot);
	obstacle->SetRotSpeed(speed);

	Capsule* cap = actor->GetCapsule();

	cap->a = DirectX::SimpleMath::Vector3(0.0f, position.y, 0.0f);
	cap->b = DirectX::SimpleMath::Vector3(cos(rot.y) * 15, position.y, -sin(rot.y) * 15);
	cap->r = 1.0f;
	if (position.y == 0.0f)
	{


	}
}
