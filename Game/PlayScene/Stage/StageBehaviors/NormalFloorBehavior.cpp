/*
* 2023/03/27
* NormalFloorBehavior.cpp
* ���ʂ̏��̓���
* �����@��
*/
#include"pch.h"
#include"NormalFloorBehavior.h"
#include"../Stage.h"
#include"Game/PlayScene/MyRandom.h"

/// <summary>
/// ���ʂ̏��̓���
/// </summary>
/// <param name="timer">�^�C�}�[</param>
/// <param name="actor">�X�e�[�W�̃|�C���^�[</param>
void NormalFloorBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	UNREFERENCED_PARAMETER(timer);

	//�X�e�[�W�^�ɃL���X�g
	Stage* stage = dynamic_cast<Stage*>(actor);

	//�s�������Ȃ����ߍs�����I��
	stage->SetMoveEndFlag(true);
	
}