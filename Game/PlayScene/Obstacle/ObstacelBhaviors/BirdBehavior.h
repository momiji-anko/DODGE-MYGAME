/*
* 2023/03/21
* BirdBehavior.h
* �����@��
*/
#pragma once
#include"../../IBehavior.h"
#include<SimpleMath.h>

/// <summary>
/// ���̃r�w�C�r�A�[
/// </summary>
class BirdBehavior :public IBehavior
{
public:
	//�R���X�g���N�^
	BirdBehavior() = default;
	//�f�X�g���N�^
	~BirdBehavior()override = default;

	/// <summary>
	/// ���̏�Q���̓���
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <param name="actor">��Q���̃|�C���^�[</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};