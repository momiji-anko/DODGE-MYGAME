/*
* 2023/03/27
* ThirdFloorToFallBhavior.h
* �O�Ԗڂɗ����鏰�̓���
* �����@��
*/
#pragma once
#include"Game/PlayScene/IBehavior.h"


/// <summary>
/// �O�Ԗڂɗ����鏰�̓���
/// </summary>
class ThirdFloorToFallBehavior : public IBehavior
{
public:
	ThirdFloorToFallBehavior() = default;
	~ThirdFloorToFallBehavior()override = default;

	/// <summary>
	/// �O�Ԗڂɗ����鏰�̓���
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <param name="actor">�X�e�[�W�̃|�C���^�[</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};