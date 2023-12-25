/*
* 2023/12/01
* BaseFloorToFallBehavior.h
* 一番目に落ちる床の動き
* 麻生　楓
*/
#include"pch.h"
#include"BaseFloorToFallBehavior.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include"../Stage.h"
#include"Game/PlayScene/MyRandom.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/Json/json.hpp"

//ステージの揺れ
const float BaseFloorToFallBehavior::STAGE_VIBRATION = 0.2f;
//落ちる床のY座標開始位置
const float BaseFloorToFallBehavior::START_FALL_POSITION_Y = -1.0f;
//落ちる床のY座標最終位置
const float BaseFloorToFallBehavior::END_FALL_POSITION_Y = -100.0f;
//帰ってくる床のX座標開始位置
const float BaseFloorToFallBehavior::END_RETURN_POSITION_X = 6.0f;
//帰ってくる床のX座標終了位置
const float BaseFloorToFallBehavior::START_RETURN_POSITION_X = 100.0f;
//移動時間
const float BaseFloorToFallBehavior::MOVE_TIME_S = 5.0f;

//行動の関数ポインタ配列
 const std::vector<std::function<void(
	int& routine,
	float& timer_s,
	DirectX::SimpleMath::Vector3& position,
	DirectX::SimpleMath::Vector3& offsetPosition,
	Stage* stage)>> BaseFloorToFallBehavior::ROUTINE_FUNCTION_LIST =
 {
	//０：何もしない
	nullptr,
	//１：タイマーリセット
	TimerReset,
	//２：床を揺らす
	VibrationPosition,
	//３：床を落とす
	FallFloor,
	//４：床を戻す用の開始位置
	ReturnStartPosition,
	//５：元の場所に戻る
	ReturnPosition
 };

/// <summary>
/// 落ちる床の動き
/// </summary>
/// <param name="numFallFloor">何番目に落ちる床か</param>
/// <returns>落ちる床の動きの番号リスト</returns>
const std::vector<int>& BaseFloorToFallBehavior::GetMoveBehaviorList( int numFallFloor)
{ 
	//なにも入っていない場合はJSONから読み込む
	if (m_moveBehaviorsNum.size() == 0)
	{
		std::wstring filePath = L"Resources/StageData/Stage1.json";
		//fstream作成
		std::fstream file(filePath, std::ifstream::in);
		//json読み込み
		nlohmann::json stageJson = nlohmann::json::parse(file);
		//ファイルを閉じる
		file.close();


		m_moveBehaviorsNum.resize(stageJson["FallMoveBehavior"][numFallFloor].size());

		//行動番号読み込み
		for (int i = 0; i < stageJson["FallMoveBehavior"][numFallFloor].size(); i++)
		{
			m_moveBehaviorsNum[i] = stageJson["FallMoveBehavior"][numFallFloor][i];
		}
	}
	
	return m_moveBehaviorsNum;
}

/// <summary>
/// タイマーリセット
/// </summary>
/// <param name="routine">ルーチン</param>
/// <param name="timer_s">タイマー</param>
/// <param name="position">座標(使わない)</param>
/// <param name="offsetPosition">オフセット座標(使わない)</param>
/// <param name="stage">ステージのポインター(使わない)</param>
void BaseFloorToFallBehavior::TimerReset(
	int& routine,
	float& timer_s,
	DirectX::SimpleMath::Vector3& position,
	DirectX::SimpleMath::Vector3& offsetPosition,
	Stage* stage)
{
	//警告避け
	UNREFERENCED_PARAMETER(position);
	UNREFERENCED_PARAMETER(offsetPosition);
	UNREFERENCED_PARAMETER(stage);
	//次のルーチンにする
	routine++;
	//タイマーリセット
	timer_s = 0.0f;
}

