/*
* 2023/03/27
* FirstFloorToFallBhavior.h
* ��Ԗڂɗ����鏰�̓���
* �����@��
*/
#pragma once
#include"Game/PlayScene/IBehavior.h"

/// <summary>
/// ��Ԗڂɗ����鏰�̓���
/// </summary>
class FirstFloorToFallBehavior : public IBehavior
{
public:
	FirstFloorToFallBehavior() = default;
	~FirstFloorToFallBehavior()override = default;

	/// <summary>
	/// ��Ԗڂɗ����鏰�̓���
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <param name="actor">�X�e�[�W�̃|�C���^�[</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};