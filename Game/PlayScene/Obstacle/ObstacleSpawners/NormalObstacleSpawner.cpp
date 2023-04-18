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
/// <param name="angle">�p�x�i���W�A���j</param>
/// <param name="behavior">���̃r�w�C�r�A</param>
/// <param name="model">���̃��f���iNULL��OK�j</param>
/// <param name="commonState">�R�����X�e�[�g</param>
/// <returns>true=���������@false=�������s</returns>
bool NormalObstacleSpawner::Create(std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//�g�p���łȂ���΁A�G������������
		if (actor->IsActive())
			continue;


		Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
		if (obstale != nullptr)
		{
			obstale->SetType(Obstacle::ObstacleType::NORMAL);

		}
		DirectX::SimpleMath::Vector3 velocity;
		velocity.x = -sin(angle) * 0.1;
		velocity.z = cos(angle) * 0.1;
		DirectX::SimpleMath::Vector3 scale;

		//�A�C�e���̏�����
		actor->Initialize(velocity, position, scale, true, angle, behavior, model, commonState);

		


		//�A�C�e���̐����ɐ���
		return true;

	}
	//���ׂăA�C�e�����g�p��
	return false;
}