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

	DirectX::SimpleMath::Vector3 AABBArea;

	//角度によって当たり判定の領域の設定
 	//0 OR 180であれば
	if (rot == 0.0f || rot == std::floor(DirectX::XM_PI))
	{
		AABBArea = DirectX::SimpleMath::Vector3(0.5f, 0.3f, 12.0f);

		aabb->SetData(position - AABBArea, position + AABBArea);
	}
	//90 OR 270であれば
	else if (rot == std::floor((DirectX::XM_PI / 2.0f)) || rot == std::floor((DirectX::XM_PI + (DirectX::XM_PI / 2.0f))))
	{
		AABBArea = DirectX::SimpleMath::Vector3(12.0f, 0.3f, 0.5f);

		aabb->SetData(position - AABBArea, position + AABBArea);
	}

	//経過時間
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	//移動する
	actor->SetPosition(position + (velocity * elapsedTime_s));

}