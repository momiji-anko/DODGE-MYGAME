/*
* 2023/03/27
* ThirdFloorToFallBhavior.h
* �����@��
*/
#pragma once
#include"Game/PlayScene/IBehavior.h"


/// <summary>
/// �O�Ԗڂɗ����鏰�̓���
/// </summary>
class ThirdFloorToFallBhavior : public IBehavior
{
	ThirdFloorToFallBhavior() = default;
	~ThirdFloorToFallBhavior()override = default;

	/// <summary>
	/// �O�Ԗڂɗ����鏰�̓���
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <param name="actor">�X�e�[�W�̃|�C���^�[</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};