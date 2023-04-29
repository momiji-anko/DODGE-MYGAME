#pragma once 
#include"../ISpawner.h"

class ItemSpawner :public ISpawner
{
public:
	ItemSpawner() = default;
	 ~ItemSpawner()override = default;
	/// <summary>
	/// 盾のアイテムのスポナー
	/// </summary>
	/// <param name="actors">アイテムの配列</param>
	/// <param name="position">アイテムを生成する座標</param>
	/// <param name="rotation">角度（ラジアン）</param>
	/// <param name="behavior">NULLでOK</param>
	/// <param name="model">アイテムのモデル</param>
	/// <returns>true＝生成成功　false＝生成失敗</returns>
	bool Create( std::vector< std::unique_ptr<Actor>>& actors,
		const DirectX::SimpleMath::Vector3 & position,
		const DirectX::SimpleMath::Vector3& rotation,
		IBehavior * behavior,
		DirectX::Model* model
	) override;
};

