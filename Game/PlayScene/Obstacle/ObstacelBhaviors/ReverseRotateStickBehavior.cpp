#include"pch.h"
#include"../../Actor/Actor.h"
#include"../Obstacle.h"
#include"ReverseRotateStickBehavior.h"


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