/*
* 2023/03/21
* MeanderingObstacleSpawner.cpp
* �����@��
* ���̏�Q���̐�������
*/
#include"pch.h"
#include"Game/PlayScene/Actor/Actor.h"
#include"../Obstacle.h"
#include"../../MyRandom.h"
#include"MeanderingObstacleSpawner.h"

/// <summary>
/// �֍s���鉊�̏�Q���̐���
/// </summary>
/// <param name="actors">��Q���̔z��</param>
/// <param name="position">��������ʒu</param>
/// <param name="rotation">�p�x�i���W�A���j</param>
/// <param name="behavior">�֍s���鉊�̃r�w�C�r�A</param>
/// <param name="model">�֍s���鉊�̃��f���iNULL��OK�j</param>
/// <returns>true=���������@false=�������s</returns>
bool MeanderingObstacleSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& rotation, IBehavior* behavior, DirectX::Model* model)
{
	static const float SPEED = 4.0f;

	for (std::unique_ptr<Actor>& actor : actors)
	{
		//�g�p���łȂ���΁A�G������������
		if (actor->IsActive())
			continue;


		//�ړ��x�N�g��
		DirectX::SimpleMath::Vector3 velocity;

		velocity = actor->GetPlayerPosition() - position;

		velocity.Normalize();

		velocity *= SPEED;

		//��Q���ɂ̃^�C�v�ݒ�
		actor->SetTypeInt(static_cast<int>(Obstacle::ObstacleType::MEANDERING));

		//�X�P�[��
		DirectX::SimpleMath::Vector3 scale;


		//��Q���̏�����
		actor->Initialize(velocity, position, scale, rotation, true, behavior, model);

		//��Q���̐����ɐ���
		return true;

	}
	//���ׂď�Q�����g�p��
	return false;
}