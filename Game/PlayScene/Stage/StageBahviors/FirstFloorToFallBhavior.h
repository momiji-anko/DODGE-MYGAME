/*
* 2023/03/27
* FirstFloorToFallBhavior.h
* �����@��
*/
#pragma once
#include"Game/PlayScene/IBehavior.h"

/// <summary>
/// ��Ԗڂɗ����鏰�̓���
/// </summary>
class FirstFloorToFallBhavior : public IBehavior
{
public:
	FirstFloorToFallBhavior() = default;
	~FirstFloorToFallBhavior()override = default;

	/// <summary>
	/// ��Ԗڂɗ����鏰�̓���
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <param name="actor">�X�e�[�W�̃|�C���^�[</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};