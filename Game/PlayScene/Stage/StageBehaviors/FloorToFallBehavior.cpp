/*
* 2023/12/06
* FloorToFallBehavior.h
* 落ちる床の動き
* 麻生　楓
*/
#include"pch.h"
#include"FloorToFallBehavior.h"
#include"../Stage.h"
#include"Game/PlayScene/MyRandom.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/Json/json.hpp"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="numFallFloor">何番目に落ちる床か</param>
FloorToFallBehavior::FloorToFallBehavior(int numFallFloor)
	:
	m_numFallFloor(numFallFloor - 1)
{
}

/// <summary>
/// 落ちる床の動き
/// </summary>
/// <param name="timer">タイマー</param>
/// <param name="actor">ステージのポインター</param>
void FloorToFallBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	//経過時間
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	//ステージ型にキャスト
	Stage* stage = dynamic_cast<Stage*>(actor);

	//stageがnullptrであれば実行しない
	if (stage == nullptr)
	{
		return;
	}

	//タイムの取得
	float time_s = stage->GetTime();

	//ルーチンの取得　
	int routine = stage->GetRoutine();

	//座標取得
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	//オフセット座標取得
	DirectX::SimpleMath::Vector3 offsetPosition = stage->GetOffsetPosition();

	//タイムを経過時間で足す
	time_s += elapsedTime_s;

	//行動読み込み
	const std::vector<int>& moveFunctionList = GetMoveBehaviorList(m_numFallFloor);

	//ルーチンが行動の数以上になれば行動を終了する
	if (routine >= static_cast<int>(moveFunctionList.size() - 1))
	{
		//行動を終了
		stage->SetMoveEndFlag(true);
	}

	//nullでなない ＆ ルーチンが動き配列以下 ＆ 行動終了していない場合実行する
	if (ROUTINE_FUNCTION_LIST[moveFunctionList[routine]] != nullptr && routine <= moveFunctionList.size() && !stage->GetMoveEndFlag())
	{
		ROUTINE_FUNCTION_LIST[moveFunctionList[routine]](routine, time_s, position, offsetPosition, stage);
	}

	//移動時間を超えたらルーチンを進める
	if (time_s > MOVE_TIME_S)
	{
		time_s = MOVE_TIME_S;
		routine++;
	}

	//座標の設定
	stage->SetPosition(position);
	//オフセット座標の設定
	stage->SetOffsetPosition(offsetPosition);
	//ルーチンの設定
	stage->SetRoutine(routine);
	//タイムの設定
	stage->SetTime(time_s);

}


