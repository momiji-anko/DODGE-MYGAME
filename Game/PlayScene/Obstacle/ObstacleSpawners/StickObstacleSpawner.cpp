/*
* 2023/03/21
* StickObstacleSpawner.cpp
* �����@��
* �_�̏�Q���̐�������
*/
#include"pch.h"
#include"Game/PlayScene/Actor/Actor.h"
#include"../Obstacle.h"
#include"../../MyRandom.h"
#include"StickObstacleSpawner.h"

/// <summary>
/// �_�̏�Q���̐���
/// </summary>
/// <param name="actors">��Q���̔z��</param>
/// <param name="position">��������ʒu</param>
/// <param name="rotation">�p�x�i���W�A���j</param>
/// <param name="behavior">�_�̃r�w�C�r�A</param>
/// <param name="model">�_�̃��f��</param>
/// <param name="commonState">�R�����X�e�[�g</param>
/// <returns>true=���������@false=�������s</returns>
bool StickObstacleSpawner::Create(std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& rotation, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	static const float SPEED = 2.0f;

	for (std::unique_ptr<Actor>& actor : actors)
	{
		//�g�p���łȂ���΁A�G������������
		if (actor->IsActive())
			continue;


		//��Q���ɂ̃^�C�v�ݒ�
		actor->SetTypeInt(static_cast<int>(Obstacle::ObstacleType::STICK));

		//�^�񒆂���o���ʒu�̃x�N�g���쐬
		DirectX::SimpleMath::Vector3 vel = (DirectX::SimpleMath::Vector3::Zero - position);
		//���K������
		vel.Normalize();
		//�X�P�[��
		DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3::One;

		//��Q���̏�����
		actor->Initialize(vel * SPEED, position, scale, rotation, true, behavior, model, commonState);

		//��Q���̐����ɐ���
		return true;

	}
	//���ׂăA�C�e�����g�p��
	return false;
}