/// <summary>
/// 床を落とす
/// </summary>
/// <param name="routine">ルーチン(使わない)</param>
/// <param name="timer_s">タイマー</param>
/// <param name="position">座標</param>
/// <param name="offsetPosition">オフセット座標</param>
/// <param name="stage">ステージのポインター</param>
void BaseFloorToFallBehavior::FallFloor(
	int& routine,
	float& timer_s,
	DirectX::SimpleMath::Vector3& position,
	DirectX::SimpleMath::Vector3& offsetPosition,
	Stage* stage)
{
	//警告避け
	UNREFERENCED_PARAMETER(routine);

	//床を落とす	
	position.y = stage->Lerp(START_FALL_POSITION_Y, END_FALL_POSITION_Y, timer_s / MOVE_TIME_S);

	//揺れを止める
	offsetPosition = DirectX::SimpleMath::Vector3::Zero;
}

/// <summary>
/// 床を元の場所に戻す
/// </summary>
/// <param name="routine">ルーチン(使わない)</param>
/// <param name="timer_s">タイマー</param>
/// <param name="position">座標</param>
/// <param name="offsetPosition">オフセット座標(使わない)</param>
/// <param name="stage">ステージのポインター</param>
void BaseFloorToFallBehavior::ReturnPosition(
	int& routine,
	float& timer_s,
	DirectX::SimpleMath::Vector3& position,
	DirectX::SimpleMath::Vector3& offsetPosition,
	Stage* stage)
{
	//警告避け
	UNREFERENCED_PARAMETER(routine);
	UNREFERENCED_PARAMETER(offsetPosition);

	//床を元の位置に戻る
	if (position.x < 0)
	{
		position.x = stage->Lerp(-START_RETURN_POSITION_X, -END_RETURN_POSITION_X, timer_s / MOVE_TIME_S);
	}
	else
	{
		position.x = stage->Lerp(START_RETURN_POSITION_X, END_RETURN_POSITION_X, timer_s / MOVE_TIME_S);
	}
}

/// <summary>
/// 床を震えさせる
/// </summary>
/// <param name="routine">ルーチン(使わない)</param>
/// <param name="timer_s">タイマー(使わない)</param>
/// <param name="position">座標(使わない)</param>
/// <param name="offsetPosition">オフセット座標</param>
/// <param name="stage">ステージのポインター(使わない)</param>
void BaseFloorToFallBehavior::VibrationPosition(
	int& routine,
	float& timer_s,
	DirectX::SimpleMath::Vector3& position,
	DirectX::SimpleMath::Vector3& offsetPosition,
	Stage* stage)
{
	//警告避け
	UNREFERENCED_PARAMETER(routine);
	UNREFERENCED_PARAMETER(timer_s);
	UNREFERENCED_PARAMETER(position);
	UNREFERENCED_PARAMETER(stage);

	//床を揺らす
	offsetPosition.x = MyRandom::GetFloatRange(-STAGE_VIBRATION, STAGE_VIBRATION);
	offsetPosition.y = MyRandom::GetFloatRange(-STAGE_VIBRATION, STAGE_VIBRATION);
	offsetPosition.z = MyRandom::GetFloatRange(-STAGE_VIBRATION, STAGE_VIBRATION);
}

/// <summary>
/// 床を戻す用の準備
/// </summary>
/// <param name="routine">ルーチン(使わない)</param>
/// <param name="timer_s">タイマー(使わない)</param>
/// <param name="position">座標</param>
/// <param name="offsetPosition">オフセット座標(使わない)</param>
/// <param name="stage">ステージのポインター(使わない)</param>
void BaseFloorToFallBehavior::ReturnStartPosition(
	int& routine,
	float& timer_s,
	DirectX::SimpleMath::Vector3& position,
	DirectX::SimpleMath::Vector3& offsetPosition,
	Stage* stage)
{
	//警告避け
	UNREFERENCED_PARAMETER(routine);
	UNREFERENCED_PARAMETER(offsetPosition);
	UNREFERENCED_PARAMETER(stage);
	UNREFERENCED_PARAMETER(timer_s);

	//スタート座標に戻す
	position.y = START_FALL_POSITION_Y;

	//Xが＋ーによって帰ってくる際の床のX座標を変える
	if (position.x < 0)
	{
		position.x = -START_RETURN_POSITION_X;
	}
	else
	{
		position.x = START_RETURN_POSITION_X;

	}
}


