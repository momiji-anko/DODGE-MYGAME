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

	//�����蔻��̗̈�
	DirectX::SimpleMath::Vector3 AABBArea{ 0.3f,0.5f,0.3f };


	//�����蔻��̗̈�ݒ�
	actor->GetAABB()->SetData(position - AABBArea, position + AABBArea);
	
	//�o�ߎ���
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	//�ړ�����
	actor->SetPosition(position + (velocity * elapsedTime_s));
}
