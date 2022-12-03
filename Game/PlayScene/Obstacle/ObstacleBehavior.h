#pragma once
#include"../IBehavior.h"
#include<SimpleMath.h>

class NormalBehavior :public IBehavior
{
public:
	NormalBehavior() = default;
	~NormalBehavior()override = default;

	 void Execute(const DX::StepTimer & timer, Actor* actor ) override;
};


class StickBehavior :public IBehavior
{
public:
	StickBehavior() = default;
	~StickBehavior()override = default;

	 void Execute(const DX::StepTimer & timer, Actor* actor ) override;
};


class RotateStickBehavior :public IBehavior
{
public:
	RotateStickBehavior() = default;
	~RotateStickBehavior()override = default;

	 void Execute(const DX::StepTimer & timer, Actor* actor ) override;
};

class ReverseRotateStickBehavior :public IBehavior
{
public:
	ReverseRotateStickBehavior() = default;
	~ReverseRotateStickBehavior()override = default;

	 void Execute(const DX::StepTimer & timer, Actor* actor ) override;
};

class MeteoriteBehavior :public IBehavior
{
public:
	MeteoriteBehavior() = default;
	~MeteoriteBehavior()override = default;

	 void Execute(const DX::StepTimer & timer, Actor* actor ) override;
};


class MeanderingeBehavior :public IBehavior
{
public:
	MeanderingeBehavior() = default;
	~MeanderingeBehavior()override = default;

	 void Execute(const DX::StepTimer & timer, Actor* actor ) override;
};


//STRAIGHT
class StraightMoveBehavior :public IBehavior
{
public:
	StraightMoveBehavior() = default;
	~StraightMoveBehavior()override = default;

	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};


class BirdBehavior :public IBehavior
{
public:
	BirdBehavior() = default;
	~BirdBehavior()override = default;

	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};

