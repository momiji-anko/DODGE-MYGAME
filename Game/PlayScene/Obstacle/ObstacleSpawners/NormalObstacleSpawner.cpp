/*
* 2023/03/21
* BirdObstacleSpawner.cpp
* �����@��
* ���̏�Q���̐�������
*/
#include"pch.h"
#include"Game/PlayScene/Actor/Actor.h"
#include"../Obstacle.h"
#include"../../MyRandom.h"
#include"NormalObstacleSpawner.h"

/// <summary>
/// ���̏�Q���̐���
/// </summary>
/// <param name="actors">��Q���̔z��</param>
/// <param name="position">��������ʒu</param>
/// <param name="rotation">�p�x�i���W�A���j</param>
/// <param name="behavior">���̃r�w�C�r�A</param>
/// <param name="model">���̃��f���iNULL��OK�j</param>
/// <returns>true=���������@false=�������s</returns>
bool NormalObstacleSpawner::Create(std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& rotation, IBehavior* behavior, DirectX::Model* model)
{

	static const float SPEED = 4.0f;

	for (std::unique_ptr<Actor>& actor : actors)
	{
		//�g�p���łȂ���΁A�G������������
		if (actor->IsActive())
			continue;

		//�ړ��x���V�e�B
		DirectX::SimpleMath::Vector3 velocity;

		velocity = actor->GetPlayerPosition() - position;

		velocity.Normalize();
		
		//��Q���ɂ̃^�C�v�ݒ�
		actor->SetTypeInt(static_cast<int>(Obstacle::ObstacleType::NORMAL));

		velocity *= SPEED;
		

		//�X�P�[��
		DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3::One;

		//�A�C�e���̏�����
		actor->Initialize(velocity, position, scale,rotation, true, behavior, model);

		//�A�C�e���̐����ɐ���
		return true;

	}
	//���ׂăA�C�e�����g�p��
	return false;
}