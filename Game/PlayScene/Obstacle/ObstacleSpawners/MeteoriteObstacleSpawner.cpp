/*
* 2023/03/21
* MeteoriteObstacleSpawner.cpp
* �����@��
* 覐΂̏�Q���̐�������
*/
#include"pch.h"
#include"Game/PlayScene/Actor/Actor.h"
#include"../Obstacle.h"
#include"../../MyRandom.h"
#include"MeteoriteObstacleSpawner.h"

/// <summary>
/// 覐΂̏�Q���̐���
/// </summary>
/// <param name="actors">��Q���̔z��</param>
/// <param name="position">��������ʒu</param>
/// <param name="rotation">�p�x�i���W�A���j</param>
/// <param name="behavior">覐΂̃r�w�C�r�A</param>
/// <param name="model">覐΂̃��f��</param>
/// <returns>true=���������@false=�������s</returns>
bool MeteoriteObstacleSpawner::Create(std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& rotation, IBehavior* behavior, DirectX::Model* model)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//�g�p���łȂ���΁A�G������������
		if (actor->IsActive())
			continue;

		//��Q���ɂ̃^�C�v�ݒ�
		actor->SetTypeInt(static_cast<int>(Obstacle::ObstacleType::METEORITE));

		//�ړ��x�N�g��
		DirectX::SimpleMath::Vector3 velocity = { 0.0f, -1.0f, 0.0f };
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
