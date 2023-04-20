/*
* 2023/03/21
* StickBehavior.cpp
* �����@��
* �_�̓���
*/
#include"pch.h"
#include"../../Actor/Actor.h"
#include"../Obstacle.h"
#include"StickBehavior.h"

/// <summary>
/// �_�̓���
/// </summary>
/// <param name="timer">�^�C�}�[</param>
/// <param name="actor">��Q���̃|�C���^�[�ithis�j</param>
void StickBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	//���W�̎擾
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	//�ړ��ʂ̎擾
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();

	//�����蔻��AABB�̎擾
	AABBFor3D* aabb = actor->GetAABB();
	//�p�x�̎擾
	float angle = std::abs(actor->GetRotation().ToEuler().y);
	
	float rot = std::floor(angle);

	//�p�x�ɂ���ē����蔻��̗̈�̐ݒ�
 	//0 OR 180�ł����
	if (rot == 0.0f || rot == std::floor(DirectX::XM_PI))
	{
		aabb->SetData(DirectX::SimpleMath::Vector3(position.x - 0.5f, position.y - 0.0f, position.z - 12.0f), DirectX::SimpleMath::Vector3(position.x + 0.5f, position.y + 1.5f, position.z + 12.0f));	
	}
	//90 OR 270�ł����
	else if (rot == std::floor((DirectX::XM_PI / 2.0f)) || rot == std::floor((DirectX::XM_PI + (DirectX::XM_PI / 2.0f))))
	{
		aabb->SetData(DirectX::SimpleMath::Vector3(position.x - 12.0f, position.y - 0.0f, position.z - 0.4f), DirectX::SimpleMath::Vector3(position.x + 12.0f, position.y + 1.0f, position.z + 0.4f));
	}

	//�ړ�����
	actor->SetPosition(position + (velocity * timer.GetElapsedSeconds()));

}