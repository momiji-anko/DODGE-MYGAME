/*
* 2023/04/11
* IBehavior.h
* ビヘイビアーのインターフェースクラス
* 麻生　楓
*/
#pragma once
#include"StepTimer.h"
#include<vector>

//前方宣言
class Actor;

/// <summary>
/// ビヘイビアーのインターフェース
/// </summary>
class IBehavior
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual~IBehavior() = default;

	/// <summary>
	/// 動く
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <param name="actor">アクター</param>
	virtual void Execute(const DX::StepTimer& timer, Actor* actor) = 0;

};