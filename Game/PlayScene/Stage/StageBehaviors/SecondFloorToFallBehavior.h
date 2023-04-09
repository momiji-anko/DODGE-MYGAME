/*
* 2023/03/27
* SecondFloorToFallBhavior.h
* 麻生　楓
*/
#pragma once
#include"Game/PlayScene/IBehavior.h"

/// <summary>
/// 二番目に落ちる床の動き
/// </summary>
class SecondFloorToFallBehavior : public IBehavior
{
public:
	SecondFloorToFallBehavior() = default;
	~SecondFloorToFallBehavior()override = default;

	/// <summary>
	/// 二番目に落ちる床の動き
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <param name="actor">ステージのポインター</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};