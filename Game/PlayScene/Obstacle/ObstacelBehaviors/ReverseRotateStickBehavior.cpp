/*
* 2023/03/21
* ReverseRotateStickBehavior.cpp
* ���v���ɉ�]����_�̓���
* �����@��
*/
#include"pch.h"
#include"../../Actor/Actor.h"
#include"../Obstacle.h"
#include"ReverseRotateStickBehavior.h"

/// <summary>
/// ���v���ɉ�]����_�̓���
/// </summary>
/// <param name="timer">�^�C�}�[</param>
/// <param name="actor">��Q���̃|�C���^�[�ithis�j</param>
void ReverseRotateStickBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	UNREFERENCED_PARAMETER(timer);

	//��]���x�����񂾂�グ��
	static const float ROT_UP_SPEED = 0.000001f;
	//�J�v�Z���̒���
	static const float CAPSULE_LENGTH = 15.0f;

	//���W�̎擾
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();

	//�A�N�^�[�^�����Q���^�Ƀ_�C�i�~�b�N�L���X�g
	Obstacle* obstacle = dynamic_cast<Obstacle*>(actor);
	
	//�p�x�̎擾
	DirectX::SimpleMath::Vector3 rot = actor->GetRotation().ToEuler();
	//��]���x�̎擾
	float speed = obstacle->GetRotSpeed();
	//��]���x�̏グ��
	speed -= ROT_UP_SPEED;

	//�p�x�ɉ�]���x�𑫂�
	rot.y += speed;
	//�p�x�̐ݒ�
	actor->SetRotation(rot);
	//��]���x�̐ݒ�
	obstacle->SetRotSpeed(speed);
	//�����蔻��J�v�Z���̎擾
	Capsule* cap = actor->GetCapsule();
	//����̗̈�̐ݒ�
	cap->a = DirectX::SimpleMath::Vector3(0.0f, position.y, 0.0f);
	//���݂̊p�x�Ōv�Z����
	cap->b = DirectX::SimpleMath::Vector3(cos(rot.y) * CAPSULE_LENGTH, position.y, -sin(rot.y) * CAPSULE_LENGTH);

}