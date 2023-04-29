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
	//���W�̎擾
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	//�ړ��ʂ̎擾
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();


	//�����蔻��AABB�擾
	AABBFor3D* aabb = actor->GetAABB();

	//�����蔻��̗̈�
	DirectX::SimpleMath::Vector3 AABBArea = { 0.5f,0.5f,0.75f };
	aabb->SetData(position - AABBArea, position + AABBArea);

	//�o�ߎ���
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	//�ړ�
	actor->SetPosition(position + (velocity * elapsedTime_s));

}
