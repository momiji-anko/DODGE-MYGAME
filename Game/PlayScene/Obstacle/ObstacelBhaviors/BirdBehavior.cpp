/*
* 2023/03/21
* BirdBehavior.cpp
* 麻生　楓
* 鳥の障害物の動き
*/

#include"pch.h"
#include"../../Actor/Actor.h"
#include"../Obstacle.h"
#include"BirdBehavior.h"

/// <summary>
/// 鳥の障害物の動き
/// </summary>
/// <param name="timer">タイマー</param>
/// <param name="actor">障害物のポインター</param>
void BirdBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	//座標の取得
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	//移動量の取得
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();
	//角度取得
	float angle = actor->GetAngle();

	//当たり判定AABB取得
	AABBFor3D* aabb = actor->GetAABB();

	//角度によって当たり判定の領域の設定
	//0 OR 180であれば
	if (angle == 0.0f || angle == DirectX::XM_PI)
	{
		aabb->SetData(DirectX::SimpleMath::Vector3(position.x - 0.5f, position.y - 0.5f, position.z - 0.75f), DirectX::SimpleMath::Vector3(position.x + 0.5f, position.y + 0.5f, position.z + 0.75f));

	}
	//90 OR 270であれば
	else if (angle == DirectX::XM_PI / 2.0f || angle == DirectX::XM_PI + (DirectX::XM_PI / 2.0f))
	{
		aabb->SetData(DirectX::SimpleMath::Vector3(position.x - 0.5f, position.y - 0.5f, position.z - 0.4f), DirectX::SimpleMath::Vector3(position.x + 0.5f, position.y + 0.5f, position.z + 0.4f));
	}

	//移動
	actor->SetPosition(position + (velocity * timer.GetElapsedSeconds()));

}