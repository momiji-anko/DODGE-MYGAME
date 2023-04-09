/*
* 2023/03/21
* NormalBehavior.h
* 麻生　楓
*/
#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>

/// <summary>
/// 普通の炎のビヘイビアー
/// </summary>
class NormalBehavior :public IBehavior
{
public:
	//コンストラクタ
	NormalBehavior() = default;
	//デストラクタ
	~NormalBehavior()override = default;
	
	/// <summary>
	/// 普通の炎の動き
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <param name="actor">障害物のポインター（this）</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};