/*
* 2023/03/21
* BirdBehavior.h
* 麻生　楓
*/
#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>

/// <summary>
/// 鳥のビヘイビアー
/// </summary>
class BirdBehavior :public IBehavior
{
public:
	//コンストラクタ
	BirdBehavior() = default;
	//デストラクタ
	~BirdBehavior()override = default;

	/// <summary>
	/// 鳥の障害物の動き
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <param name="actor">障害物のポインター</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};