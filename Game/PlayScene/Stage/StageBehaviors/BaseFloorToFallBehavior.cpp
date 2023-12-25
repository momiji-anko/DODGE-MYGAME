/*
* 2023/12/01
* BaseFloorToFallBehavior.h
* ��Ԗڂɗ����鏰�̓���
* �����@��
*/
#include"pch.h"
#include"BaseFloorToFallBehavior.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include"../Stage.h"
#include"Game/PlayScene/MyRandom.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/Json/json.hpp"

//�X�e�[�W�̗h��
const float BaseFloorToFallBehavior::STAGE_VIBRATION = 0.2f;
//�����鏰��Y���W�J�n�ʒu
const float BaseFloorToFallBehavior::START_FALL_POSITION_Y = -1.0f;
//�����鏰��Y���W�ŏI�ʒu
const float BaseFloorToFallBehavior::END_FALL_POSITION_Y = -100.0f;
//�A���Ă��鏰��X���W�J�n�ʒu
const float BaseFloorToFallBehavior::END_RETURN_POSITION_X = 6.0f;
//�A���Ă��鏰��X���W�I���ʒu
const float BaseFloorToFallBehavior::START_RETURN_POSITION_X = 100.0f;
//�ړ�����
const float BaseFloorToFallBehavior::MOVE_TIME_S = 5.0f;

//�s���̊֐��|�C���^�z��
 const std::vector<std::function<void(
	int& routine,
	float& timer_s,
	DirectX::SimpleMath::Vector3& position,
	DirectX::SimpleMath::Vector3& offsetPosition,
	Stage* stage)>> BaseFloorToFallBehavior::ROUTINE_FUNCTION_LIST =
 {
	//�O�F�������Ȃ�
	nullptr,
	//�P�F�^�C�}�[���Z�b�g
	TimerReset,
	//�Q�F����h�炷
	VibrationPosition,
	//�R�F���𗎂Ƃ�
	FallFloor,
	//�S�F����߂��p�̊J�n�ʒu
	ReturnStartPosition,
	//�T�F���̏ꏊ�ɖ߂�
	ReturnPosition
 };

/// <summary>
/// �����鏰�̓���
/// </summary>
/// <param name="numFallFloor">���Ԗڂɗ����鏰��</param>
/// <returns>�����鏰�̓����̔ԍ����X�g</returns>
const std::vector<int>& BaseFloorToFallBehavior::GetMoveBehaviorList( int numFallFloor)
{ 
	//�Ȃɂ������Ă��Ȃ��ꍇ��JSON����ǂݍ���
	if (m_moveBehaviorsNum.size() == 0)
	{
		std::wstring filePath = L"Resources/StageData/Stage1.json";
		//fstream�쐬
		std::fstream file(filePath, std::ifstream::in);
		//json�ǂݍ���
		nlohmann::json stageJson = nlohmann::json::parse(file);
		//�t�@�C�������
		file.close();


		m_moveBehaviorsNum.resize(stageJson["FallMoveBehavior"][numFallFloor].size());

		//�s���ԍ��ǂݍ���
		for (int i = 0; i < stageJson["FallMoveBehavior"][numFallFloor].size(); i++)
		{
			m_moveBehaviorsNum[i] = stageJson["FallMoveBehavior"][numFallFloor][i];
		}
	}
	
	return m_moveBehaviorsNum;
}

/// <summary>
/// �^�C�}�[���Z�b�g
/// </summary>
/// <param name="routine">���[�`��</param>
/// <param name="timer_s">�^�C�}�[</param>
/// <param name="position">���W(�g��Ȃ�)</param>
/// <param name="offsetPosition">�I�t�Z�b�g���W(�g��Ȃ�)</param>
/// <param name="stage">�X�e�[�W�̃|�C���^�[(�g��Ȃ�)</param>
void BaseFloorToFallBehavior::TimerReset(
	int& routine,
	float& timer_s,
	DirectX::SimpleMath::Vector3& position,
	DirectX::SimpleMath::Vector3& offsetPosition,
	Stage* stage)
{
	//�x������
	UNREFERENCED_PARAMETER(position);
	UNREFERENCED_PARAMETER(offsetPosition);
	UNREFERENCED_PARAMETER(stage);
	//���̃��[�`���ɂ���
	routine++;
	//�^�C�}�[���Z�b�g
	timer_s = 0.0f;
}

