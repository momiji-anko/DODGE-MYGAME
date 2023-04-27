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
	
	//当たり判定の領域
	DirectX::SimpleMath::Vector3 AABBArea{ 0.3f,0.5f,0.3f };


	//当たり判定AABBの当たり判定領域の設定
	actor->GetAABB()->SetData(position - AABBArea, position + AABBArea);

	//経過時間
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	//移動する
	actor->SetPosition(position + (velocity * elapsedTime_s));

	//Actor型をObstacle型にダイナミックキャストする
	Obstacle* obs = dynamic_cast<Obstacle*>(actor);
	//キャストに成功していればエフェクトを更新する
	if (obs != nullptr&& obs->GetEffect() != nullptr)
	{
		//エフェクト更新
		obs->GetEffect()->Update(timer);
	}
	

}



