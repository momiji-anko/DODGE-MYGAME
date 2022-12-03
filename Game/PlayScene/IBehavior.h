#pragma once
#include"StepTimer.h"
#include<vector>

class Actor;
class IBehavior
{
public:
	IBehavior() = default;
	virtual~IBehavior() = default;

	virtual void Execute(const DX::StepTimer& timer, Actor* actor) = 0;

};