/// <summary>
/// ���𗎂Ƃ�
/// </summary>
/// <param name="routine">���[�`��(�g��Ȃ�)</param>
/// <param name="timer_s">�^�C�}�[</param>
/// <param name="position">���W</param>
/// <param name="offsetPosition">�I�t�Z�b�g���W</param>
/// <param name="stage">�X�e�[�W�̃|�C���^�[</param>
void BaseFloorToFallBehavior::FallFloor(
	int& routine,
	float& timer_s,
	DirectX::SimpleMath::Vector3& position,
	DirectX::SimpleMath::Vector3& offsetPosition,
	Stage* stage)
{
	//�x������
	UNREFERENCED_PARAMETER(routine);

	//���𗎂Ƃ�	
	position.y = stage->Lerp(START_FALL_POSITION_Y, END_FALL_POSITION_Y, timer_s / MOVE_TIME_S);

	//�h����~�߂�
	offsetPosition = DirectX::SimpleMath::Vector3::Zero;
}

/// <summary>
/// �������̏ꏊ�ɖ߂�
/// </summary>
/// <param name="routine">���[�`��(�g��Ȃ�)</param>
/// <param name="timer_s">�^�C�}�[</param>
/// <param name="position">���W</param>
/// <param name="offsetPosition">�I�t�Z�b�g���W(�g��Ȃ�)</param>
/// <param name="stage">�X�e�[�W�̃|�C���^�[</param>
void BaseFloorToFallBehavior::ReturnPosition(
	int& routine,
	float& timer_s,
	DirectX::SimpleMath::Vector3& position,
	DirectX::SimpleMath::Vector3& offsetPosition,
	Stage* stage)
{
	//�x������
	UNREFERENCED_PARAMETER(routine);
	UNREFERENCED_PARAMETER(offsetPosition);

	//�������̈ʒu�ɖ߂�
	if (position.x < 0)
	{
		position.x = stage->Lerp(-START_RETURN_POSITION_X, -END_RETURN_POSITION_X, timer_s / MOVE_TIME_S);
	}
	else
	{
		position.x = stage->Lerp(START_RETURN_POSITION_X, END_RETURN_POSITION_X, timer_s / MOVE_TIME_S);
	}
}

/// <summary>
/// ����k��������
/// </summary>
/// <param name="routine">���[�`��(�g��Ȃ�)</param>
/// <param name="timer_s">�^�C�}�[(�g��Ȃ�)</param>
/// <param name="position">���W(�g��Ȃ�)</param>
/// <param name="offsetPosition">�I�t�Z�b�g���W</param>
/// <param name="stage">�X�e�[�W�̃|�C���^�[(�g��Ȃ�)</param>
void BaseFloorToFallBehavior::VibrationPosition(
	int& routine,
	float& timer_s,
	DirectX::SimpleMath::Vector3& position,
	DirectX::SimpleMath::Vector3& offsetPosition,
	Stage* stage)
{
	//�x������
	UNREFERENCED_PARAMETER(routine);
	UNREFERENCED_PARAMETER(timer_s);
	UNREFERENCED_PARAMETER(position);
	UNREFERENCED_PARAMETER(stage);

	//����h�炷
	offsetPosition.x = MyRandom::GetFloatRange(-STAGE_VIBRATION, STAGE_VIBRATION);
	offsetPosition.y = MyRandom::GetFloatRange(-STAGE_VIBRATION, STAGE_VIBRATION);
	offsetPosition.z = MyRandom::GetFloatRange(-STAGE_VIBRATION, STAGE_VIBRATION);
}

/// <summary>
/// ����߂��p�̏���
/// </summary>
/// <param name="routine">���[�`��(�g��Ȃ�)</param>
/// <param name="timer_s">�^�C�}�[(�g��Ȃ�)</param>
/// <param name="position">���W</param>
/// <param name="offsetPosition">�I�t�Z�b�g���W(�g��Ȃ�)</param>
/// <param name="stage">�X�e�[�W�̃|�C���^�[(�g��Ȃ�)</param>
void BaseFloorToFallBehavior::ReturnStartPosition(
	int& routine,
	float& timer_s,
	DirectX::SimpleMath::Vector3& position,
	DirectX::SimpleMath::Vector3& offsetPosition,
	Stage* stage)
{
	//�x������
	UNREFERENCED_PARAMETER(routine);
	UNREFERENCED_PARAMETER(offsetPosition);
	UNREFERENCED_PARAMETER(stage);
	UNREFERENCED_PARAMETER(timer_s);

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
}


