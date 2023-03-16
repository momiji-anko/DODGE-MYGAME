#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>

class StickBehavior :public IBehavior
{
public:
	StickBehavior() = default;
	~StickBehavior()override = default;

	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};
