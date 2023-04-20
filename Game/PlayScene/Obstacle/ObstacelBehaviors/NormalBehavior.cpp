/*
* 2023/03/21
* NormalBehavior.cpp
* 麻生　楓
* 普通の炎の動き
*/
#include"pch.h"
#include"../../Actor/Actor.h"
#include"../Obstacle.h"
#include"NormalBehavior.h"

/// <summary>
/// 普通の炎の動き
/// </summary>
/// <param name="timer">タイマー</param>
/// <param name="actor">障害物のポインター（this）</param>
void NormalBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	//座標の取得
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	//移動量の取得
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();
	
	//当たり判定AABBの当たり判定領域の設定
	actor->GetAABB()->SetData(DirectX::SimpleMath::Vector3(position.x - 0.3f, position.y - 0.5f, position.z - 0.3f), DirectX::SimpleMath::Vector3(position.x + 0.3f, position.y + 0.5f, position.z + 0.3f));

	//移動する
	actor->SetPosition(position + (velocity * timer.GetElapsedSeconds()));

	//Actor型をObstacle型にダイナミックキャストする
	Obstacle* obs = dynamic_cast<Obstacle*>(actor);
	//キャストに成功していればエフェクトを更新する
	if (obs != nullptr&& obs->GetEffect() != nullptr)
	{
		//エフェクト更新
		obs->GetEffect()->Update(timer);
	}
	

}



