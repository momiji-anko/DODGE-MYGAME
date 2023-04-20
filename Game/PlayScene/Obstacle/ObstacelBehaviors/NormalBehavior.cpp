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
	
	//�����蔻��AABB�̓����蔻��̈�̐ݒ�
	actor->GetAABB()->SetData(DirectX::SimpleMath::Vector3(position.x - 0.3f, position.y - 0.5f, position.z - 0.3f), DirectX::SimpleMath::Vector3(position.x + 0.3f, position.y + 0.5f, position.z + 0.3f));

	//�ړ�����
	actor->SetPosition(position + (velocity * timer.GetElapsedSeconds()));

	//Actor�^��Obstacle�^�Ƀ_�C�i�~�b�N�L���X�g����
	Obstacle* obs = dynamic_cast<Obstacle*>(actor);
	//�L���X�g�ɐ������Ă���΃G�t�F�N�g���X�V����
	if (obs != nullptr&& obs->GetEffect() != nullptr)
	{
		//�G�t�F�N�g�X�V
		obs->GetEffect()->Update(timer);
	}
	

}



