/*
* 2023/04/21
* BirdBehavior.cpp
* �����@��
* ���̏�Q���̓���
*/
#include"pch.h"
#include"../../Actor/Actor.h"
#include"../Obstacle.h"
#include"BirdBehavior.h"

/// <summary>
/// ���̏�Q���̓���
/// </summary>
/// <param name="timer">�^�C�}�[</param>
/// <param name="actor">��Q���̃|�C���^�[</param>
void BirdBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	//90�x
	static const float NINETY_ANGLE = NumRoundDownSecond(DirectX::XM_PI / 2.0f);
	//180�x
	static const float ONE_HUNDRED_EIGHTY_ANGLE = NumRoundDownSecond(DirectX::XM_PI);
	//270�x
	static const float TWO_HUNDRED_SEVENTY_ANGLE = NumRoundDownSecond(NINETY_ANGLE + ONE_HUNDRED_EIGHTY_ANGLE);


	//���W�̎擾
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	//�ړ��ʂ̎擾
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();
	//�p�x�擾(�f�O���[)
	float angle = NumRoundDownSecond(actor->GetRotation().ToEuler().y);
	angle = std::abs(angle);

	//�����蔻��AABB�擾
	AABBFor3D* aabb = actor->GetAABB();


	//�����蔻��̗̈�
	DirectX::SimpleMath::Vector3 AABBArea;

	//�p�x�ɂ���ē����蔻��̗̈�̐ݒ�
	//0 OR 180
	if (angle == 0.0f || angle == ONE_HUNDRED_EIGHTY_ANGLE)
	{
		AABBArea = { 0.5f,0.5f,0.75f };

		aabb->SetData(position - AABBArea, position + AABBArea);

	}
	//90 OR 270
	else if (angle == NINETY_ANGLE || angle == ONE_HUNDRED_EIGHTY_ANGLE + NINETY_ANGLE)
	{
		AABBArea = { 1.5f,1.5f,1.4f };

		aabb->SetData(position - AABBArea, position + AABBArea);
	}

	//�o�ߎ���
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	//�ړ�
	actor->SetPosition(position + (velocity * elapsedTime_s));

}

/// <summary>
/// �����_��O�ȉ��؂�̂�
/// </summary>
/// <param name="num">�؂�̂Ă�������</param>
/// <returns>�؂�̂Ă�����</returns>
float BirdBehavior::NumRoundDownSecond(float num)
{
	//������S�{����
	num *= 100;
	//�����_�S�Ă�؂�̂�
	num = std::floor(num);
	//������S�Ŋ���
	num /= 100;

	return num;
}
