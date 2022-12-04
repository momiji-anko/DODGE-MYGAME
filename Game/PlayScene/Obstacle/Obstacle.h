#pragma once 
#include"../Actor/Actor.h"
#include"../../EffectManager3.h"
#include"../ShadowMap.h"
#include"../Objects.h"
#include <GeometricPrimitive.h>


class Obstacle :public Actor
{
public:
	//�񋓑�
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
	//�萔
	static const float MOVE_SPEED;

	//�ϐ�
	ObstacleType                                                       m_type;
	EffectManager3*                                                    m_effect;
	std::unique_ptr<DirectX::GeometricPrimitive>                       m_geo;

	DirectX::SimpleMath::Vector3                                       m_scale;

	float                                                              m_rotSpeed;
	float                                                              m_meanderingSpeed;


	float                                                              m_mass;           // ����
	float                                                              m_maxSpeed;       // �ő�ړ����x(�X�s�[�h)
	float                                                              m_maxForce;       // �K�p�����ő�̗�

	DirectX::SimpleMath::Vector3                                       m_force;       // ���̂ɉ�����

		// �p�j�s���p�p�����[�^
	float                                                              m_wanderRadius;
	float                                                              m_wanderDistance;
	float                                                              m_wanderAngularVelocity;
	float                                                              m_wanderAngle;
	DirectX::SimpleMath::Vector3                                       m_playerPosition;

public :
	//�R���X�g���N�^
	Obstacle();
	//�f�X�g���N�^
	virtual ~Obstacle()override;

	// ������
	virtual void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
		bool active,
		float angle,
		IBehavior* behavia,
		DirectX::Model* model,
		DirectX::CommonStates* commonState)override;

	// �X�V
	virtual void Update(const DX::StepTimer& timer)override;

	// �`��
	virtual void Draw(Camera* camera)override;

	// �I������
	virtual void Finalize()override;

	//���Z�b�g
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
		// ���̂ɉ����͂̒���
		m_force = Truncate(m_force, m_maxForce);


		// �����x�̎Z�o(�����x = �� / ����)
		DirectX::SimpleMath::Vector3 acceleration = m_force / m_mass;


		// ���x�̍X�V����ђ���
		m_velocity += acceleration * elapsedTime;
		m_velocity = Truncate(m_velocity, m_maxSpeed);


		// ���W�̍X�V
		DirectX::SimpleMath::Vector3 position = GetPosition();
		position += m_velocity * elapsedTime;
		SetPosition(position);


		// ���̂ɉ����͂̃��Z�b�g
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