/*
* 202323/12/06
* FloorToFallBehavior.h
* �����鏰�̓���
* �����@��
*/
#pragma once
#include"BaseFloorToFallBehavior.h"
#include<SimpleMath.h>
#include<vector>

//�O���錾
class Stage;

/// <summary>
/// �����鏰�̓���
/// </summary>
class FloorToFallBehavior : public BaseFloorToFallBehavior
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="numFallFloor">���Ԗڂɗ����鏰��</param>
	FloorToFallBehavior(int numFallFloor);
	//�f�X�g���N�^
	~FloorToFallBehavior()override = default;

	/// <summary>
	/// �����鏰�̓���
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <param name="actor">�X�e�[�W�̃|�C���^�[</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;

private:
	int m_numFallFloor;

};