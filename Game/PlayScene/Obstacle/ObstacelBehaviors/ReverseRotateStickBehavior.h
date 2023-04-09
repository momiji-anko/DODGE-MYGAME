/*
* 2023/03/21
* ReverseRotateStickBehavior.h
* 麻生　楓
*/
#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>

/// <summary>
/// 反時計回りに回転する棒のビヘイビアー
/// </summary>
class ReverseRotateStickBehavior :public IBehavior
{
public:
	//コンストラクタ
	ReverseRotateStickBehavior() = default;
	//デストラクタ
	~ReverseRotateStickBehavior()override = default;

	/// <summary>
	/// 反時計回りに回転する棒の動き
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <param name="actor">障害物のポインター（this）</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};