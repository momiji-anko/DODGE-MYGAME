/*
* 2023/03/21
* MeteoriteObstacleSpawner.h
* 麻生　楓
*/
#pragma once 
#include"../../ISpawner.h"


/// <summary>
/// 隕石の障害物を生成するスポナー
/// </summary>
class MeteoriteObstacleSpawner :public ISpawner
{
public:
	
	// コンストラクタ
	MeteoriteObstacleSpawner() = default;
	//デストラクタ
	~MeteoriteObstacleSpawner()override = default;

	/// <summary>
	/// 隕石の障害物の生成
	/// </summary>
	/// <param name="actors">障害物の配列</param>
	/// <param name="position">生成する位置</param>
	/// <param name="rotation">角度（ラジアン）</param>
	/// <param name="behavior">隕石のビヘイビア</param>
	/// <param name="model">隕石のモデル</param>
	/// <param name="commonState">コモンステート</param>
	/// <returns>true=生成成功　false=生成失敗</returns>
	bool Create(std::vector< std::unique_ptr<Actor>>& actors,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& rotation,
		IBehavior* behavior,
		DirectX::Model* model,
		DirectX::CommonStates* commonState
	) override;
};