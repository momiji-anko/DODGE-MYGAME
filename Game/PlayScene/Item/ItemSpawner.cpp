#include"pch.h"
#include"ItemSpawner.h"
#include"../Actor/Actor.h"
#include"Item.h"

/// <summary>
/// 盾のアイテムのスポナー
/// </summary>
/// <param name="actors">アイテムの配列</param>
/// <param name="position">アイテムを生成する座標</param>
/// <param name="angle">角度（ラジアン）</param>
/// <param name="behavior">NULLでOK</param>
/// <param name="model">アイテムのモデル</param>
/// <param name="commonState">コモンステート</param>
/// <returns>true＝生成成功　false＝生成失敗</returns>
bool ItemSpawner::Create( std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior,DirectX::Model* model,DirectX::CommonStates* commonState)
{
	UNREFERENCED_PARAMETER(behavior);

	for ( std::unique_ptr<Actor>& actor : actors)
	{
		//使用中でなければ、敵を初期化する
		if (actor->IsActive())
			continue;

		//アクターからアイテム型にダイナミックキャスト
		Item* item = dynamic_cast<Item*>(actor.get());
		//NULLチェック
		if (item != nullptr)
		{
			//アイテムタイプの設定
			item->SetItemType(Item::ItemType::SHIELD_ITEM);
		}

		//アイテムの初期化
		actor->Initialize(DirectX::SimpleMath::Vector3(0.0f, -3.0f, 0.0f), position, true, angle, nullptr, model, commonState);

		//アイテムの生成に成功
		return true;

	}
	//すべてアイテムが使用中
	return false;
}
