#include"pch.h"
#include"../../Actor/Actor.h"
#include"../Obstacle.h"
#include"MeteoriteBehavior.h"


void MeteoriteBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{

	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();

	if (position.y <= -1.0f)
	{
		actor->SetActive(false);
	}
	AABBFor3D* aabb = actor->GetAABB();
	actor->GetAABB()->SetData(DirectX::SimpleMath::Vector3(position.x - 0.3f, position.y - 0.5f, position.z - 0.3f), DirectX::SimpleMath::Vector3(position.x + 0.3f, position.y + 0.5f, position.z + 0.3f));

	actor->SetPosition(position + (velocity * timer.GetElapsedSeconds()));
}
