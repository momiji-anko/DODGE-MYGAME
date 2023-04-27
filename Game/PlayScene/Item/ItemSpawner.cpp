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
/// <param name="commonState">�R�����X�e�[�g</param>
/// <returns>true�����������@false���������s</returns>
bool ItemSpawner::Create( std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& rotation, IBehavior* behavior,DirectX::Model* model,DirectX::CommonStates* commonState)
{
	UNREFERENCED_PARAMETER(behavior);

	for (std::unique_ptr<Actor>& actor : actors)
	{
		//�g�p���łȂ���΁A�G������������
		if (actor->IsActive())
			continue;

		
		actor->SetTypeInt(static_cast<int>(Item::ItemType::SHIELD_ITEM));

		DirectX::SimpleMath::Vector3 scale = { 0.008f,0.008f ,0.008f };

		DirectX::SimpleMath::Vector3 velocity = { 0.0f, -3.0f, 0.0 };


		//�A�C�e���̏�����
		actor->Initialize(velocity, position, scale, rotation, true, behavior, model, commonState);

		//�A�C�e���̐����ɐ���
		return true;

	}
	//���ׂăA�C�e�����g�p��
	return false;
}
