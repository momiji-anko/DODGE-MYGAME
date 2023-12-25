/*
* 2023/12/01
* BaseFloorToFallBehavior.h
* 一番目に落ちる床の動き
* 麻生　楓
*/
#pragma once
#include"Game/PlayScene/IBehavior.h"
#include<SimpleMath.h>
#include<vector>

//前方宣言
class Stage;

/// <summary>
/// 落ちる床の基底クラス
/// </summary>
class BaseFloorToFallBehavior : public IBehavior
{
public:

	//ステージの揺れ
	static const float STAGE_VIBRATION;
	//落ちる床のY座標開始位置
	static const float START_FALL_POSITION_Y;
	//落ちる床のY座標最終位置
	static const float END_FALL_POSITION_Y;
	//帰ってくる床のX座標開始位置
	static const float END_RETURN_POSITION_X;
	//帰ってくる床のX座標終了位置
	static const float START_RETURN_POSITION_X;
	//移動時間
	static const float MOVE_TIME_S;

	//行動の関数ポインタ配列
	static const std::vector<std::function<void(
		int& routine,
		float& timer_s,
		DirectX::SimpleMath::Vector3& position,
		DirectX::SimpleMath::Vector3& offsetPosition,
		Stage* stage)>> ROUTINE_FUNCTION_LIST;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BaseFloorToFallBehavior() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~BaseFloorToFallBehavior()override = default;

	/// <summary>
	/// 落ちる床の動きの読み込み
	/// </summary>
	/// <param name="numFallFloor">何番目に落ちる床か</param>
	/// <returns>落ちる床の動きの番号リスト</returns>
	const std::vector<int>& GetMoveBehaviorList(int numFallFloor);

	///関数ポインタ
	
	/// <summary>
	/// タイマーリセット
	/// </summary>
	/// <param name="routine">ルーチン</param>
	/// <param name="timer_s">タイマー</param>
	/// <param name="position">座標(使わない)</param>
	/// <param name="offsetPosition">オフセット座標(使わない)</param>
	/// <param name="stage">ステージのポインター(使わない)</param>
	static void TimerReset(int& routine, float& timer_s, DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& offsetPosition, Stage* stage);
	/// <summary>
	/// 床を落とす
	/// </summary>
	/// <param name="routine">ルーチン(使わない)</param>
	/// <param name="timer_s">タイマー</param>
	/// <param name="position">座標</param>
	/// <param name="offsetPosition">オフセット座標</param>
	/// <param name="stage">ステージのポインター</param>
	static void FallFloor(int& routine, float& timer_s, DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& offsetPosition, Stage* stage);
	/// <summary>
	/// 床を元の場所に戻す
	/// </summary>
	/// <param name="routine">ルーチン(使わない)</param>
	/// <param name="timer_s">タイマー</param>
	/// <param name="position">座標</param>
	/// <param name="offsetPosition">オフセット座標(使わない)</param>
	/// <param name="stage">ステージのポインター</param>
	static void ReturnPosition(int& routine, float& timer_s, DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& offsetPosition, Stage* stage);
	/// <summary>
	/// 床を震えさせる
	/// </summary>
	/// <param name="routine">ルーチン(使わない)</param>
	/// <param name="timer_s">タイマー(使わない)</param>
	/// <param name="position">座標(使わない)</param>
	/// <param name="offsetPosition">オフセット座標</param>
	/// <param name="stage">ステージのポインター(使わない)</param>
	static void VibrationPosition(int& routine, float& timer_s, DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& offsetPosition, Stage* stage);
	/// <summary>
	/// 床を戻す用の準備
	/// </summary>
	/// <param name="routine">ルーチン(使わない)</param>
	/// <param name="timer_s">タイマー(使わない)</param>
	/// <param name="position">座標</param>
	/// <param name="offsetPosition">オフセット座標(使わない)</param>
	/// <param name="stage">ステージのポインター(使わない)</param>
	static void ReturnStartPosition(int& routine, float& timer_s, DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& offsetPosition, Stage* stage);

private:
	//行動配列の保存用配列
	std::vector<int> m_moveBehaviorsNum;
};