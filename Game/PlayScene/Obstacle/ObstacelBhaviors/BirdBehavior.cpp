/*
* 2023/03/21
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
	//���W�̎擾
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	//�ړ��ʂ̎擾
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();
	//�p�x�擾
	float angle = actor->GetAngle();

	//�����蔻��AABB�擾
	AABBFor3D* aabb = actor->GetAABB();

	//�p�x�ɂ���ē����蔻��̗̈�̐ݒ�
	//0 OR 180�ł����
	if (angle == 0.0f || angle == DirectX::XM_PI)
	{
		aabb->SetData(DirectX::SimpleMath::Vector3(position.x - 0.5f, position.y - 0.5f, position.z - 0.75f), DirectX::SimpleMath::Vector3(position.x + 0.5f, position.y + 0.5f, position.z + 0.75f));

	}
	//90 OR 270�ł����
	else if (angle == DirectX::XM_PI / 2.0f || angle == DirectX::XM_PI + (DirectX::XM_PI / 2.0f))
	{
		aabb->SetData(DirectX::SimpleMath::Vector3(position.x - 0.5f, position.y - 0.5f, position.z - 0.4f), DirectX::SimpleMath::Vector3(position.x + 0.5f, position.y + 0.5f, position.z + 0.4f));
	}

	//�ړ�
	actor->SetPosition(position + (velocity * timer.GetElapsedSeconds()));

}