/*
* 2023/03/21
* RotateStickObstacleSpawner.cpp
* �����@��
* ��]����_�̏�Q���̐�������
*/
#include"pch.h"
#include"Game/PlayScene/Actor/Actor.h"
#include"../Obstacle.h"
#include"../../MyRandom.h"
#include"RotateStickObstacleSpawner.h"


/// <summary>
/// ��]����_�̏�Q���̐���
/// </summary>
/// <param name="actors">��Q���̔z��</param>
/// <param name="position">��������ʒu</param>
/// <param name="rotation">�p�x�i���W�A���j</param>
/// <param name="behavior">��]����_�̃r�w�C�r�A</param>
/// <param name="model">��]����_�̃��f��</param>
/// <returns>true=���������@false=�������s</returns>
bool RotateStickObstacleSpawner::Create(std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& rotation, IBehavior* behavior, DirectX::Model* model)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//�g�p���łȂ���΁A�G������������
		if (actor->IsActive())
			continue;

		//��]���鑬�x�̐ݒ�
		actor->SetRotSpeed(0.01f);

		//��Q���ɂ̃^�C�v�ݒ�
		actor->SetTypeInt(static_cast<int>(Obstacle::ObstacleType::ROTATESTICK));

		//�X�P�[��
		DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3(0.005f, 0.005f, 0.005f);

		//��Q��������
		actor->Initialize(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), position, scale, rotation, true, behavior, model);

		//��Q���̐����ɐ���
		return true;

	}
	//���ׂď�Q�����g�p��
	return false;
}
