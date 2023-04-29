/*
* 2023/04/21
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


	//当たり判定AABB取得
	AABBFor3D* aabb = actor->GetAABB();

	//当たり判定の領域
	DirectX::SimpleMath::Vector3 AABBArea = { 0.5f,0.5f,0.75f };
	aabb->SetData(position - AABBArea, position + AABBArea);

	//経過時間
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	//移動
	actor->SetPosition(position + (velocity * elapsedTime_s));

}
