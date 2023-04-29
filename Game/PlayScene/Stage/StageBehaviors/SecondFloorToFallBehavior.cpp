/*
* 2023/03/27
* SecondFloorToFallBhavior.cpp
* ��Ԗڂɗ����鏰�̓���
* �����@��
*/
#include"pch.h"
#include"SecondFloorToFallBehavior.h"
#include"../Stage.h"
#include"Game/PlayScene/MyRandom.h"

/// <summary>
/// ��Ԗڂɗ����鏰�̓���
/// </summary>
/// <param name="timer">�^�C�}�[</param>
/// <param name="actor">�X�e�[�W�̃|�C���^�[</param>
void SecondFloorToFallBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	//�X�e�[�W�̗h��
	static const float STAGE_VIBRATION = 0.2f;
	//�����鏰��Y���W�J�n�ʒu
	static const float START_FALL_POSITION_Y = -1.0f;
	//�����鏰��Y���W�ŏI�ʒu
	static const float END_FALL_POSITION_Y = -100.0f;
	//�A���Ă��鏰��X���W�J�n�ʒu
	static const float END_RETURN_POSITION_X = 6.0f;
	//�A���Ă��鏰��X���W�I���ʒu
	static const float START_RETURN_POSITION_X = 100.0f;
	//�ړ�����
	static const float MOVE_TIME_S = 5.0f;

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

	//���[�`���̎擾
	int routine = stage->GetRoutine();

	//���W�擾
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	//�I�t�Z�b�g���W�擾
	DirectX::SimpleMath::Vector3 offsetPosition = stage->GetOffsetPosition();

	//�^�C�����o�ߎ��Ԃő���
	time_s += elapsedTime_s;


	//���[�`���ɂ���čs����ς���
	switch (routine)
	{
	case 0:

		break;
	case 1:
		time_s = 0;
		routine++;
	case 2:

		break;
	case 3:
		time_s = 0;
		routine++;
	case 4:
		break;
	case 5:
		time_s = 0;
		routine++;
	case 6:

		break;
	case 7:
		time_s = 0;
		routine++;
	case 8:

		break;
	case 9:
		time_s = 0;
		routine++;
	case 10:

		//�X�e�[�W��h�炷
		offsetPosition.x = MyRandom::GetFloatRange(-STAGE_VIBRATION, STAGE_VIBRATION);
		offsetPosition.y = MyRandom::GetFloatRange(-STAGE_VIBRATION, STAGE_VIBRATION);
		offsetPosition.z = MyRandom::GetFloatRange(-STAGE_VIBRATION, STAGE_VIBRATION);

		break;
	case 11:
		time_s = 0;
		routine++;
	case 12:
		//�X�e�[�W�𗎂Ƃ�
		position.y = stage->Lerp(START_FALL_POSITION_Y, END_FALL_POSITION_Y, time_s / MOVE_TIME_S);
		//�h����~�߂�
		offsetPosition = DirectX::SimpleMath::Vector3::Zero;

		break;
	case 13:
		time_s = 0;
		routine++;
	case 14:

		//�X�^�[�g���W�ɖ߂�
		position.y = START_FALL_POSITION_Y;

		//X���{�[�ɂ���ċA���Ă���ۂ̏���X���W��ς���
		if (position.x < 0)
		{
			position.x = -START_RETURN_POSITION_X;
		}
		else
		{
			position.x = START_RETURN_POSITION_X;

		}

		break;
	case 15:
		time_s = 0;
		routine++;
	case 16:

		break;
	case 17:
		time_s = 0;
		routine++;
	case 18:

		break;
	case 19:
		time_s = 0;
		routine++;
	case 20:

		break;
	case 21:
		time_s = 0;
		routine++;
	case 22:

		break;
	case 23:
		time_s = 0;
		routine++;
	case 24:

		break;
	case 25:
		time_s = 0;
		routine++;
	case 26:

		if (position.x < 0)
		{
			position.x = stage->Lerp(-START_RETURN_POSITION_X, -END_RETURN_POSITION_X, time_s / MOVE_TIME_S);
		}
		else
		{
			position.x = stage->Lerp(START_RETURN_POSITION_X, END_RETURN_POSITION_X, time_s / MOVE_TIME_S);
		}
		break;
	case 27:
		//�s�����I��
		stage->SetMoveEndFlag(true);
		break;

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



