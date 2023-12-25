/*
* 202323/12/06
* FloorToFallBehavior.h
* 落ちる床の動き
* 麻生　楓
*/
#pragma once
#include"BaseFloorToFallBehavior.h"
#include<SimpleMath.h>
#include<vector>

//前方宣言
class Stage;

/// <summary>
/// 落ちる床の動き
/// </summary>
class FloorToFallBehavior : public BaseFloorToFallBehavior
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="numFallFloor">何番目に落ちる床か</param>
	FloorToFallBehavior(int numFallFloor);
	//デストラクタ
	~FloorToFallBehavior()override = default;

	/// <summary>
	/// 落ちる床の動き
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <param name="actor">ステージのポインター</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;

private:
	int m_numFallFloor;

};