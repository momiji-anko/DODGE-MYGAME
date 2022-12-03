#pragma once

#include <SimpleMath.h>

//	�J�v�Z���̍\����
struct Capsule
{
	DirectX::SimpleMath::Vector3 a;		//	�c�̐����̊J�n�_
	DirectX::SimpleMath::Vector3 b;		//	�c�̐����̏I���_
	float r;							//	���a
};

//���̍\����
struct Sphere
{
	DirectX::SimpleMath::Vector3 pos;
	float r;
};