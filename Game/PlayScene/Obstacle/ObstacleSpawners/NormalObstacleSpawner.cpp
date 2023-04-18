/*
* 2023/03/21
* BirdObstacleSpawner.cpp
* 麻生　楓
* 炎の障害物の生成する
*/
#include"pch.h"
#include"Game/PlayScene/Actor/Actor.h"
#include"../Obstacle.h"
#include"../../MyRandom.h"
#include"NormalObstacleSpawner.h"

/// <summary>
/// 炎の障害物の生成
/// </summary>
/// <param name="actors">障害物の配列</param>
/// <param name="position">生成する位置</param>
/// <param name="angle">角度（ラジアン）</param>
/// <param name="behavior">炎のビヘイビア</param>
/// <param name="model">炎のモデル（NULLでOK）</param>
/// <param name="commonState">コモンステート</param>
/// <returns>true=生成成功　false=生成失敗</returns>
bool NormalObstacleSpawner::Create(std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const float angle, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//使用中でなければ、敵を初期化する
		if (actor->IsActive())
			continue;


		Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
		if (obstale != nullptr)
		{
			obstale->SetType(Obstacle::ObstacleType::NORMAL);

		}
		DirectX::SimpleMath::Vector3 velocity;
		velocity.x = -sin(angle) * 0.1;
		velocity.z = cos(angle) * 0.1;
		DirectX::SimpleMath::Vector3 scale;

		//アイテムの初期化
		actor->Initialize(velocity, position, scale, true, angle, behavior, model, commonState);

		


		//アイテムの生成に成功
		return true;

	}
	//すべてアイテムが使用中
	return false;
}