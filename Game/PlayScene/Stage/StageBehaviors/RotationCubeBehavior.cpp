/*
* 2023/04/11
* RotationCubeBehavior.h
* 回転するキューブ
* 麻生　楓
*/
#include"pch.h"
#include"RotationCubeBehavior.h"
#include"Game/PlayScene/MyRandom.h"
#include"../Stage.h"

/// <summary>
/// 回転するキューブの動き
/// </summary>
/// <param name="timer">タイマー</param>
/// <param name="actor">ステージのポインター</param>
void RotationCubeBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	//回転量
	static const float ROTATION_ANGLE = DirectX::XM_PI;
	//移動秒数
	static const float MOVE_TIME_S = 5.0f;
	//回転時間のクールタイム
	static float ROTATION_COOL_TIME_S = 10.0f;

	//Actor型かたStage型にダイナミックキャストする
	Stage* stage = dynamic_cast<Stage*>(actor);

	//stageがnullptrであれば実行しない
	if (stage == nullptr)
	{
		return;
	}

	//経過時間
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());
	//回転時間
	float rotationTime_s = stage->GetRotationTime();
	//回転しているかどうか
	bool isRotation = stage->IsRotation();
	//タイム
	float time_s = stage->GetTime();

	//現在のの角度
	DirectX::SimpleMath::Vector3 nowRotation = stage->GetRotation().ToEuler();
	//前の角度
	DirectX::SimpleMath::Vector3 previousRotation = stage->GetPreviousRotation();
	//次の角度
	DirectX::SimpleMath::Vector3 nextRotation = stage->GetNextRotation();

	//経過時間で引く
	rotationTime_s -= elapsedTime_s;

	//回転時間０になったら回転する
	if (rotationTime_s <= 0)
	{
		//回転いないのであれば次の角度を決める
		if (!isRotation)
		{
			//回転方向
			int rotationDir = MyRandom::GetIntRange(0, 1);
			//１であればX軸回転する
			if (rotationDir == 1)
			{
				nextRotation.x += ROTATION_ANGLE * MyRandom::GetIntRange(-2, 2);
			}
			//０であればZ軸回転する
			else if (rotationDir == 0)
			{
				nextRotation.z += ROTATION_ANGLE * MyRandom::GetIntRange(-2, 2);
				
			}
			//回転している状態にする
			isRotation = true;

		}
		//タイムを経過時間で足す
		time_s += elapsedTime_s;

		//回転する
		nowRotation = DirectX::SimpleMath::Vector3::Lerp(previousRotation, nextRotation, time_s / MOVE_TIME_S);
		 

		//タイムが移動時間より大きくなったらタイムをリセット
		if (time_s > MOVE_TIME_S)
		{

			//タイムのリセットする
			rotationTime_s = ROTATION_COOL_TIME_S;
			time_s = 0;
			//前の角度を更新
			previousRotation = nowRotation;
			//回転していない状態にする
			isRotation = false;
		}
	}

	//回転しているかの状態を設定
	stage->SetIsRotation(isRotation);
	//回転時間の設定
	stage->SetRotationTime(rotationTime_s);
	//タイムを設定
	stage->SetTime(time_s);
	//前の角度を設定
	stage->SetPreviousRotation(previousRotation);
	//次の角度を設定
	stage->SetNextRotation(nextRotation);
	//現在の角度を設定
	stage->SetRotation(nowRotation);

}
