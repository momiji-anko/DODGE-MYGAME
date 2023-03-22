/*
* 2023/03/21
* MeteoriteBehavior.cpp
* 麻生　楓
* 隕石の動き
*/
#include"pch.h"
#include"../../Actor/Actor.h"
#include"../Obstacle.h"
#include"MeteoriteBehavior.h"

/// <summary>
/// 隕石の動き
/// </summary>
/// <param name="timer">タイマー</param>
/// <param name="actor">障害物のポインター（this）</param>
void MeteoriteBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	//座標の取得
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	//移動量の取得
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();
	
	//地面に着いたら消す
	if (position.y <= -1.0f)
	{
		actor->SetActive(false);
	}

	//当たり判定AABB取得
	AABBFor3D* aabb = actor->GetAABB();
	//当たり判定の領域設定
	actor->GetAABB()->SetData(DirectX::SimpleMath::Vector3(position.x - 0.3f, position.y - 0.5f, position.z - 0.3f), DirectX::SimpleMath::Vector3(position.x + 0.3f, position.y + 0.5f, position.z + 0.3f));
	
	//移動する
	actor->SetPosition(position + (velocity * timer.GetElapsedSeconds()));
}
