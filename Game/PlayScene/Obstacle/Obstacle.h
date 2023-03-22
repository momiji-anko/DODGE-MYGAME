#pragma once 
#include"../Actor/Actor.h"
#include"../../Shader/FireEffectManager.h"
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
		
		

		BIRD,
		
	};
private:
	//�萔
	static const float MOVE_SPEED;

	//�ϐ�
	ObstacleType                                 m_type;
	FireEffectManager*                           m_effect;
	std::unique_ptr<DirectX::GeometricPrimitive> m_geo;

	DirectX::SimpleMath::Vector3                 m_scale;

	float                                        m_rotSpeed;
	float                                        m_meanderingSpeed;


	float                                        m_mass;           // ����
	float                                        m_maxSpeed;       // �ő�ړ����x(�X�s�[�h)
	float                                        m_maxForce;       // �K�p�����ő�̗�

	DirectX::SimpleMath::Vector3                 m_force;       // ���̂ɉ�����

		// �p�j�s���p�p�����[�^
	float                                        m_wanderRadius;
	float                                        m_wanderDistance;
	float                                        m_wanderAngularVelocity;
	float                                        m_wanderAngle;
	DirectX::SimpleMath::Vector3                 m_playerPosition;
	float                                        m_seekTime_s;
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

	void SetEffect(FireEffectManager* effect) { m_effect = effect; }

	ObstacleType GetObstacleType() { return m_type; }

	float& GetRotSpeed() { return m_rotSpeed; }
	void SetRotSpeed(float speed) { m_rotSpeed = speed; }
	float GetMeanderingSpeed() { return m_meanderingSpeed; }

	DirectX::SimpleMath::Vector3 GetScale(){ return m_scale; }
	void SetScale(DirectX::SimpleMath::Vector3 scale) { m_scale = scale; }





	DirectX::SimpleMath::Vector3 GetPlayerPosition() { return m_playerPosition; }
	void SetPlayerPosition(DirectX::SimpleMath::Vector3 playerPos) { m_playerPosition = playerPos; }


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

	float GetSeekTime() { return m_seekTime_s; }
};