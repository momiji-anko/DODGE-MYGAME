#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>

class RotateStickBehavior :public IBehavior
{
public:
	RotateStickBehavior() = default;
	~RotateStickBehavior()override = default;

	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};
