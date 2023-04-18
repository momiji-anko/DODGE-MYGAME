/*
* 2023/04/11
* RotationCubeBehavior.h
* 回転するキューブ
* 麻生　楓
*/
#include"Game/PlayScene/IBehavior.h"


/// <summary>
/// 回転するキューブの動き
/// </summary>
class RotationCubeBehavior : public IBehavior
{
public:
	RotationCubeBehavior() = default;
	~RotationCubeBehavior()override = default;

	/// <summary>
	/// 回転するキューブの動き
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <param name="actor">ステージのポインター</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};