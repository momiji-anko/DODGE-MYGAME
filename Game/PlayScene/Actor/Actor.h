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
#include"Game/PlayScene/ShadowMap.h"
#include"Game/Shader/FireEffectManager.h"

class StageManager;
class ObstacleManager;
class ItemManager;


/// <summary>
/// アクター
/// </summary>
class Actor
{
public:

	/// <summary>
	/// アクティブ状態の設定
	/// </summary>
	/// <param name="active">アクティブ</param>
	void SetActive(bool active) { m_active = active; }
	/// <summary>
	/// アクティブ状態か非アクティブ状態か
	/// </summary>
	/// <returns>true = アクティブ、false = 非アクティブ</returns>
	bool IsActive()const { return m_active; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	DirectX::SimpleMath::Vector3 GetPosition()const { return m_position; }

	/// <summary>
	/// 移動量の設定
	/// </summary>
	/// <param name="velocity">移動量</param>
	void SetVelocity(const DirectX::SimpleMath::Vector3& velocity) { m_velocity = velocity; }
	/// <summary>
	/// 移動量の取得
	/// </summary>
	/// <returns>現在の移動量</returns>
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }

	/// <summary>
	/// 角度の設定
	/// </summary>
	/// <param name="rotate">角度</param>
	void SetRotation(const DirectX::SimpleMath::Quaternion& rotate) { m_rotation = rotate; }
	/// <summary>
	/// 角度の設定
	/// </summary>
	/// <param name="rotation">角度</param>
	void SetRotation(const DirectX::SimpleMath::Vector3& rotation) { m_rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rotation); }
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
	/// ワールド行列の設定
	/// </summary>
	/// <param name="world">ワールド行列</param>
	void SetWorld(const DirectX::SimpleMath::Matrix& world) { m_world = world; }
	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	DirectX::SimpleMath::Matrix GetWorld() const { return m_world; }

	/// <summary>
	/// カプセルの設定
	/// </summary>
	/// <param name="capsule">カプセルのユニークポインタ</param>
	void SetCapsule(std::unique_ptr<Capsule>& capsule) { m_capsule = std::move(capsule); }
	/// <summary>
	/// 当たり判定用カプセルの取得
	/// </summary>
	/// <returns>カプセルの生ポインタ</returns>
	Capsule* GetCapsule()const { return m_capsule.get(); }

	/// <summary>
	/// スケール設定
	/// </summary>
	/// <param name="scale">スケール</param>
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; }

	/// <summary>
	/// スケール取得
	/// </summary>
	/// <returns>スケール</returns>
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }

	/// <summary>
	/// ビヘイビアー設定
	/// </summary>
	/// <param name="behavior">ビヘイビアー</param>
	void SetBehavior(IBehavior* behavior) { m_behavia = behavior; }
	/// <summary>
	/// ビヘイビアーの取得
	/// </summary>
	/// <returns>ビヘイビアー</returns>
	IBehavior* GetBehavior()const { return m_behavia; }

	/// <summary>
	/// モデル設定
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(DirectX::Model* model) { m_pModel = model; }

	/// <summary>
	/// モデル取得
	/// </summary>
	/// <returns>モデル</returns>
	DirectX::Model* GetModel()const { return m_pModel; }

	/// <summary>
	///　ステージマネージャーの設定
	/// </summary>
	/// <param name="stageManeger">ステージマネージャーの生ポインター</param>
	virtual void SetStageManeger(StageManager* stageManeger) { UNREFERENCED_PARAMETER(stageManeger); }

	/// <summary>
	/// タイプを整数型で取得
	/// </summary>
	/// <returns>タイプ</returns>
	virtual int GetTypeInt() const { return 0; }
	/// <summary>
	/// タイプを整数型で設定
	/// </summary>
	/// <param name="type">タイプ</param>
	virtual void SetTypeInt(int type) { UNREFERENCED_PARAMETER(type); }

	/// <summary>
	/// タイム設定
	/// </summary>
	/// <param name="time">タイム</param>
	virtual void SetTime(float time) { UNREFERENCED_PARAMETER(time); }
	/// <summary>
	/// タイム取得
	/// </summary>
	/// <returns>タイム</returns>
	virtual float GetTime() const { return 0.0f; }

	/// <summary>
	/// 回転速度の設定
	/// </summary>
	/// <param name="speed">回転速度</param>
	virtual void SetRotSpeed(float speed) { UNREFERENCED_PARAMETER(speed); }
	/// <summary>
	/// 回転速度取得
	/// </summary>
	/// <returns>回転速度</returns>
	virtual float GetRotSpeed() const { return 0.0f; }

	/// <summary>
	/// プレイヤーの座標取得
	/// </summary>
	/// <returns>プレイヤーの座標</returns>
	virtual DirectX::SimpleMath::Vector3 GetPlayerPosition() const { return DirectX::SimpleMath::Vector3::Zero; }
	/// <summary>
	/// プレイヤーの座標設定
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	virtual void SetPlayerPosition(const DirectX::SimpleMath::Vector3& playerPos) { UNREFERENCED_PARAMETER(playerPos); }

	/// <summary>
	/// エフェクト設定
	/// </summary>
	/// <param name="effect">エフェクトの生ポインタ</param>
	virtual void SetEffect(FireEffectManager* effect) { UNREFERENCED_PARAMETER(effect); }
	/// <summary>
	/// エフェクトの取得
	/// </summary>
	/// <returns>エフェクト</returns>
	virtual FireEffectManager* GetEffect()const { return nullptr; }



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
		const DirectX::SimpleMath::Vector3& rotataion,
		bool active,
		IBehavior* behavia,
		DirectX::Model* model
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
	/// リセット
	/// </summary>
	virtual void Reset() = 0;

	/// <summary>
	/// ワールド行列の計算
	/// </summary>
	/// <returns>ワールド行列</returns>
	DirectX::SimpleMath::Matrix CalculateWorld();


	/// <summary>
	/// 影生成
	/// </summary>
	/// <param name="shadow">シャドウマップの生ポインタ</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="projection">プロジェクション行列</param>
	virtual void CreateShadow(ShadowMap* shadow, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);

private:
	/// <summary>
	/// AABBの作成
	/// </summary>
	void CreateAABB();

	/// <summary>
	/// カプセルの生成
	/// </summary>
	void CreateCapsule();

private:
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
	//拡縮
	DirectX::SimpleMath::Vector3 m_scale;

};