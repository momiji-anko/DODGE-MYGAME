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
	//90度
	static const float NINETY_ANGLE = NumRoundDownSecond(DirectX::XM_PI / 2.0f);
	//180度
	static const float ONE_HUNDRED_EIGHTY_ANGLE = NumRoundDownSecond(DirectX::XM_PI);
	//270度
	static const float TWO_HUNDRED_SEVENTY_ANGLE = NumRoundDownSecond(NINETY_ANGLE + ONE_HUNDRED_EIGHTY_ANGLE);


	//座標の取得
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	//移動量の取得
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();
	//角度取得(デグリー)
	float angle = NumRoundDownSecond(actor->GetRotation().ToEuler().y);
	angle = std::abs(angle);

	//当たり判定AABB取得
	AABBFor3D* aabb = actor->GetAABB();


	//当たり判定の領域
	DirectX::SimpleMath::Vector3 AABBArea;

	//角度によって当たり判定の領域の設定
	//0 OR 180
	if (angle == 0.0f || angle == ONE_HUNDRED_EIGHTY_ANGLE)
	{
		AABBArea = { 0.5f,0.5f,0.75f };

		aabb->SetData(position - AABBArea, position + AABBArea);

	}
	//90 OR 270
	else if (angle == NINETY_ANGLE || angle == ONE_HUNDRED_EIGHTY_ANGLE + NINETY_ANGLE)
	{
		AABBArea = { 1.5f,1.5f,1.4f };

		aabb->SetData(position - AABBArea, position + AABBArea);
	}

	//経過時間
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	//移動
	actor->SetPosition(position + (velocity * elapsedTime_s));

}

/// <summary>
/// 小数点第三以下切り捨て
/// </summary>
/// <param name="num">切り捨てたい数字</param>
/// <returns>切り捨てた数字</returns>
float BirdBehavior::NumRoundDownSecond(float num)
{
	//数字を百倍する
	num *= 100;
	//小数点全てを切り捨て
	num = std::floor(num);
	//数字を百で割る
	num /= 100;

	return num;
}
