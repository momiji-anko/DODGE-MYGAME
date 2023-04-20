/*
* 2023/03/21
* ReverseRotateStickObstacleSpawner.cpp
* �����@��
* �����v���ɉ�]����_�̏�Q���̐�������
*/
#include"pch.h"
#include"Game/PlayScene/Actor/Actor.h"
#include"../Obstacle.h"
#include"../../MyRandom.h"
#include"ReverseRotateStickObstacleSpawner.h"


/// <summary>
/// �����v���ɉ�]����_�̏�Q���̐���
/// </summary>
/// <param name="actors">��Q���̔z��</param>
/// <param name="position">��������ʒu</param>
/// <param name="rotation">�p�x�i���W�A���j</param>
/// <param name="behavior">��]����_�̃r�w�C�r�A</param>
/// <param name="model">��]����_�̃��f��</param>
/// <param name="commonState">�R�����X�e�[�g</param>
/// <returns>true=���������@false=�������s</returns>
bool ReverseRotateStickObstacleSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& rotation, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
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
			obstale->SetType(Obstacle::ObstacleType::REVERSE_ROTATESTICK);
			//��]���鑬�x�̐ݒ�
			obstale->SetRotSpeed(-0.005f);
		}
		//�X�P�[��
		DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3(0.005f, 0.005f, 0.005f);

		//��Q���̏�����
		actor->Initialize(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), position, scale, rotation, true, behavior, model, commonState);

		//��Q���̐����ɐ���
		return true;

	}
	//���ׂď�Q�����g�p��
	return false;
}