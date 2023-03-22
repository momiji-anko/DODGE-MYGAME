#include"pch.h"
#include"ItemSpawner.h"
#include"../Actor/Actor.h"
#include"Item.h"

/// <summary>
/// ���̃A�C�e���̃X�|�i�[
/// </summary>
/// <param name="actors">�A�C�e���̔z��</param>
/// <param name="position">�A�C�e���𐶐�������W</param>
/// <param name="angle">�p�x�i���W�A���j</param>
/// <param name="behavior">NULL��OK</param>
/// <param name="model">�A�C�e���̃��f��</param>
/// <param name="commonState">�R�����X�e�[�g</param>
/// <returns>true�����������@false���������s</returns>
bool ItemSpawner::Create( std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior,DirectX::Model* model,DirectX::CommonStates* commonState)
{
	UNREFERENCED_PARAMETER(behavior);

	for ( std::unique_ptr<Actor>& actor : actors)
	{
		//�g�p���łȂ���΁A�G������������
		if (actor->IsActive())
			continue;

		//�A�N�^�[����A�C�e���^�Ƀ_�C�i�~�b�N�L���X�g
		Item* item = dynamic_cast<Item*>(actor.get());
		//NULL�`�F�b�N
		if (item != nullptr)
		{
			//�A�C�e���^�C�v�̐ݒ�
			item->SetItemType(Item::ItemType::SHIELD_ITEM);
		}

		//�A�C�e���̏�����
		actor->Initialize(DirectX::SimpleMath::Vector3(0.0f, -3.0f, 0.0f), position, true, angle, nullptr, model, commonState);

		//�A�C�e���̐����ɐ���
		return true;

	}
	//���ׂăA�C�e�����g�p��
	return false;
}
