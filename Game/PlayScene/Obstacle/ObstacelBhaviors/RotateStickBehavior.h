/*
* 2023/03/21
* RotateStickBehavior.h
* �����@��
*/
#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>

/// <summary>
/// ��]����_�̃r�w�C�r�A�[
/// </summary>
class RotateStickBehavior :public IBehavior
{
public:
	//�R���X�g���N�^
	RotateStickBehavior() = default;
	//�f�X�g���N�^
	~RotateStickBehavior()override = default;
	
	/// <summary>
	/// ��]����_�̓���
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <param name="actor">��Q���̃|�C���^�[�ithis�j</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};
