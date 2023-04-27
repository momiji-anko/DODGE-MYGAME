/*
* 2023/04/11
* RotationCubeBehavior.h
* ��]����L���[�u
* �����@��
*/
#include"pch.h"
#include"RotationCubeBehavior.h"
#include"Game/PlayScene/MyRandom.h"
#include"../Stage.h"

/// <summary>
/// ��]����L���[�u�̓���
/// </summary>
/// <param name="timer">�^�C�}�[</param>
/// <param name="actor">�X�e�[�W�̃|�C���^�[</param>
void RotationCubeBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	//��]��
	static const float ROTATION_ANGLE = DirectX::XM_PI;
	//�ړ��b��
	static const float MOVE_TIME_S = 5.0f;
	//��]���Ԃ̃N�[���^�C��
	static float ROTATION_COOL_TIME_S = 10.0f;

	//Actor�^����Stage�^�Ƀ_�C�i�~�b�N�L���X�g����
	Stage* stage = dynamic_cast<Stage*>(actor);

	//stage��nullptr�ł���Ύ��s���Ȃ�
	if (stage == nullptr)
	{
		return;
	}

	//�o�ߎ���
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());
	//��]����
	float rotationTime_s = stage->GetRotationTime();
	//��]���Ă��邩�ǂ���
	bool isRotation = stage->IsRotation();
	//�^�C��
	float time_s = stage->GetTime();

	//���݂̂̊p�x
	DirectX::SimpleMath::Vector3 nowRotation = stage->GetRotation().ToEuler();
	//�O�̊p�x
	DirectX::SimpleMath::Vector3 previousRotation = stage->GetPreviousRotation();
	//���̊p�x
	DirectX::SimpleMath::Vector3 nextRotation = stage->GetNextRotation();

	//�o�ߎ��Ԃň���
	rotationTime_s -= elapsedTime_s;

	//��]���ԂO�ɂȂ������]����
	if (rotationTime_s <= 0)
	{
		//��]���Ȃ��̂ł���Ύ��̊p�x�����߂�
		if (!isRotation)
		{
			//��]����
			int rotationDir = MyRandom::GetIntRange(0, 1);
			//�P�ł����X����]����
			if (rotationDir == 1)
			{
				nextRotation.x += ROTATION_ANGLE * MyRandom::GetIntRange(-2, 2);
			}
			//�O�ł����Z����]����
			else if (rotationDir == 0)
			{
				nextRotation.z += ROTATION_ANGLE * MyRandom::GetIntRange(-2, 2);
				
			}
			//��]���Ă����Ԃɂ���
			isRotation = true;

		}
		//�^�C�����o�ߎ��Ԃő���
		time_s += elapsedTime_s;

		//��]����
		nowRotation = DirectX::SimpleMath::Vector3::Lerp(previousRotation, nextRotation, time_s / MOVE_TIME_S);
		 

		//�^�C�����ړ����Ԃ��傫���Ȃ�����^�C�������Z�b�g
		if (time_s > MOVE_TIME_S)
		{

			//�^�C���̃��Z�b�g����
			rotationTime_s = ROTATION_COOL_TIME_S;
			time_s = 0;
			//�O�̊p�x���X�V
			previousRotation = nowRotation;
			//��]���Ă��Ȃ���Ԃɂ���
			isRotation = false;
		}
	}

	//��]���Ă��邩�̏�Ԃ�ݒ�
	stage->SetIsRotation(isRotation);
	//��]���Ԃ̐ݒ�
	stage->SetRotationTime(rotationTime_s);
	//�^�C����ݒ�
	stage->SetTime(time_s);
	//�O�̊p�x��ݒ�
	stage->SetPreviousRotation(previousRotation);
	//���̊p�x��ݒ�
	stage->SetNextRotation(nextRotation);
	//���݂̊p�x��ݒ�
	stage->SetRotation(nowRotation);

}
