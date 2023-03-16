#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>


class BirdBehavior :public IBehavior
{
public:
	BirdBehavior() = default;
	~BirdBehavior()override = default;

	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};