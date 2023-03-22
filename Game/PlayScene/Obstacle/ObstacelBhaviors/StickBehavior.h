/*
* 2023/03/21
* StickBehavior.h
* �����@��
*/
#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>

/// <summary>
/// �_�̏�Q���̃r�w�C�r�A�[
/// </summary>
class StickBehavior :public IBehavior
{
public:
	//�R���X�g���N�^
	StickBehavior()= default;
	//�f�X�g���N�^
	~StickBehavior()override = default;

	/// <summary>
	/// �_�̓���
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <param name="actor">��Q���̃|�C���^�[�ithis�j</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};
