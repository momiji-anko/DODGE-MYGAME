/*
* 2023/03/27
* FirstFloorToFallBhavior.h
* 麻生　楓
*/
#pragma once
#include"Game/PlayScene/IBehavior.h"

/// <summary>
/// 一番目に落ちる床の動き
/// </summary>
class FirstFloorToFallBhavior : public IBehavior
{
	FirstFloorToFallBhavior() = default;
	~FirstFloorToFallBhavior()override = default;

	/// <summary>
	/// 一番目に落ちる床の動き
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <param name="actor">ステージのポインター</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};