#pragma once

#include <SimpleMath.h>

//	カプセルの構造体
struct Capsule
{
	DirectX::SimpleMath::Vector3 a;		//	芯の線分の開始点
	DirectX::SimpleMath::Vector3 b;		//	芯の線分の終了点
	float r;							//	半径
};

//球の構造体
struct Sphere
{
	DirectX::SimpleMath::Vector3 pos;
	float r;
};