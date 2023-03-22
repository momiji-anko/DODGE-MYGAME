/*
* 2023/03/21
* RotateStickBehavior.cpp
* 麻生　楓
* 回転する棒の動き
*/
#include"pch.h"
#include"../../Actor/Actor.h"
#include"../Obstacle.h"
#include"RotateStickBehavior.h"

/// <summary>
/// 回転する棒の動き
/// </summary>
/// <param name="timer">タイマー</param>
/// <param name="actor">障害物のポインター（this）</param>
void RotateStickBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	//座標の取得
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();

	//ワールド行列の取得
	DirectX::SimpleMath::Matrix world = actor->GetWorld();
	//ワールド行列の角度を０にする
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0, 0, 0));
	//ワールド行列の設定
	actor->SetWorld(world);

	//アクター型から障害物型にダイナミックキャスト
	Obstacle* obstacle = dynamic_cast<Obstacle*>(actor);

	//角度の取得
	DirectX::SimpleMath::Vector3 rot = actor->GetRotation();
	//回転速度の取得
	float speed = obstacle->GetRotSpeed();
	//回転速度の上げる
	speed += 0.000001f;

	//角度に回転速度を足す
	rot.y += speed;
	//角度の設定
	actor->SetRotation(rot);
	//回転速度の設定
	obstacle->SetRotSpeed(speed);
	//当たり判定カプセルの取得
	Capsule* cap = actor->GetCapsule();
	//判定の領域の設定
	cap->a = DirectX::SimpleMath::Vector3(0.0f, position.y, 0.0f);
	//現在の角度で計算する
	cap->b = DirectX::SimpleMath::Vector3(cos(rot.y) * 15, position.y, -sin(rot.y) * 15);
	//半径
	cap->r = 1.0f;

}