/*
* 2023/03/21
* NormalBehavior.h
* �����@��
*/
#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>

/// <summary>
/// ���ʂ̉��̃r�w�C�r�A�[
/// </summary>
class NormalBehavior :public IBehavior
{
public:
	//�R���X�g���N�^
	NormalBehavior() = default;
	//�f�X�g���N�^
	~NormalBehavior()override = default;
	
	/// <summary>
	/// ���ʂ̉��̓���
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <param name="actor">��Q���̃|�C���^�[�ithis�j</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};