/*
* 2023/03/21
* NormalBehavior.cpp
* �����@��
* ���ʂ̉��̓���
*/
#include"pch.h"
#include"../../Actor/Actor.h"
#include"../Obstacle.h"
#include"NormalBehavior.h"

/// <summary>
/// ���ʂ̉��̓���
/// </summary>
/// <param name="timer">�^�C�}�[</param>
/// <param name="actor">��Q���̃|�C���^�[�ithis�j</param>
void NormalBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	//���W�̎擾
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	//�ړ��ʂ̎擾
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();
	
	//�����蔻��̗̈�
	DirectX::SimpleMath::Vector3 AABBArea{ 0.3f,0.5f,0.3f };


	//�����蔻��AABB�̓����蔻��̈�̐ݒ�
	actor->GetAABB()->SetData(position - AABBArea, position + AABBArea);

	//�o�ߎ���
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	//�ړ�����
	actor->SetPosition(position + (velocity * elapsedTime_s));

	//Actor�^��Obstacle�^�Ƀ_�C�i�~�b�N�L���X�g����
	Obstacle* obs = dynamic_cast<Obstacle*>(actor);
	//�L���X�g�ɐ������Ă���΃G�t�F�N�g���X�V����
	if (obs != nullptr&& obs->GetEffect() != nullptr)
	{
		//�G�t�F�N�g�X�V
		obs->GetEffect()->Update(timer);
	}
	

}


