/*
* 2023/03/21
* MeanderingObstacleSpawner.cpp
* 麻生　楓
* 鳥の障害物の生成する
*/
#include"pch.h"
#include"Game/PlayScene/Actor/Actor.h"
#include"../Obstacle.h"
#include"../../MyRandom.h"
#include"MeanderingObstacleSpawner.h"

/// <summary>
/// 蛇行する炎の障害物の生成
/// </summary>
/// <param name="actors">障害物の配列</param>
/// <param name="position">生成する位置</param>
/// <param name="rotation">角度（ラジアン）</param>
/// <param name="behavior">蛇行する炎のビヘイビア</param>
/// <param name="model">蛇行する炎のモデル（NULLでOK）</param>
/// <returns>true=生成成功　false=生成失敗</returns>
bool MeanderingObstacleSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& rotation, IBehavior* behavior, DirectX::Model* model)
{
	static const float SPEED = 4.0f;

	for (std::unique_ptr<Actor>& actor : actors)
	{
		//使用中でなければ、敵を初期化する
		if (actor->IsActive())
			continue;


		//移動ベクトル
		DirectX::SimpleMath::Vector3 velocity;

		velocity = actor->GetPlayerPosition() - position;

		velocity.Normalize();

		velocity *= SPEED;

		//障害物にのタイプ設定
		actor->SetTypeInt(static_cast<int>(Obstacle::ObstacleType::MEANDERING));

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