/*
* 2023/03/21
* ReverseRotateStickBehavior.h
* �����@��
*/
#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>

/// <summary>
/// �����v���ɉ�]����_�̃r�w�C�r�A�[
/// </summary>
class ReverseRotateStickBehavior :public IBehavior
{
public:
	//�R���X�g���N�^
	ReverseRotateStickBehavior() = default;
	//�f�X�g���N�^
	~ReverseRotateStickBehavior()override = default;

	/// <summary>
	/// �����v���ɉ�]����_�̓���
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <param name="actor">��Q���̃|�C���^�[�ithis�j</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};