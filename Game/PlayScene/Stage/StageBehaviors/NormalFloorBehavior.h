/*
* 2023/03/27
* NormalFloorBehavior.h
* 普通の床の動き
* 麻生　楓
*/
#pragma once
#include"Game/PlayScene/IBehavior.h"


/// <summary>
/// 普通の床の動き
/// </summary>
class NormalFloorBehavior : public IBehavior
{
public:
	NormalFloorBehavior() = default;
	~NormalFloorBehavior()override = default;

	/// <summary>
	/// 普通の床の動き
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <param name="actor">ステージのポインター</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};