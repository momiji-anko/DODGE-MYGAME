/*
* 2023/03/21
* MeanderingBehavior.h
* �����@��
*/
#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>

/// <summary>
/// �֍s���鉊�̏�Q���̃r�w�C�r�A�[
/// </summary>
class MeanderingBehavior :public IBehavior
{
public:
	//�R���X�g���N�^
	MeanderingBehavior() = default;
	//�f�X�g���N�^
	~MeanderingBehavior()override = default;

	/// <summary>
	/// �֍s���鉊�̌ŗL�̍s��
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <param name="actor">��Q���̃|�C���^�[�ithis�j</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};