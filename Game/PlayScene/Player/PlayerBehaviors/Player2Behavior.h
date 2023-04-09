/*
* 
*/
#pragma once
#include"Game/PlayScene/IBehavior.h"


class Player2Behavior :public IBehavior
{
	//コンストラクタ
	Player2Behavior() = default;
	//デストラクタ
	~Player2Behavior()override = default;

	/// <summary>
	/// プレイヤー２の動き
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <param name="actor">プレイヤーのポインター（this）</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};