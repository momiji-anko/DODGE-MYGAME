/*
* 2023/03/21
* NormalObstacleSpawner.h
* 麻生　楓
*/
#pragma once 
#include"../../ISpawner.h"

/// <summary>
/// 炎の障害物のスポナー
/// </summary>
class NormalObstacleSpawner :public ISpawner
{
public:
	//コンストラクタ
	NormalObstacleSpawner() = default;
	//デストラクタ
	~NormalObstacleSpawner()override = default;

	/// <summary>
	/// 炎の障害物の生成
	/// </summary>
	/// <param name="actors">障害物の配列</param>
	/// <param name="position">生成する位置</param>
	/// <param name="rotation">角度（ラジアン）</param>
	/// <param name="behavior">炎のビヘイビア</param>
	/// <param name="model">炎のモデル（NULLでOK）</param>
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
