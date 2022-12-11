//
//MyRandom.cpp
//
#include"pch.h"
#include"MyRandom.h"

std::random_device MyRandom::s_rd;
std::mt19937 MyRandom::s_mt;

//===================================
//初期化
//===================================
void MyRandom::Initialize()
{
	MyRandom::Seed(s_rd());
}

//===================================
//シード値を更新
//===================================
void MyRandom::Seed(unsigned int seed)
{
	s_mt.seed(seed);
}

//===================================
//floatの値を取得する
//===================================
float MyRandom::GetFloat()
{
	return GetFloatRange(0.0f, 1.0f);
}

//===================================
//範囲を指定して、float値を取得する
//===================================
float MyRandom::GetFloatRange(float min, float max)
{
	assert(min < max);
	std::uniform_real_distribution<float> urd(min, max);
	return urd(s_mt);
}

//===================================
//範囲を指定して、int値を取得する
//===================================
int MyRandom::GetIntRange(int min, int max)
{
	assert(min <= max);

	std::uniform_int_distribution<int> urd(min, max);
	return urd(s_mt);
}

//===================================
//範囲を指定して、Vector2値を取得する
//===================================
DirectX::SimpleMath::Vector2 MyRandom::GetVector(const DirectX::SimpleMath::Vector2& min, const DirectX::SimpleMath::Vector2& max)
{
	DirectX::SimpleMath::Vector2 r = DirectX::SimpleMath::Vector2(GetFloat(), GetFloat());
	return min + (max - min) * r;
}
