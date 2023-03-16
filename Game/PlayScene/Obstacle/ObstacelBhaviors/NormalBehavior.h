#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>

class NormalBehavior :public IBehavior
{
public:
	NormalBehavior() = default;
	~NormalBehavior()override = default;

	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};