/*
* 2023/03/21
* BirdObstacleSpawner.cpp
* 麻生　楓
* 鳥の障害物の生成する
*/
#include"pch.h"
#include"Game/PlayScene/Actor/Actor.h"
#include"../Obstacle.h"
#include"../../MyRandom.h"
#include"BirdObstacleSpawner.h"


/// <summary>
/// 鳥の障害物の生成
/// </summary>
/// <param name="actors">障害物の配列</param>
/// <param name="position">生成する位置</param>
/// <param name="rotation">角度（ラジアン）</param>
/// <param name="behavior">鳥のビヘイビア</param>
/// <param name="model">鳥のモデル</param>
/// <param name="commonState">コモンステート</param>
/// <returns>true=生成成功　false=生成失敗</returns>
bool BirdObstacleSpawner::Create(std::vector<std::unique_ptr<Actor>>& actors, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& rotation, IBehavior* behavior, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	//スポーンカウント
	int spawnCount = 0;
	//６体生成する
	for (int i = 0; i < 6; i++)
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
				obstale->SetType(Obstacle::ObstacleType::BIRD);

			}
			//移動ベクトル
			DirectX::SimpleMath::Vector3 velocity = DirectX::SimpleMath::Vector3(-10.0f, 0.0f, 0.0f);

			
			//障害物を回転させる（発射角度を計算する行列を作る）
			DirectX::SimpleMath::Matrix rotetion = DirectX::SimpleMath::Matrix::CreateRotationY(rotation.y);
			//速度を計算する（発射角度を加味した移動速度を計算する）
			velocity = DirectX::SimpleMath::Vector3::TransformNormal(velocity, rotetion);


			//1体づつのポジションの計算
			DirectX::SimpleMath::Vector3 pos;
			pos = position;
			//xが０であれば横一列になるようにする
			if (pos.x == 0.0f)
			{
				pos.x = (i - 2) * 4.0f;
			}
			//ｚが０であれば縦一列になるようにする
			if (pos.z == 0.0f)
			{
				pos.z = (i - 2) * 4.0f;
			}
			//スケール
			DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3(0.005f, 0.005f, 0.005f);


			//障害物の初期化
			actor->Initialize(velocity, pos, scale, rotation, true, behavior, model, commonState);

			DirectX::SimpleMath::Quaternion rotation;

			rotation = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, rotation.y + DirectX::XM_PI);
			
			//角度の設定
			actor->SetRotation(rotation);


			//障害物の生成に成功
			spawnCount++;
			break;

		}

	}
	//スポーンカウントが６以上であれば全て生成できた
	if (spawnCount >= 6)
	{
		return true;
	}

	//すべて障害物が使用中
	return false;
}
