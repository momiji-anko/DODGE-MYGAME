#pragma once
#include<SimpleMath.h>
#include<Model.h>
#include"../IBehavior.h"
#include <CommonStates.h>
#include"Libraries/MyLibraries/Camera.h"
#include"../AABBFor3D/AABBFor3D.h"
#include"../Objects.h"
class Actor
{
protected:
	DirectX::CommonStates*          m_commonState;
	DirectX::SimpleMath::Vector3    m_position;
	DirectX::SimpleMath::Vector3    m_velocity;
	DirectX::SimpleMath::Vector3    m_rotation;

	DirectX::Model*                 m_pModel;
	
	IBehavior*                      m_behavia;

	float                           m_angle;
	bool                            m_active;
	
	std::unique_ptr<AABBFor3D>      m_AABBObject;
	std::unique_ptr<Capsule>      m_capsule;

	DirectX::SimpleMath::Matrix m_world;
public:
	Actor() :
		m_position(DirectX::SimpleMath::Vector3::Zero),
		m_velocity(DirectX::SimpleMath::Vector3::Zero),
		m_rotation(DirectX::SimpleMath::Vector3::Zero),
		m_active(false),
		m_angle(0.0f),
		m_behavia(nullptr),
		m_pModel(nullptr),
		m_commonState(nullptr),
		m_AABBObject(nullptr)
	{
	}

	virtual ~Actor() = default;

	// èâä˙âª
	virtual void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
		bool active,
		float angle,
		IBehavior* behavia,
		DirectX::Model* model, 
		DirectX::CommonStates* commonState) = 0;

	// çXêV
	virtual void Update(const DX::StepTimer& timer) = 0;

	// ï`âÊ
	virtual void Draw(Camera* camera) = 0;

	// èIóπèàóù
	virtual void Finalize() = 0;

	bool IsActive() { return m_active; }

	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }
	DirectX::SimpleMath::Vector3 GetPosition()const { return m_position; }

	void SetVelocity(DirectX::SimpleMath::Vector3 velocity) { m_velocity = velocity; }
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