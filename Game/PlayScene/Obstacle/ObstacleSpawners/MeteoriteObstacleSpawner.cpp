/*
* 2023/03/21
* MeteoriteObstacleSpawner.cpp
* 麻生　楓
* 隕石の障害物の生成する
*/
#include"pch.h"
#include"Game/PlayScene/Actor/Actor.h"
#include"../Obstacle.h"
#include"../../MyRandom.h"
#include"MeteoriteObstacleSpawner.h"

/// <summary>
/// 隕石の障害物の生成
/// </summary>
/// <param name="actors">障害物の配列</param>
/// <param name="position">生成する位置</param>
/// <param name="rotation">角度（ラジアン）</param>
/// <param name="behavior">隕石のビヘイビア</param>
/// <param name="model">隕石のモデル</param>
/// <returns>true=生成成功　false=生成失敗</returns>
bool MeteoriteObstacleSpawner::Create(std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& rotation, IBehavior* behavior, DirectX::Model* model)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//使用中でなければ、敵を初期化する
		if (actor->IsActive())
			continue;

		//障害物にのタイプ設定
		actor->SetTypeInt(static_cast<int>(Obstacle::ObstacleType::METEORITE));

		//移動ベクトル
		DirectX::SimpleMath::Vector3 velocity = { 0.0f, -1.0f, 0.0f };
		//スケール
		DirectX::SimpleMath::Vector3 scale;

		//障害物の初期化
		actor->Initialize(velocity, position, scale, rotation, true, behavior, model);


		//障害物の生成に成功
		return true;

	}
	//すべて障害物が使用中
	return false;
}
