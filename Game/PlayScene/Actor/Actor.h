/*
* 2023/04/09
* Actor.cpp
* �A�N�^�[
* �����@��
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
/// �A�N�^�[
/// </summary>
class Actor
{
protected:
	//�R�����X�e�[�g�̃|�C���^�[
	DirectX::CommonStates*          m_commonState;
	//���W
	DirectX::SimpleMath::Vector3    m_position;
	//�ړ���
	DirectX::SimpleMath::Vector3    m_velocity;
	//�p�x
	DirectX::SimpleMath::Vector3    m_rotation;
	//���f���̃|�C���^�[
	DirectX::Model*                 m_pModel;
	//�r�w�C�r�A�[�̃|�C���^�[
	IBehavior*                      m_behavia;
	//�A���O��
	float                           m_angle;
	//�A�N�e�B�u
	bool                            m_active;
	//AABB�����蔻��
	std::unique_ptr<AABBFor3D>      m_AABBObject;
	//�J�v�Z�������蔻��
	std::unique_ptr<Capsule>      m_capsule;
	//���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Actor();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Actor() = default;

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="velocity">�x���V�e�B</param>
	/// <param name="position">���W</param>
	/// <param name="active">�A�N�e�B�u</param>
	/// <param name="angle">�A���O��</param>
	/// <param name="behavia">�r�w�C�r�A�[�̃|�C���^�[</param>
	/// <param name="model">���f���̃|�C���^�[</param>
	/// <param name="commonState">�R�����X�e�[�g�̃|�C���^�[</param>
	virtual void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
		bool active,
		float angle,
		IBehavior* behavia,
		DirectX::Model* model, 
		DirectX::CommonStates* commonState) = 0;

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	virtual void Update(const DX::StepTimer& timer) = 0;

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="camera">�J�����̐��|�C���^�[</param>
	virtual void Draw(Camera* camera) = 0;

	/// <summary>
	/// �I������
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// �A�N�e�B�u��Ԃ���A�N�e�B�u��Ԃ�
	/// </summary>
	/// <returns>true = �A�N�e�B�u�Afalse = ��A�N�e�B�u</returns>
	bool IsActive() { return m_active; }

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }
	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	DirectX::SimpleMath::Vector3 GetPosition()const { return m_position; }

	/// <summary>
	/// �ړ��ʂ̐ݒ�
	/// </summary>
	/// <param name="velocity">�ړ���</param>
	void SetVelocity(DirectX::SimpleMath::Vector3 velocity) { m_velocity = velocity; }
	/// <summary>
	/// �ړ��ʂ̎擾
	/// </summary>
	/// <returns>���݂̈ړ���</returns>
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