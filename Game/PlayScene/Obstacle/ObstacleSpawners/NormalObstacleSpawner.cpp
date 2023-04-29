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
/// <param name="rotation">角度（ラジアン）</param>
/// <param name="behavior">炎のビヘイビア</param>
/// <param name="model">炎のモデル（NULLでOK）</param>
/// <returns>true=生成成功　false=生成失敗</returns>
bool NormalObstacleSpawner::Create(std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& rotation, IBehavior* behavior, DirectX::Model* model)
{

	static const float SPEED = 4.0f;

	for (std::unique_ptr<Actor>& actor : actors)
	{
		//使用中でなければ、敵を初期化する
		if (actor->IsActive())
			continue;

		//移動ベロシティ
		DirectX::SimpleMath::Vector3 velocity;

		velocity = actor->GetPlayerPosition() - position;

		velocity.Normalize();
		
		//障害物にのタイプ設定
		actor->SetTypeInt(static_cast<int>(Obstacle::ObstacleType::NORMAL));

		velocity *= SPEED;
		

		//スケール
		DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3::One;

		//アイテムの初期化
		actor->Initialize(velocity, position, scale,rotation, true, behavior, model);

		//アイテムの生成に成功
		return true;

	}
	//すべてアイテムが使用中
	return false;
}