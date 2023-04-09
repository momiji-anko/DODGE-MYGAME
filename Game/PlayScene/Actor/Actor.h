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
protected:
	//コモンステートのポインター
	DirectX::CommonStates*          m_commonState;
	//座標
	DirectX::SimpleMath::Vector3    m_position;
	//移動量
	DirectX::SimpleMath::Vector3    m_velocity;
	//角度
	DirectX::SimpleMath::Vector3    m_rotation;
	//モデルのポインター
	DirectX::Model*                 m_pModel;
	//ビヘイビアーのポインター
	IBehavior*                      m_behavia;
	//アングル
	float                           m_angle;
	//アクティブ
	bool                            m_active;
	//AABB当たり判定
	std::unique_ptr<AABBFor3D>      m_AABBObject;
	//カプセル当たり判定
	std::unique_ptr<Capsule>      m_capsule;
	//ワールド行列
	DirectX::SimpleMath::Matrix m_world;
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
	/// <param name="active">アクティブ</param>
	/// <param name="angle">アングル</param>
	/// <param name="behavia">ビヘイビアーのポインター</param>
	/// <param name="model">モデルのポインター</param>
	/// <param name="commonState">コモンステートのポインター</param>
	virtual void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
		bool active,
		float angle,
		IBehavior* behavia,
		DirectX::Model* model, 
		DirectX::CommonStates* commonState) = 0;

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

	void SetRotation(DirectX::SimpleMath::Vector3 rotate) { m_rotation = rotate; }
	DirectX::SimpleMath::Vector3 GetRotation() const { return m_rotation; }

	AABBFor3D* GetAABB() const { return m_AABBObject.get(); }

	float GetAngle()const { return m_angle; }

	DirectX::SimpleMath::Matrix GetWorld() const{ return m_world; }
	void SetWorld(DirectX::SimpleMath::Matrix& world) { m_world = world; }

	Capsule* GetCapsule() { return m_capsule.get(); }
	void SetCapsule(std::unique_ptr<Capsule>& capsule) { m_capsule=std::move(capsule); }

	void SetActive(bool active) { m_active = active; }
};