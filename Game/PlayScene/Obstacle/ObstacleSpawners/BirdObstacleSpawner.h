/*
* 2023/03/21
* BirdObstacleSpawner.h
* 麻生　楓
*/
#pragma once 
#include"../../ISpawner.h"

/// <summary>
/// 鳥の障害物のスポナー
/// </summary>
class BirdObstacleSpawner :public ISpawner
{
public:
	//コンストラクタ
	BirdObstacleSpawner() = default;
	//デストラクタ
	~BirdObstacleSpawner()override = default;
	/// <summary>
	/// 鳥の障害物の生成
	/// </summary>
	/// <param name="actors">障害物の配列</param>
	/// <param name="position">生成する位置</param>
	/// <param name="angle">角度（ラジアン）</param>
	/// <param name="behavior">鳥のビヘイビア</param>
	/// <param name="model">鳥のモデル</param>
	/// <param name="commonState">コモンステート</param>
	/// <returns>true=生成成功　false=生成失敗</returns>
	bool Create(std::vector< std::unique_ptr<Actor>>& actors,
		const DirectX::SimpleMath::Vector3& position,
		const float angle,
		IBehavior* behavior,
		DirectX::Model* model,
		DirectX::CommonStates* commonState
	) override;
};