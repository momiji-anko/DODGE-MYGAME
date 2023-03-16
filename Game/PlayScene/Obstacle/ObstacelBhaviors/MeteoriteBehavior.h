#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>

class MeteoriteBehavior :public IBehavior
{
public:
	MeteoriteBehavior() = default;
	~MeteoriteBehavior()override = default;

	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};