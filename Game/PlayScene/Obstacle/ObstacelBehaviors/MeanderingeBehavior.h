/*
* 2023/03/21
* MeanderingBehavior.h
* 麻生　楓
*/
#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>

/// <summary>
/// 蛇行する炎の障害物のビヘイビアー
/// </summary>
class MeanderingBehavior :public IBehavior
{
public:
	//コンストラクタ
	MeanderingBehavior() = default;
	//デストラクタ
	~MeanderingBehavior()override = default;

	/// <summary>
	/// 蛇行する炎の固有の行動
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <param name="actor">障害物のポインター（this）</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};