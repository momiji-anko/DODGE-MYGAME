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
/// <param name="angle">�p�x�i���W�A���j</param>
/// <param name="behavior">�_�̃r�w�C�r�A</param>
/// <param name="model">�_�̃��f��</param>
/// <param name="commonState">�R�����X�e�[�g</param>
/// <returns>true=���������@false=�������s</returns>
bool StickObstacleSpawner::Create(std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//�g�p���łȂ���΁A�G������������
		if (actor->IsActive())
			continue;

		//��Q���^�Ƀ_�C�i�~�b�N�L���X�g����
		Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
		//�L���X�g�o������
		if (obstale != nullptr)
		{
			//��Q���ɂ̃^�C�v�ݒ�
			obstale->SetType(Obstacle::ObstacleType::STICK);

		}

		DirectX::SimpleMath::Vector3 scale;

		//��Q���̏�����
		actor->Initialize(DirectX::SimpleMath::Vector3(-3.0f, 0.0f, 0.0f), position, scale, true, angle, behavior, model, commonState);

		//��Q���̐����ɐ���
		return true;

	}
	//���ׂăA�C�e�����g�p��
	return false;
}