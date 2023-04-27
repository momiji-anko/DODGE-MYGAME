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

	//当たり判定の領域
	DirectX::SimpleMath::Vector3 AABBArea{ 0.3f,0.5f,0.3f };


	//当たり判定の領域設定
	actor->GetAABB()->SetData(position - AABBArea, position + AABBArea);
	
	//経過時間
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	//移動する
	actor->SetPosition(position + (velocity * elapsedTime_s));
}
