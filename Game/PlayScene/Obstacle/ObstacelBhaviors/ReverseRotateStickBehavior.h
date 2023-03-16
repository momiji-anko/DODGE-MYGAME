#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>

class ReverseRotateStickBehavior :public IBehavior
{
public:
	ReverseRotateStickBehavior() = default;
	~ReverseRotateStickBehavior()override = default;

	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};