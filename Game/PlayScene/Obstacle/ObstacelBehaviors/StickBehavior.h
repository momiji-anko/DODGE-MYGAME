/*
* 2023/03/21
* StickBehavior.h
* 麻生　楓
*/
#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>

/// <summary>
/// 棒の障害物のビヘイビアー
/// </summary>
class StickBehavior :public IBehavior
{
public:
	//コンストラクタ
	StickBehavior()= default;
	//デストラクタ
	~StickBehavior()override = default;

	/// <summary>
	/// 棒の動き
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <param name="actor">障害物のポインター（this）</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};
