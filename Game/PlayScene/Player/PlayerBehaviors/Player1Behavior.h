/*
*
*/
#pragma once
#include"Game/PlayScene/IBehavior.h"


class Player1Behavior :public IBehavior
{
	//コンストラクタ
	Player1Behavior() = default;
	//デストラクタ
	~Player1Behavior()override = default;

	/// <summary>
	/// プレイヤー1の動き
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <param name="actor">プレイヤーのポインター（this）</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};