/*
* 2023/03/21
* RotateStickObstacleSpawner.cpp
* 麻生　楓
* 回転する棒の障害物の生成する
*/
#include"pch.h"
#include"Game/PlayScene/Actor/Actor.h"
#include"../Obstacle.h"
#include"../../MyRandom.h"
#include"RotateStickObstacleSpawner.h"


/// <summary>
/// 回転する棒の障害物の生成
/// </summary>
/// <param name="actors">障害物の配列</param>
/// <param name="position">生成する位置</param>
/// <param name="rotation">角度（ラジアン）</param>
/// <param name="behavior">回転する棒のビヘイビア</param>
/// <param name="model">回転する棒のモデル</param>
/// <param name="commonState">コモンステート</param>
/// <returns>true=生成成功　false=生成失敗</returns>
bool RotateStickObstacleSpawner::Create(std::vector< std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& rotation, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	for (std::unique_ptr<Actor>& actor : actors)
	{
		//使用中でなければ、敵を初期化する
		if (actor->IsActive())
			continue;

		//障害物型にダイナミックキャストする
		Obstacle* obstale = dynamic_cast<Obstacle*>(actor.get());
		//キャスト出来たか
		if (obstale != nullptr)
		{
			//障害物にのタイプ設定
			obstale->SetType(Obstacle::ObstacleType::ROTATESTICK);
			//回転する速度の設定
			obstale->SetRotSpeed(0.01f);
		}
		//スケール
		DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3(0.005f, 0.005f, 0.005f);

		//障害物初期化
		actor->Initialize(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), position, scale, rotation, true, behavior, model, commonState);




		//障害物の生成に成功
		return true;

	}
	//すべて障害物が使用中
	return false;
}
