/*
* 2023/03/21
* MeanderingBehavior.cpp
* 麻生　楓
* 蛇行する炎のビヘイビアークラス
*/
#include"pch.h"
#include"../../Actor/Actor.h"
#include"../Obstacle.h"
#include"MeanderingeBehavior.h"


/// <summary>
/// 蛇行する炎の固有の行動
/// </summary>
/// <param name="timer">タイマー</param>
/// <param name="actor">障害物のポインター（this）</param>
void MeanderingBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	//移動速度
	const float MOVE_SPEED = 4.0f;
	//経過時間
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	//座標の取得
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	//移動量の取得
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();

	//アクター型から障害物型にダイナミックキャスト
	Obstacle* obstacle = dynamic_cast<Obstacle*>(actor);

	//NULLチェック
	if (obstacle == nullptr)
	{
		return;
	}
	//エフェクト更新
	obstacle->GetEffect()->Update(timer);

	//蛇行タイム取得
	float meandelingTime_s = obstacle->GetTime();
	
	//蛇行タイムに経過時間を足す
	meandelingTime_s += elapsedTime;

	//蛇行ベロシティ
	DirectX::SimpleMath::Vector3 meanderingVelocity{ velocity.z,0,-velocity.x };

	//２で割ったあまりが０であればベクトルを反転
	if (static_cast<int>(meandelingTime_s) % 2 != 0)
	{
		meanderingVelocity *= -1;
	}

	//当たり判定の領域
	DirectX::SimpleMath::Vector3 AABBArea{ 0.3f,0.5f,0.3f };

	//当たり判定AABBの当たり判定領域の設定
	actor->GetAABB()->SetData(position - AABBArea, position + AABBArea);

	//蛇行ベロシティの影響力を強めるためをMOVE_SPEEDでかける
	meanderingVelocity *= MOVE_SPEED;
	//ベロシティに蛇行ベロシティを足す
	velocity += meanderingVelocity;
	//ベロシティを正規化
	velocity.Normalize();
	//ベロシティの移動量計算
	velocity *= MOVE_SPEED;
	//ベロシティに経過時間をかける
	velocity *= elapsedTime;

	//移動する
	actor->SetPosition(position + velocity);

	obstacle->SetTime(meandelingTime_s);

}