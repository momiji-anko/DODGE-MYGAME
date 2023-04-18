/*
* 2023/04/09
* Actor.cpp
* アクター
* 麻生　楓
*/
#pragma once
#include<SimpleMath.h>
#include<Model.h>
#include"../IBehavior.h"
#include <CommonStates.h>
#include"Libraries/MyLibraries/Camera.h"
#include"../AABBFor3D/AABBFor3D.h"
#include"../Objects.h"

/// <summary>
/// アクター
/// </summary>
class Actor
{
private:
	//コモンステートのポインター
	DirectX::CommonStates*          m_commonState;
	//座標
	DirectX::SimpleMath::Vector3    m_position;
	//移動量
	DirectX::SimpleMath::Vector3    m_velocity;
	//角度
	DirectX::SimpleMath::Quaternion m_rotation;
	//モデルのポインター
	DirectX::Model*                 m_pModel;
	//ビヘイビアーのポインター
	IBehavior*                      m_behavia;
	//アングル
	float                           m_angle;
	//アクティブ
	bool                            m_active;
	//AABB当たり判定
	std::unique_ptr<AABBFor3D> m_AABBObject;
	//カプセル当たり判定
	std::unique_ptr<Capsule>   m_capsule;
	//ワールド行列
	DirectX::SimpleMath::Matrix  m_world;

	DirectX::SimpleMath::Vector3 m_scale;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Actor();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Actor() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="velocity">ベロシティ</param>
	/// <param name="position">座標</param>
	/// <param name="scale">スケール</param>
	/// <param name="active">アクティブ</param>
	/// <param name="angle">アングル</param>
	/// <param name="behavia">ビヘイビアーのポインター</param>
	/// <param name="model">モデルのポインター</param>
	/// <param name="commonState">コモンステートのポインター</param>
	virtual void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& scale,
		bool active,
		float angle,
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
	/// アクティブ状態の設定
	/// </summary>
	/// <param name="active">アクティブ</param>
	void SetActive(bool active) { m_active = active; }
	/// <summary>
	/// アクティブ状態か非アクティブ状態か
	/// </summary>
	/// <returns>true = アクティブ、false = 非アクティブ</returns>
	bool IsActive() { return m_active; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	DirectX::SimpleMath::Vector3 GetPosition()const { return m_position; }

	/// <summary>
	/// 移動量の設定
	/// </summary>
	/// <param name="velocity">移動量</param>
	void SetVelocity(DirectX::SimpleMath::Vector3 velocity) { m_velocity = velocity; }
	/// <summary>
	/// 移動量の取得
	/// </summary>
	/// <returns>現在の移動量</returns>
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }

	/// <summary>
	/// 角度の設定
	/// </summary>
	/// <param name="rotate">角度</param>
	void SetRotation(DirectX::SimpleMath::Quaternion rotate) { m_rotation = rotate; }
	/// <summary>
	/// 角度の取得
	/// </summary>
	/// <returns>角度</returns>
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }

	/// <summary>
	/// 当たり判定用AABBの取得
	/// </summary>
	/// <returns>AABBの生ポインタ</returns>
	AABBFor3D* GetAABB() const { return m_AABBObject.get(); }

	/// <summary>
	/// アングル設定
	/// </summary>
	/// <param name="angle">アングル</param>
	void SetAngle(float angle) { m_angle = angle; }

	/// <summary>
	/// アングルの取得
	/// </summary>
	/// <returns>アングル</returns>
	float GetAngle()const { return m_angle; }


	/// <summary>
	/// ワールド行列の設定
	/// </summary>
	/// <param name="world">ワールド行列</param>
	void SetWorld(DirectX::SimpleMath::Matrix& world) { m_world = world; }
	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	DirectX::SimpleMath::Matrix GetWorld() const { return m_world; }

	/// <summary>
	/// ワールド行列の計算
	/// </summary>
	/// <returns>ワールド行列</returns>
	DirectX::SimpleMath::Matrix CalculationWorld();

	/// <summary>
	/// カプセルの設定
	/// </summary>
	/// <param name="capsule">カプセルのユニークポインタ</param>
	void SetCapsule(std::unique_ptr<Capsule>& capsule) { m_capsule = std::move(capsule); }
	/// <summary>
	/// 当たり判定用カプセルの取得
	/// </summary>
	/// <returns>カプセルの生ポインタ</returns>
	Capsule* GetCapsule() { return m_capsule.get(); }
	
	/// <summary>
	/// スケール設定
	/// </summary>
	/// <param name="scale">スケール</param>
	void SetScale(DirectX::SimpleMath::Vector3 scale) { m_scale = scale; }

	/// <summary>
	/// スケール取得
	/// </summary>
	/// <returns>スケール</returns>
	DirectX::SimpleMath::Vector3 GetScale() { return m_scale; }

	/// <summary>
	/// ビヘイビアー設定
	/// </summary>
	/// <param name="behavior">ビヘイビアー</param>
	void SetBehavior(IBehavior* behavior) { m_behavia = behavior; }
	/// <summary>
	/// ビヘイビアーの取得
	/// </summary>
	/// <returns>ビヘイビアー</returns>
	IBehavior* GetBehavior() { return m_behavia; }

	/// <summary>
	/// モデル設定
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(DirectX::Model* model) { m_pModel = model; }

	/// <summary>
	/// モデル取得
	/// </summary>
	/// <returns>モデル</returns>
	DirectX::Model* GetModel() { return m_pModel; }

	/// <summary>
	/// コモンステートの設定
	/// </summary>
	/// <param name="state">コモンステート</param>
	void SetCommonState(DirectX::CommonStates* state) { m_commonState = state; }
	/// <summary>
	/// コモンステートの取得
	/// </summary>
	/// <returns>コモンステート</returns>
	DirectX::CommonStates* GetCommonState() { return m_commonState; }
private:
	void CreateAABB();
	void CreateCapsule();
};