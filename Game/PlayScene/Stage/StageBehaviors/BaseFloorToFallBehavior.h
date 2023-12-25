/*
* 2023/12/01
* BaseFloorToFallBehavior.h
* ��Ԗڂɗ����鏰�̓���
* �����@��
*/
#pragma once
#include"Game/PlayScene/IBehavior.h"
#include<SimpleMath.h>
#include<vector>

//�O���錾
class Stage;

/// <summary>
/// �����鏰�̊��N���X
/// </summary>
class BaseFloorToFallBehavior : public IBehavior
{
public:

	//�X�e�[�W�̗h��
	static const float STAGE_VIBRATION;
	//�����鏰��Y���W�J�n�ʒu
	static const float START_FALL_POSITION_Y;
	//�����鏰��Y���W�ŏI�ʒu
	static const float END_FALL_POSITION_Y;
	//�A���Ă��鏰��X���W�J�n�ʒu
	static const float END_RETURN_POSITION_X;
	//�A���Ă��鏰��X���W�I���ʒu
	static const float START_RETURN_POSITION_X;
	//�ړ�����
	static const float MOVE_TIME_S;

	//�s���̊֐��|�C���^�z��
	static const std::vector<std::function<void(
		int& routine,
		float& timer_s,
		DirectX::SimpleMath::Vector3& position,
		DirectX::SimpleMath::Vector3& offsetPosition,
		Stage* stage)>> ROUTINE_FUNCTION_LIST;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	BaseFloorToFallBehavior() = default;
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~BaseFloorToFallBehavior()override = default;

	/// <summary>
	/// �����鏰�̓����̓ǂݍ���
	/// </summary>
	/// <param name="numFallFloor">���Ԗڂɗ����鏰��</param>
	/// <returns>�����鏰�̓����̔ԍ����X�g</returns>
	const std::vector<int>& GetMoveBehaviorList(int numFallFloor);

	///�֐��|�C���^
	
	/// <summary>
	/// �^�C�}�[���Z�b�g
	/// </summary>
	/// <param name="routine">���[�`��</param>
	/// <param name="timer_s">�^�C�}�[</param>
	/// <param name="position">���W(�g��Ȃ�)</param>
	/// <param name="offsetPosition">�I�t�Z�b�g���W(�g��Ȃ�)</param>
	/// <param name="stage">�X�e�[�W�̃|�C���^�[(�g��Ȃ�)</param>
	static void TimerReset(int& routine, float& timer_s, DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& offsetPosition, Stage* stage);
	/// <summary>
	/// ���𗎂Ƃ�
	/// </summary>
	/// <param name="routine">���[�`��(�g��Ȃ�)</param>
	/// <param name="timer_s">�^�C�}�[</param>
	/// <param name="position">���W</param>
	/// <param name="offsetPosition">�I�t�Z�b�g���W</param>
	/// <param name="stage">�X�e�[�W�̃|�C���^�[</param>
	static void FallFloor(int& routine, float& timer_s, DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& offsetPosition, Stage* stage);
	/// <summary>
	/// �������̏ꏊ�ɖ߂�
	/// </summary>
	/// <param name="routine">���[�`��(�g��Ȃ�)</param>
	/// <param name="timer_s">�^�C�}�[</param>
	/// <param name="position">���W</param>
	/// <param name="offsetPosition">�I�t�Z�b�g���W(�g��Ȃ�)</param>
	/// <param name="stage">�X�e�[�W�̃|�C���^�[</param>
	static void ReturnPosition(int& routine, float& timer_s, DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& offsetPosition, Stage* stage);
	/// <summary>
	/// ����k��������
	/// </summary>
	/// <param name="routine">���[�`��(�g��Ȃ�)</param>
	/// <param name="timer_s">�^�C�}�[(�g��Ȃ�)</param>
	/// <param name="position">���W(�g��Ȃ�)</param>
	/// <param name="offsetPosition">�I�t�Z�b�g���W</param>
	/// <param name="stage">�X�e�[�W�̃|�C���^�[(�g��Ȃ�)</param>
	static void VibrationPosition(int& routine, float& timer_s, DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& offsetPosition, Stage* stage);
	/// <summary>
	/// ����߂��p�̏���
	/// </summary>
	/// <param name="routine">���[�`��(�g��Ȃ�)</param>
	/// <param name="timer_s">�^�C�}�[(�g��Ȃ�)</param>
	/// <param name="position">���W</param>
	/// <param name="offsetPosition">�I�t�Z�b�g���W(�g��Ȃ�)</param>
	/// <param name="stage">�X�e�[�W�̃|�C���^�[(�g��Ȃ�)</param>
	static void ReturnStartPosition(int& routine, float& timer_s, DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& offsetPosition, Stage* stage);

private:
	//�s���z��̕ۑ��p�z��
	std::vector<int> m_moveBehaviorsNum;
};