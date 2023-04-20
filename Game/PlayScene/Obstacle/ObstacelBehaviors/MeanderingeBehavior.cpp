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
	//1フレームの経過時間
	float elapsedTime = timer.GetElapsedSeconds();
	//座標の取得
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	//移動量の取得
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();
	//角度の取得
	float angle = actor->GetRotation().y;
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
	float seekTime_s = obstacle->GetSeekTime();
	
	//蛇行ベロシティ
	DirectX::SimpleMath::Vector3 meanderingVelocity{ velocity.z,0,-velocity.x };

	//２で割ったあまりが０であればベクトルを反転
	if (static_cast<int>(seekTime_s) % 2 != 0)
	{
		meanderingVelocity *= -1;
	}

	//当たり判定AABBのエリア設定
	actor->GetAABB()->SetData(DirectX::SimpleMath::Vector3(position.x - 0.3f, position.y - 0.5f, position.z - 0.3f), DirectX::SimpleMath::Vector3(position.x + 0.3f, position.y + 0.5f, position.z + 0.3f));
	
	//蛇行ベロシティの影響力を強めるためをMOVE_SPEEDでかける
	meanderingVelocity *= MOVE_SPEED;
	//ベロシティに蛇行ベロシティを足す
	velocity += meanderingVelocity;
	//ベロシティを正規化
	velocity.Normalize();
	//ベロシティの移動量計算
	velocity *= MOVE_SPEED;
	//ベロシティに１フレームの秒数をかける
	velocity *= elapsedTime;

	//移動する
	actor->SetPosition(DirectX::SimpleMath::Vector3(position.x + velocity.x, position.y, position.z + velocity.z));

}