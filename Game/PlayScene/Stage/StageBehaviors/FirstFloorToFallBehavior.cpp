/*
* 2023/03/27
* FirstFloorToFallBhavior.cpp
* 麻生　楓
* 一番目に落ちる床の動き
*/
#include"pch.h"
#include"FirstFloorToFallBehavior.h"
#include"../Stage.h"
#include"Game/PlayScene/MyRandom.h"

/// <summary>
/// 一番目に落ちる床の動き
/// </summary>
/// <param name="timer">タイマー</param>
/// <param name="actor">ステージのポインター</param>
void FirstFloorToFallBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	float elapsedTime_s = timer.GetElapsedSeconds();

	Stage* stage = dynamic_cast<Stage*>(actor);

	static const float MOVE_TIME_S = 5.0f;
	
	float time_s = stage->GetTime();
	

	int routine = stage->GetRoutine();

	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	DirectX::SimpleMath::Vector3 offsetPosition = stage->GetOffsetPosition();
	

	time_s += elapsedTime_s;


	if (time_s > MOVE_TIME_S)
	{
		time_s = MOVE_TIME_S;
		routine++;
	}


	switch (routine)
	{
	case 0:
		break;
	case 1:
		time_s = 0;
		routine++;
	case 2:


		break;
	case 3:
		time_s = 0;
		routine++;
	case 4:
		
		offsetPosition.x = MyRandom::GetFloatRange(-0.2f, 0.2f);
		offsetPosition.y = MyRandom::GetFloatRange(-0.2f, 0.2f);
		offsetPosition.z = MyRandom::GetFloatRange(-0.2f, 0.2f);

		break;
	case 5:
		time_s = 0;
		routine++;
	case 6:
	

		position.y = stage->Lerp(-1.0f, -100.0f, time_s / MOVE_TIME_S);

		offsetPosition.x = 0.0f;
		offsetPosition.y = 0.0f;
		offsetPosition.z = 0.0f;

		break;
	case 7:
		time_s = 0;
		routine++;
	case 8:
	
		position.y = -1.0f;

		if (position.x < 0)
		{
			position.x = -100.0f;
		}
		else
		{
			position.x = 100.0f;

		}
		break;
	case 9:
		time_s = 0;
		routine++;
	case 10:
		
		break;
	case 11:
		time_s = 0;
		routine++;
	case 12:
	

		break;
	case 13:

		time_s = 0;
		routine++;
	case 14:
	


	case 15:
		time_s = 0;
		routine++;
	case 16:

		break;
	case 17:
		time_s = 0;
		routine++;

	case 18:

		break;
	case 19:
		time_s = 0;
		routine++;
	case 20:

		break;
	case 21:
		time_s = 0;
		routine++;
	case 22:

		break;
	case 23:
		time_s = 0;
		routine++;
	case 24:

		break;
	case 25:
		time_s = 0;
		routine++;
	case 26:
		
		if (position.x < 0)
		{
			position.x = stage->Lerp(-100.0f, -6.0f, time_s / MOVE_TIME_S);
		}
		else
		{
			position.x = stage->Lerp(100.0f, 6.0f, time_s / MOVE_TIME_S);
		}

		break;
	}


	stage->SetPosition(position);
	stage->SetOffsetPosition(offsetPosition);
	stage->SetRoutine(routine);
	stage->SetTime(time_s);
}
