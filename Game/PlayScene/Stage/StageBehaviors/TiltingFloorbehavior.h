/*
* 2023/04/07
* TiltingFloorbehavior.h
* ステージ２の傾く床
* 麻生　楓
*/
#pragma once
#include"Game/PlayScene/IBehavior.h"

/// <summary>
/// ステージ２の傾く床
/// </summary>
class TiltingFloorbehavior : public IBehavior
{
public:
	TiltingFloorbehavior() = default;
	~TiltingFloorbehavior()override = default;

	/// <summary>
	/// ステージ２の傾く床の動き
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <param name="actor">ステージのポインター</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};