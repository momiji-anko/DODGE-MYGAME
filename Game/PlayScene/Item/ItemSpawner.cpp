#include"pch.h"
#include"ItemSpawner.h"
#include"../Actor/Actor.h"
#include"Item.h"
bool ItemSpawner::Create( std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior,DirectX::Model* model,DirectX::CommonStates* commonState)
{
	for ( std::unique_ptr<Actor>& actor : actors)
	{
		//�g�p���łȂ���΁A�G������������
		if (actor->IsActive())
			continue;

		//�A�C�e���̏�����
		actor->Initialize(DirectX::SimpleMath::Vector3(0.0f, -3.0f, 0.0f), position, true, angle, nullptr, model, commonState);

		Item* item = dynamic_cast<Item*>(actor.get());
		if (item != nullptr)
		{
			item->SetItemType(Item::ItemType::INVINCIBLE_ITEM);
		}


		//�A�C�e���̐����ɐ���
		return true;

	}
	//���ׂăA�C�e�����g�p��
	return false;
}

bool SlipItemSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//�g�p���łȂ���΁A�G������������
		if (actor->IsActive())
			continue;

		//�A�C�e���̏�����
		actor->Initialize(DirectX::SimpleMath::Vector3(0.0f, -3.0f, 0.0f), position, true, angle, nullptr, model, commonState);

		Item* item = dynamic_cast<Item*>(actor.get());
		if (item == nullptr)
		{
			item->SetItemType(Item::ItemType::SLIP_ITEM);
		}


		//�A�C�e���̐����ɐ���
		return true;

	}
	//���ׂăA�C�e�����g�p��
	return false;
}
