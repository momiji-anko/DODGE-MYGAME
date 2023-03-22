/*
* 2023/03/21
* RotateStickBehavior.h
* 麻生　楓
*/
#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>

/// <summary>
/// 回転する棒のビヘイビアー
/// </summary>
class RotateStickBehavior :public IBehavior
{
public:
	//コンストラクタ
	RotateStickBehavior() = default;
	//デストラクタ
	~RotateStickBehavior()override = default;
	
	/// <summary>
	/// 回転する棒の動き
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <param name="actor">障害物のポインター（this）</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};
