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
	//�U�̐�������
	for (int i = 0; i < 6; i++)
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
				obstale->SetType(Obstacle::ObstacleType::BIRD);

			}
			//�ړ��x�N�g��
			DirectX::SimpleMath::Vector3 velocity = DirectX::SimpleMath::Vector3(-10.0f, 0.0f, 0.0f);

			
			//��Q������]������i���ˊp�x���v�Z����s������j
			DirectX::SimpleMath::Matrix rotetion = DirectX::SimpleMath::Matrix::CreateRotationY(rotation.y);
			//���x���v�Z����i���ˊp�x�����������ړ����x���v�Z����j
			velocity = DirectX::SimpleMath::Vector3::TransformNormal(velocity, rotetion);


			//1�̂Â̃|�W�V�����̌v�Z
			DirectX::SimpleMath::Vector3 pos;
			pos = position;
			//x���O�ł���Ή����ɂȂ�悤�ɂ���
			if (pos.x == 0.0f)
			{
				pos.x = (i - 2) * 4.0f;
			}
			//�����O�ł���Ώc���ɂȂ�悤�ɂ���
			if (pos.z == 0.0f)
			{
				pos.z = (i - 2) * 4.0f;
			}
			//�X�P�[��
			DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3(0.005f, 0.005f, 0.005f);


			//��Q���̏�����
			actor->Initialize(velocity, pos, scale, rotation, true, behavior, model, commonState);

			DirectX::SimpleMath::Quaternion rotation;

			rotation = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, rotation.y + DirectX::XM_PI);
			
			//�p�x�̐ݒ�
			actor->SetRotation(rotation);


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
