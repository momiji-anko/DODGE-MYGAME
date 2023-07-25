/*
* 2023/03/27
* ThirdFloorToFallBhavior.cpp
* 三番目に落ちる床の動き
* 麻生　楓
*/
#include"pch.h"
#include"ThirdFloorToFallBehavior.h"
#include"../Stage.h"
#include"Game/PlayScene/MyRandom.h"

/// <summary>
/// 三番目に落ちる床の動き
/// </summary>
/// <param name="timer">タイマー</param>
/// <param name="actor">ステージのポインター</param>
void ThirdFloorToFallBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	//ステージの揺れ
	static const float STAGE_VIBRATION = 0.2f;
	//落ちる床のY座標開始位置
	static const float START_FALL_POSITION_Y = -1.0f;
	//落ちる床のY座標最終位置
	static const float END_FALL_POSITION_Y = -100.0f;
	//帰ってくる床のX座標開始位置
	static const float END_RETURN_POSITION_X = 6.0f;
	//帰ってくる床のX座標終了位置
	static const float START_RETURN_POSITION_X = 100.0f;
	//移動時間
	static const float MOVE_TIME_S = 5.0f;

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


	//ルーチンによって行動を変える
	switch (routine)
	{
	case 0:
	
		break;
	case 1:
		//タイマーの初期化とルーチンを進める
		time_s = 0;
		routine++;
	case 2:

		break;
	case 3:
		//タイマーの初期化とルーチンを進める
		time_s = 0;
		routine++;
	case 4:
	
		break;
	case 5:
		//タイマーの初期化とルーチンを進める
		time_s = 0;
		routine++;
	case 6:

		break;
	case 7:
		//タイマーの初期化とルーチンを進める
		time_s = 0;
		routine++;
	case 8:
	
		break;
	case 9:
		//タイマーの初期化とルーチンを進める
		time_s = 0;
		routine++;
	case 10:

		break;

	case 11:
		//タイマーの初期化とルーチンを進める
		time_s = 0;
		routine++;
	case 12:

		break;
	case 13:
		//タイマーの初期化とルーチンを進める
		time_s = 0;
		routine++;
	case 14:



		break;
	case 15:
		//タイマーの初期化とルーチンを進める
		time_s = 0;
		routine++;
	case 16:


		break;
	case 17:
		//タイマーの初期化とルーチンを進める
		time_s = 0;
		routine++;

	case 18:

		//床を揺らす
		offsetPosition.x = MyRandom::GetFloatRange(-STAGE_VIBRATION, STAGE_VIBRATION);
		offsetPosition.y = MyRandom::GetFloatRange(-STAGE_VIBRATION, STAGE_VIBRATION);
		offsetPosition.z = MyRandom::GetFloatRange(-STAGE_VIBRATION, STAGE_VIBRATION);

		break;
	case 19:
		//タイマーの初期化とルーチンを進める
		time_s = 0;
		routine++;
	case 20:

		//床を落とす	
		position.y = stage->Lerp(START_FALL_POSITION_Y, END_FALL_POSITION_Y, time_s / MOVE_TIME_S);

		//揺れを止める
		offsetPosition = DirectX::SimpleMath::Vector3::Zero;

		break;
	case 21:
		//タイマーの初期化とルーチンを進める
		time_s = 0;
		routine++;
	case 22:


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

		break;
	case 23:
		//タイマーの初期化とルーチンを進める
		time_s = 0;
		routine++;
	case 24:


		break;
	case 25:
		//タイマーの初期化とルーチンを進める
		time_s = 0;
		routine++;
	case 26:

		//床を元の位置に戻る
		if (position.x < 0)
		{
			position.x = stage->Lerp(-START_RETURN_POSITION_X, -END_RETURN_POSITION_X, time_s / MOVE_TIME_S);
		}
		else
		{
			position.x = stage->Lerp(START_RETURN_POSITION_X, END_RETURN_POSITION_X, time_s / MOVE_TIME_S);
		}

		break;
	case 27:
		//行動を終了
		stage->SetMoveEndFlag(true);
		break;
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