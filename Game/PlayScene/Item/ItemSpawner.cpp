/*
* 2023/04/14
* ItemSpawner.cpp
* �A�C�e���X�|�i�[
* �����@��
*/
#include"pch.h"
#include"ItemSpawner.h"
#include"../Actor/Actor.h"
#include"Item.h"

/// <summary>
/// ���̃A�C�e���̃X�|�i�[
/// </summary>
/// <param name="actors">�A�C�e���̔z��</param>
/// <param name="position">�A�C�e���𐶐�������W</param>
/// <param name="rotation">�p�x�i���W�A���j</param>
/// <param name="behavior">NULL��OK</param>
/// <param name="model">�A�C�e���̃��f��</param>
/// <returns>true�����������@false���������s</returns>
bool ItemSpawner::Create( std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& rotation, IBehavior* behavior,DirectX::Model* model)
{

	for (std::unique_ptr<Actor>& actor : actors)
	{
		//�g�p���łȂ���΁A�G������������
		if (actor->IsActive())
			continue;

		//�^�C�v�̐ݒ�
		actor->SetTypeInt(static_cast<int>(Item::ItemType::SHIELD_ITEM));
		//�g�嗦
		DirectX::SimpleMath::Vector3 scale = { 0.008f,0.008f ,0.008f };
		//�ړ���
		DirectX::SimpleMath::Vector3 velocity = { 0.0f, -3.0f, 0.0 };


		//�A�C�e���̏�����
		actor->Initialize(velocity, position, scale, rotation, true, behavior, model);

		//�A�C�e���̐����ɐ���
		return true;

	}
	//���ׂăA�C�e�����g�p��
	return false;
}
