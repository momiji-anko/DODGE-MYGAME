/*
*
*/
#pragma once
#include"Game/PlayScene/IBehavior.h"


class Player1Behavior :public IBehavior
{
	//�R���X�g���N�^
	Player1Behavior() = default;
	//�f�X�g���N�^
	~Player1Behavior()override = default;

	/// <summary>
	/// �v���C���[1�̓���
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <param name="actor">�v���C���[�̃|�C���^�[�ithis�j</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};