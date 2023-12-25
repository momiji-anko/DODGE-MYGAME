/*
* 2023/12/06
* FloorToFallBehavior.h
* �����鏰�̓���
* �����@��
*/
#include"pch.h"
#include"FloorToFallBehavior.h"
#include"../Stage.h"
#include"Game/PlayScene/MyRandom.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/Json/json.hpp"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="numFallFloor">���Ԗڂɗ����鏰��</param>
FloorToFallBehavior::FloorToFallBehavior(int numFallFloor)
	:
	m_numFallFloor(numFallFloor - 1)
{
}

/// <summary>
/// �����鏰�̓���
/// </summary>
/// <param name="timer">�^�C�}�[</param>
/// <param name="actor">�X�e�[�W�̃|�C���^�[</param>
void FloorToFallBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	//�o�ߎ���
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	//�X�e�[�W�^�ɃL���X�g
	Stage* stage = dynamic_cast<Stage*>(actor);

	//stage��nullptr�ł���Ύ��s���Ȃ�
	if (stage == nullptr)
	{
		return;
	}

	//�^�C���̎擾
	float time_s = stage->GetTime();

	//���[�`���̎擾�@
	int routine = stage->GetRoutine();

	//���W�擾
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	//�I�t�Z�b�g���W�擾
	DirectX::SimpleMath::Vector3 offsetPosition = stage->GetOffsetPosition();

	//�^�C�����o�ߎ��Ԃő���
	time_s += elapsedTime_s;

	//�s���ǂݍ���
	const std::vector<int>& moveFunctionList = GetMoveBehaviorList(m_numFallFloor);

	//���[�`�����s���̐��ȏ�ɂȂ�΍s�����I������
	if (routine >= static_cast<int>(moveFunctionList.size() - 1))
	{
		//�s�����I��
		stage->SetMoveEndFlag(true);
	}

	//null�łȂȂ� �� ���[�`���������z��ȉ� �� �s���I�����Ă��Ȃ��ꍇ���s����
	if (ROUTINE_FUNCTION_LIST[moveFunctionList[routine]] != nullptr && routine <= moveFunctionList.size() && !stage->GetMoveEndFlag())
	{
		ROUTINE_FUNCTION_LIST[moveFunctionList[routine]](routine, time_s, position, offsetPosition, stage);
	}

	//�ړ����Ԃ𒴂����烋�[�`����i�߂�
	if (time_s > MOVE_TIME_S)
	{
		time_s = MOVE_TIME_S;
		routine++;
	}

	//���W�̐ݒ�
	stage->SetPosition(position);
	//�I�t�Z�b�g���W�̐ݒ�
	stage->SetOffsetPosition(offsetPosition);
	//���[�`���̐ݒ�
	stage->SetRoutine(routine);
	//�^�C���̐ݒ�
	stage->SetTime(time_s);

}


