/*
* 2023/03/27
* NormalFloorBehavior.cpp
* 普通の床の動き
* 麻生　楓
*/
#include"pch.h"
#include"NormalFloorBehavior.h"
#include"../Stage.h"
#include"Game/PlayScene/MyRandom.h"

/// <summary>
/// 普通の床の動き
/// </summary>
/// <param name="timer">タイマー</param>
/// <param name="actor">ステージのポインター</param>
void NormalFloorBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	UNREFERENCED_PARAMETER(timer);

	//ステージ型にキャスト
	Stage* stage = dynamic_cast<Stage*>(actor);

	//行動をしないため行動を終了
	stage->SetMoveEndFlag(true);
	
}