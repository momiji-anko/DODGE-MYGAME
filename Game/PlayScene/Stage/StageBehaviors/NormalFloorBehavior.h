/*
* 2023/03/27
* NormalFloorBehavior.h
* ���ʂ̏��̓���
* �����@��
*/
#pragma once
#include"Game/PlayScene/IBehavior.h"


/// <summary>
/// ���ʂ̏��̓���
/// </summary>
class NormalFloorBehavior : public IBehavior
{
public:
	NormalFloorBehavior() = default;
	~NormalFloorBehavior()override = default;

	/// <summary>
	/// ���ʂ̏��̓���
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <param name="actor">�X�e�[�W�̃|�C���^�[</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};