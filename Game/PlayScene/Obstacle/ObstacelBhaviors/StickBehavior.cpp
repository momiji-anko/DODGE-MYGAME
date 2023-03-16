#include"pch.h"
#include"../../Actor/Actor.h"
#include"../Obstacle.h"
#include"StickBehavior.h"


void StickBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();

	//障害物からAABB取得
	AABBFor3D* aabb = actor->GetAABB();
	//アングル取得
	float angle = actor->GetAngle();
	//アングルによってAABBの当たり判定の領域を変更する
	if (angle == 0.0f || angle == DirectX::XM_PI)
	{
		aabb->SetData(DirectX::SimpleMath::Vector3(position.x - 0.5f, position.y - 0.0f, position.z - 12.0f), DirectX::SimpleMath::Vector3(position.x + 0.5f, position.y + 1.5f, position.z + 12.0f));

	}
	else if (angle == DirectX::XM_PI / 2.0f || angle == DirectX::XM_PI + (DirectX::XM_PI / 2.0f))
	{
		aabb->SetData(DirectX::SimpleMath::Vector3(position.x - 12.0f, position.y - 0.0f, position.z - 0.4f), DirectX::SimpleMath::Vector3(position.x + 12.0f, position.y + 1.0f, position.z + 0.4f));

	}
	//移動（座標＋（移動ベクトル＊1フレームの秒数））
	actor->SetPosition(position + (velocity * timer.GetElapsedSeconds()));

}