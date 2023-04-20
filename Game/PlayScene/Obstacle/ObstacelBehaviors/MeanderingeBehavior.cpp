/*
* 2023/03/21
* MeanderingBehavior.cpp
* �����@��
* �֍s���鉊�̃r�w�C�r�A�[�N���X
*/
#include"pch.h"
#include"../../Actor/Actor.h"
#include"../Obstacle.h"
#include"MeanderingeBehavior.h"


/// <summary>
/// �֍s���鉊�̌ŗL�̍s��
/// </summary>
/// <param name="timer">�^�C�}�[</param>
/// <param name="actor">��Q���̃|�C���^�[�ithis�j</param>
void MeanderingBehavior::Execute(const DX::StepTimer& timer, Actor* actor)
{
	//�ړ����x
	const float MOVE_SPEED = 4.0f;
	//1�t���[���̌o�ߎ���
	float elapsedTime = timer.GetElapsedSeconds();
	//���W�̎擾
	DirectX::SimpleMath::Vector3 position = actor->GetPosition();
	//�ړ��ʂ̎擾
	DirectX::SimpleMath::Vector3 velocity = actor->GetVelocity();
	//�p�x�̎擾
	float angle = actor->GetRotation().y;
	//�A�N�^�[�^�����Q���^�Ƀ_�C�i�~�b�N�L���X�g
	Obstacle* obstacle = dynamic_cast<Obstacle*>(actor);

	//NULL�`�F�b�N
	if (obstacle == nullptr)
	{
		return;
	}
	//�G�t�F�N�g�X�V
	obstacle->GetEffect()->Update(timer);

	//�֍s�^�C���擾
	float seekTime_s = obstacle->GetSeekTime();
	
	//�֍s�x���V�e�B
	DirectX::SimpleMath::Vector3 meanderingVelocity{ velocity.z,0,-velocity.x };

	//�Q�Ŋ��������܂肪�O�ł���΃x�N�g���𔽓]
	if (static_cast<int>(seekTime_s) % 2 != 0)
	{
		meanderingVelocity *= -1;
	}

	//�����蔻��AABB�̃G���A�ݒ�
	actor->GetAABB()->SetData(DirectX::SimpleMath::Vector3(position.x - 0.3f, position.y - 0.5f, position.z - 0.3f), DirectX::SimpleMath::Vector3(position.x + 0.3f, position.y + 0.5f, position.z + 0.3f));
	
	//�֍s�x���V�e�B�̉e���͂����߂邽�߂�MOVE_SPEED�ł�����
	meanderingVelocity *= MOVE_SPEED;
	//�x���V�e�B�Ɏ֍s�x���V�e�B�𑫂�
	velocity += meanderingVelocity;
	//�x���V�e�B�𐳋K��
	velocity.Normalize();
	//�x���V�e�B�̈ړ��ʌv�Z
	velocity *= MOVE_SPEED;
	//�x���V�e�B�ɂP�t���[���̕b����������
	velocity *= elapsedTime;

	//�ړ�����
	actor->SetPosition(DirectX::SimpleMath::Vector3(position.x + velocity.x, position.y, position.z + velocity.z));

}