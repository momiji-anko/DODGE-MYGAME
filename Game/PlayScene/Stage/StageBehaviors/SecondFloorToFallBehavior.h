/*
* 2023/03/27
* SecondFloorToFallBhavior.h
* �����@��
*/
#pragma once
#include"Game/PlayScene/IBehavior.h"

/// <summary>
/// ��Ԗڂɗ����鏰�̓���
/// </summary>
class SecondFloorToFallBehavior : public IBehavior
{
public:
	SecondFloorToFallBehavior() = default;
	~SecondFloorToFallBehavior()override = default;

	/// <summary>
	/// ��Ԗڂɗ����鏰�̓���
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <param name="actor">�X�e�[�W�̃|�C���^�[</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};