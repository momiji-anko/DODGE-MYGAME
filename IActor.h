/*
* 2023/04/09
* Actor.cpp
* アクター
* 麻生　楓
*/
#pragma once
#include<SimpleMath.h>
#include<Model.h>
#include"Game/PlayScene/IBehavior.h"
#include <CommonStates.h>
#include"Libraries/MyLibraries/Camera.h"
#include"Game/PlayScene/AABBFor3D/AABBFor3D.h"
#include"Game/PlayScene/Objects.h"


class StageManager;
class ObstacleManager;
class ItemManager;

class IActor
{
public:
	/// <summary>
	/// アクティブ状態の設定
	/// </summary>
	/// <param name="active">アクティブ</param>
	virtual void SetActive(bool active) = 0;
	/// <summary>
	/// アクティブ状態か非アクティブ状態か
	/// </summary>
	/// <returns>true = アクティブ、false = 非アクティブ</returns>
	virtual bool IsActive() = 0;

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	virtual void SetPosition(DirectX::SimpleMath::Vector3 position) = 0;
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	virtual DirectX::SimpleMath::Vector3 GetPosition()const = 0;
	/// <summary>
	/// 移動量の設定
	/// </summary>
	/// <param name="velocity">移動量</param>
	virtual void SetVelocity(DirectX::SimpleMath::Vector3 velocity) = 0;
	/// <summary>
	/// 移動量の取得
	/// </summary>
	/// <returns>現在の移動量</returns>
	virtual DirectX::SimpleMath::Vector3 GetVelocity() const = 0;

	/// <summary>
	/// 角度の設定
	/// </summary>
	/// <param name="rotate">角度</param>
	virtual void SetRotation(DirectX::SimpleMath::Quaternion rotate) = 0;
	/// <summary>
	/// 角度の設定
	/// </summary>
	/// <param name="rotation">角度</param>
	virtual void SetRotation(DirectX::SimpleMath::Vector3 rotation) = 0;
	/// <summary>
	/// 角度の取得
	/// </summary>
	/// <returns>角度</returns>
	virtual DirectX::SimpleMath::Quaternion GetRotation() const = 0;

	/// <summary>
	/// 当たり判定用AABBの取得
	/// </summary>
	/// <returns>AABBの生ポインタ</returns>
	virtual AABBFor3D* GetAABB() const = 0;

	/// <summary>
	/// アングル設定
	/// </summary>
	/// <param name="angle">アングル</param>
	virtual void SetAngle(float angle) = 0;

	/// <summary>
	/// アングルの取得
	/// </summary>
	/// <returns>アングル</returns>
	virtual float GetAngle()const = 0;

	/// <summary>
	/// ワールド行列の設定
	/// </summary>
	/// <param name="world">ワールド行列</param>
	virtual void SetWorld(DirectX::SimpleMath::Matrix& world) = 0;
	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	virtual DirectX::SimpleMath::Matrix GetWorld() const = 0;


	/// <summary>
	/// カプセルの設定
	/// </summary>
	/// <param name="capsule">カプセルのユニークポインタ</param>
	virtual void SetCapsule(std::unique_ptr<Capsule>& capsule) = 0;
	/// <summary>
	/// 当たり判定用カプセルの取得
	/// </summary>
	/// <returns>カプセルの生ポインタ</returns>
	virtual Capsule* GetCapsule() = 0;

	/// <summary>
	/// スケール設定
	/// </summary>
	/// <param name="scale">スケール</param>
	virtual void SetScale(DirectX::SimpleMath::Vector3 scale) = 0;
	/// <summary>
	/// スケール取得
	/// </summary>
	/// <returns>スケール</returns>
	virtual DirectX::SimpleMath::Vector3 GetScale() = 0;

	/// <summary>
	/// ビヘイビアー設定
	/// </summary>
	/// <param name="behavior">ビヘイビアー</param>
	virtual void SetBehavior(IBehavior* behavior) = 0;
	/// <summary>
	/// ビヘイビアーの取得
	/// </summary>
	/// <returns>ビヘイビアー</returns>
	virtual IBehavior* GetBehavior() = 0;

	/// <summary>
	/// モデル設定
	/// </summary>
	/// <param name="model">モデル</param>
	virtual void SetModel(DirectX::Model* model) = 0;

	/// <summary>
	/// モデル取得
	/// </summary>
	/// <returns>モデル</returns>
	virtual DirectX::Model* GetModel() = 0;

	/// <summary>
	/// コモンステートの設定
	/// </summary>
	/// <param name="state">コモンステート</param>
	virtual void SetCommonState(DirectX::CommonStates* state) = 0;
	/// <summary>
	/// コモンステートの取得
	/// </summary>
	/// <returns>コモンステート</returns>
	virtual DirectX::CommonStates* GetCommonState() = 0;





public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IActor() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="velocity">ベロシティ</param>
	/// <param name="position">座標</param>
	/// <param name="scale">スケール</param>
	/// <param name="rotation">角度</param>
	/// <param name="active">アクティブ</param>
	/// <param name="behavia">ビヘイビアーのポインター</param>
	/// <param name="model">モデルのポインター</param>
	/// <param name="commonState">コモンステートのポインター</param>
	virtual void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Vector3& rotataion,
		bool active,
		IBehavior* behavia,
		DirectX::Model* model,
		DirectX::CommonStates* commonState
	) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	virtual void Update(const DX::StepTimer& timer) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラの生ポインター</param>
	virtual void Draw(Camera* camera) = 0;

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// ワールド行列の計算
	/// </summary>
	/// <returns>ワールド行列</returns>
	virtual DirectX::SimpleMath::Matrix CalculationWorld() = 0;

	/// <summary>
	/// リセット
	/// </summary>
	virtual void Reset() = 0;


};

