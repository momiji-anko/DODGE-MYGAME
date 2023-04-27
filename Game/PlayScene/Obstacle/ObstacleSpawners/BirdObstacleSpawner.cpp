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
#include"BirdObstacleSpawner.h"


/// <summary>
/// ���̏�Q���̐���
/// </summary>
/// <param name="actors">��Q���̔z��</param>
/// <param name="position">��������ʒu</param>
/// <param name="rotation">�p�x�i���W�A���j</param>
/// <param name="behavior">���̃r�w�C�r�A</param>
/// <param name="model">���̃��f��</param>
/// <param name="commonState">�R�����X�e�[�g</param>
/// <returns>true=���������@false=�������s</returns>
bool BirdObstacleSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& rotation, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	//�X�|�[���J�E���g
	int spawnCount = 0;

	//�����m�̋���
	static const float BIRD_TO_BIRD_DISTANCE = 4.0f;

	//�U�̐�������
	for (int i = 0; i < 6; i++)
	{
		for (std::unique_ptr<Actor>& actor : actors)
		{
			//�g�p���łȂ���΁A�G������������
			if (actor->IsActive())
				continue;

		
			//��Q���ɂ̃^�C�v�ݒ�
			actor->SetTypeInt(static_cast<int>(Obstacle::ObstacleType::BIRD));
			
			//�ړ��x�N�g��
			DirectX::SimpleMath::Vector3 velocity = DirectX::SimpleMath::Vector3(-10.0f, 0.0f, 0.0f);

			
			//��Q������]������i���ˊp�x���v�Z����s������j
			DirectX::SimpleMath::Matrix rotetion = DirectX::SimpleMath::Matrix::CreateRotationY(rotation.y);
			//���x���v�Z����i���ˊp�x�����������ړ����x���v�Z����j
			velocity = DirectX::SimpleMath::Vector3::TransformNormal(velocity, rotetion);


			//1�̂Â̃|�W�V�����̌v�Z
			DirectX::SimpleMath::Vector3 pos;
			pos = position;

			//���ɂȂ�p�̕␳
			int correction = 2;

			//x���O�ł���Ή����ɂȂ�悤�ɂ���
			if (pos.x == 0.0f)
			{
				pos.x = (i - correction) * BIRD_TO_BIRD_DISTANCE;
			}
			//�����O�ł���Ώc���ɂȂ�悤�ɂ���
			if (pos.z == 0.0f)
			{
				pos.z = (i - correction) * BIRD_TO_BIRD_DISTANCE;
			}
			//�X�P�[��
			DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3::One;


			//��Q���̏�����
			actor->Initialize(velocity, pos, scale, rotation, true, behavior, model, commonState);

			//��Q���̐����ɐ���
			spawnCount++;
			break;

		}

	}
	//�X�|�[���J�E���g���U�ȏ�ł���ΑS�Đ����ł���
	if (spawnCount >= 6)
	{
		return true;
	}

	//���ׂď�Q�����g�p��
	return false;
}
