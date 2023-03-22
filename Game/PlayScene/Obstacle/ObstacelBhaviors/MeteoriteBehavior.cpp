/*
* 2023/03/21
* MeteoriteBehavior.cpp
* �����@��
* 覐΂̓���
*/
#include"pch.h"
#include"../../Actor/Actor.h"
#include"../Obstacle.h"
#include"MeteoriteBehavior.h"

/// <summary>
/// 覐΂̓���
/// </summary>
/// <param name="timer">�^�C�}�[</param>
/// <param name="actor">��Q���̃|�C���^�[�ithis�j</param>
void MeteoriteBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	//���W�̎擾
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	//�ړ��ʂ̎擾
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();
	
	//�n�ʂɒ����������
	if (position.y <= -1.0f)
	{
		actor->SetActive(false);
	}

	//�����蔻��AABB�擾
	AABBFor3D* aabb = actor->GetAABB();
	//�����蔻��̗̈�ݒ�
	actor->GetAABB()->SetData(DirectX::SimpleMath::Vector3(position.x - 0.3f, position.y - 0.5f, position.z - 0.3f), DirectX::SimpleMath::Vector3(position.x + 0.3f, position.y + 0.5f, position.z + 0.3f));
	
	//�ړ�����
	actor->SetPosition(position + (velocity * timer.GetElapsedSeconds()));
}
