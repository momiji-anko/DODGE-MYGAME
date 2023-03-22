/*
* 2023/03/21
* MeteoriteBehavior.h
* 麻生　楓
*/
#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>

/// <summary>
/// 隕石の障害物のビヘイビアー
/// </summary>
class MeteoriteBehavior :public IBehavior
{
public:
	//コンストラクタ
	MeteoriteBehavior() = default;
	//デストラクタ
	~MeteoriteBehavior()override = default;
	/// <summary>
	/// 隕石の動き
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <param name="actor">障害物のポインター（this）</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};