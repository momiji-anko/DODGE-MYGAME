/*
* 2023/03/21
* MeteoriteBehavior.h
* �����@��
*/
#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>

/// <summary>
/// 覐΂̏�Q���̃r�w�C�r�A�[
/// </summary>
class MeteoriteBehavior :public IBehavior
{
public:
	//�R���X�g���N�^
	MeteoriteBehavior() = default;
	//�f�X�g���N�^
	~MeteoriteBehavior()override = default;
	/// <summary>
	/// 覐΂̓���
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <param name="actor">��Q���̃|�C���^�[�ithis�j</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};