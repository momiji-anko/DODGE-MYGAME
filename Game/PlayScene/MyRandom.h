//
//MyRandom.h
//
#pragma once
#include<random>
#include<SimpleMath.h>
class MyRandom
{
private:
	static std::random_device s_rd;
	static std::mt19937 s_mt;

public:	
	MyRandom() = default;
	~MyRandom() = default;

	static void Initialize();
	static void Seed(unsigned int seed);
	static float GetFloat();
	static float GetFloatRange(float min, float max);
	static int GetIntRange(int min, int max);
	static DirectX::SimpleMath::Vector2 GetVector(
		const DirectX::SimpleMath::Vector2& min,
		const DirectX::SimpleMath::Vector2& max
	);
};