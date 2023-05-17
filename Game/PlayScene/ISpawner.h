/*
* 2023/04/11
* ISpawner.h
* スポナーのインターフェースクラス
* 麻生　楓
*/
#pragma once
#include<vector>
#include<SimpleMath.h>
#include<Model.h>
#include<CommonStates.h>
class Actor;
class IBehavior;

/// <summary>
/// スポナーのインターフェースクラス
/// </summary>
class ISpawner
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ISpawner() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ISpawner() = default;
	/// <summary>
	/// オブジェクトの生成
	/// </summary>
	/// <param name="actors">アクター</param>
	/// <param name="position">座標</param>
	/// <param name="rotation">角度</param>
	/// <param name="behavior">ビヘイビアー</param>
	/// <param name="model">モデル</param>
	/// <param name="commonState">コモンステート</param>
	/// <returns>true=生成成功, false=生成失敗</returns>
	virtual bool Create( std::vector< std::unique_ptr<Actor>>& actors,
		 const DirectX::SimpleMath::Vector3& position,
		  const DirectX::SimpleMath::Vector3& rotation,
		  IBehavior* behavior,
		DirectX::Model* model
	) = 0;
};