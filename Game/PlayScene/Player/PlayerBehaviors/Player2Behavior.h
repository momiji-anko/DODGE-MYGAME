/*
* 
*/
#pragma once
#include"Game/PlayScene/IBehavior.h"


class Player2Behavior :public IBehavior
{
	//�R���X�g���N�^
	Player2Behavior() = default;
	//�f�X�g���N�^
	~Player2Behavior()override = default;

	/// <summary>
	/// �v���C���[�Q�̓���
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <param name="actor">�v���C���[�̃|�C���^�[�ithis�j</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};