/*
* 2023/04/07
* TiltingFloorbehavior.h
* �X�e�[�W�Q�̌X����
* �����@��
*/
#pragma once
#include"Game/PlayScene/IBehavior.h"

/// <summary>
/// �X�e�[�W�Q�̌X����
/// </summary>
class TiltingFloorbehavior : public IBehavior
{
public:
	TiltingFloorbehavior() = default;
	~TiltingFloorbehavior()override = default;

	/// <summary>
	/// �X�e�[�W�Q�̌X�����̓���
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <param name="actor">�X�e�[�W�̃|�C���^�[</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};