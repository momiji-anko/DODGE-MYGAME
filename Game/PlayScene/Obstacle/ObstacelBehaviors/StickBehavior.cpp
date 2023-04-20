/*
* 2023/03/21
* StickBehavior.cpp
* 麻生　楓
* 棒の動き
*/
#include"pch.h"
#include"../../Actor/Actor.h"
#include"../Obstacle.h"
#include"StickBehavior.h"

/// <summary>
/// 棒の動き
/// </summary>
/// <param name="timer">タイマー</param>
/// <param name="actor">障害物のポインター（this）</param>
void StickBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	//座標の取得
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	//移動量の取得
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();

	//当たり判定AABBの取得
	AABBFor3D* aabb = actor->GetAABB();
	//角度の取得
	float angle = std::abs(actor->GetRotation().ToEuler().y);
	
	float rot = std::floor(angle);

	//角度によって当たり判定の領域の設定
 	//0 OR 180であれば
	if (rot == 0.0f || rot == std::floor(DirectX::XM_PI))
	{
		aabb->SetData(DirectX::SimpleMath::Vector3(position.x - 0.5f, position.y - 0.0f, position.z - 12.0f), DirectX::SimpleMath::Vector3(position.x + 0.5f, position.y + 1.5f, position.z + 12.0f));	
	}
	//90 OR 270であれば
	else if (rot == std::floor((DirectX::XM_PI / 2.0f)) || rot == std::floor((DirectX::XM_PI + (DirectX::XM_PI / 2.0f))))
	{
		aabb->SetData(DirectX::SimpleMath::Vector3(position.x - 12.0f, position.y - 0.0f, position.z - 0.4f), DirectX::SimpleMath::Vector3(position.x + 12.0f, position.y + 1.0f, position.z + 0.4f));
	}

	//移動する
	actor->SetPosition(position + (velocity * timer.GetElapsedSeconds()));

}