/*
* 2023/03/27
* ThirdFloorToFallBhavior.h
* 三番目に落ちる床の動き
* 麻生　楓
*/
#pragma once
#include"Game/PlayScene/IBehavior.h"


/// <summary>
/// 三番目に落ちる床の動き
/// </summary>
class ThirdFloorToFallBehavior : public IBehavior
{
public:
	ThirdFloorToFallBehavior() = default;
	~ThirdFloorToFallBehavior()override = default;

	/// <summary>
	/// 三番目に落ちる床の動き
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <param name="actor">ステージのポインター</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};