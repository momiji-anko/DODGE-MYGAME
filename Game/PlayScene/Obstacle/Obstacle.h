#pragma once 
#include"../Actor/Actor.h"
#include"../../EffectManager3.h"
#include"../ShadowMap.h"
#include"../Objects.h"
#include <GeometricPrimitive.h>


class Obstacle :public Actor
{
public:
	//列挙体
	enum class ObstacleType
	{
		NORMAL,
		STICK,
		METEORITE, 
		ROTATESTICK,
		REVERSE_ROTATESTICK,
		MEANDERING,
		
		STRAIGHT_MOVE_RIGHT_LEFT,
		STRAIGHT_MOVE_LEFT_RIGHT,
		
		STRAIGHT_MOVE_TOP_BOTTOM,
		STRAIGHT_MOVE_BOTTOM_TOP,

		BIRD,
		
	};
private:
	//定数
	static const float MOVE_SPEED;

	//変数
	ObstacleType                                                       m_type;
	EffectManager3*                                                    m_effect;
	std::unique_ptr<DirectX::GeometricPrimitive>                       m_geo;

	DirectX::SimpleMath::Vector3                                       m_scale;

	float                                                              m_rotSpeed;
	float                                                              m_meanderingSpeed;


	float                                                              m_mass;           // 質量
	float                                                              m_maxSpeed;       // 最大移動速度(スピード)
	float                                                              m_maxForce;       // 適用される最大の力

	DirectX::SimpleMath::Vector3                                       m_force;       // 物体に加わる力

		// 徘徊行動用パラメータ
	float                                                              m_wanderRadius;
	float                                                              m_wanderDistance;
	float                                                              m_wanderAngularVelocity;
	float                                                              m_wanderAngle;
	DirectX::SimpleMath::Vector3                                       m_playerPosition;

public :
	//コンストラクタ
	Obstacle();
	//デストラクタ
	virtual ~Obstacle()override;

	// 初期化
	virtual void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
		bool active,
		float angle,
		IBehavior* behavia,
		DirectX::Model* model,
		DirectX::CommonStates* commonState)override;

	// 更新
	virtual void Update(const DX::StepTimer& timer)override;

	// 描画
	virtual void Draw(Camera* camera)override;

	// 終了処理
	virtual void Finalize()override;

	//リセット
	virtual void Reset();

	void SetAABB(AABBFor3D* aabb) { m_AABBObject.reset(aabb); }

	void SetType(ObstacleType type) { m_type = type; }

	void ObstacleShadow(ShadowMap* shadow, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	void SetEffect(EffectManager3* effect) { m_effect = effect; }

	ObstacleType GetObstacleType() { return m_type; }

	float& GetRotSpeed() { return m_rotSpeed; }
	void SetRotSpeed(float speed) { m_rotSpeed = speed; }
	float GetMeanderingSpeed() { return m_meanderingSpeed; }

	DirectX::SimpleMath::Vector3 GetScale(){ return m_scale; }
	void SetScale(DirectX::SimpleMath::Vector3 scale) { m_scale = scale; }

	void ApplyForce(const DirectX::SimpleMath::Vector3& force)
	{
		m_force += force;
	}

	DirectX::SimpleMath::Vector3 Truncate(const DirectX::SimpleMath::Vector3& v, const float maxLength)
	{
		const float maxLengthSquared = maxLength * maxLength;
		const float vecLengthSquared = v.LengthSquared();
		if (vecLengthSquared <= maxLengthSquared)
			return v;
		else
			return v * (maxLength / v.Length());
	}

	DirectX::SimpleMath::Vector3 GetPlayerPosition() { return m_playerPosition; }
	void SetPlayerPosition(DirectX::SimpleMath::Vector3 playerPos) { m_playerPosition = playerPos; }
	void Locomote(float elapsedTime)
	{
		// 物体に加わる力の調整
		m_force = Truncate(m_force, m_maxForce);


		// 加速度の算出(加速度 = 力 / 質量)
		DirectX::SimpleMath::Vector3 acceleration = m_force / m_mass;


		// 速度の更新および調整
		m_velocity += acceleration * elapsedTime;
		m_velocity = Truncate(m_velocity, m_maxSpeed);


		// 座標の更新
		DirectX::SimpleMath::Vector3 position = GetPosition();
		position += m_velocity * elapsedTime;
		SetPosition(position);


		// 物体に加わる力のリセット
		m_force = DirectX::SimpleMath::Vector3::Zero;
	}


	DirectX::SimpleMath::Vector3 Wander();
	DirectX::SimpleMath::Vector3 Seek(const DirectX::SimpleMath::Vector3& targetPosition);

	float GetMaxSpeed() const { return m_maxSpeed; }
	float GetMaxForce() const { return m_maxForce; }
	void SetMaxSpeed(float speed) { m_maxSpeed = speed; }
	void SetMaxForce(float force) { m_maxForce = force; }

	DirectX::SimpleMath::Vector3 Forward() const
	{
		float x = std::cos(m_angle) * GetVelocity().Length();
		float z = std::sin(m_angle) * GetVelocity().Length();
		x = -sin(m_angle) * MOVE_SPEED;
		z = -cos(m_angle) * MOVE_SPEED;
		DirectX::SimpleMath::Vector3 vec{x,0.0,z};
		vec.Normalize();
		return vec;
	}
};