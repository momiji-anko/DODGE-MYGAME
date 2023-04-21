/*
* 2023/04/09
* Actor.cpp
* �A�N�^�[
* �����@��
*/
#pragma once
#include"IActor.h"
#include"Game/PlayScene/ShadowMap.h"

/// <summary>
/// �A�N�^�[
/// </summary>
class Actor
{
private:
	//�R�����X�e�[�g�̃|�C���^�[
	DirectX::CommonStates*          m_commonState;
	//���W
	DirectX::SimpleMath::Vector3    m_position;
	//�ړ���
	DirectX::SimpleMath::Vector3    m_velocity;
	//�p�x
	DirectX::SimpleMath::Quaternion m_rotation;
	//���f���̃|�C���^�[
	DirectX::Model*                 m_pModel;
	//�r�w�C�r�A�[�̃|�C���^�[
	IBehavior*                      m_behavia;
	//�A���O��
	float                           m_angle;
	//�A�N�e�B�u
	bool                            m_active;
	//AABB�����蔻��
	std::unique_ptr<AABBFor3D> m_AABBObject;
	//�J�v�Z�������蔻��
	std::unique_ptr<Capsule>   m_capsule;
	//���[���h�s��
	DirectX::SimpleMath::Matrix  m_world;

	DirectX::SimpleMath::Vector3 m_scale;
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
	/// <param name="scale">�X�P�[��</param>
	/// <param name="active">�A�N�e�B�u</param>
	/// <param name="angle">�A���O��</param>
	/// <param name="behavia">�r�w�C�r�A�[�̃|�C���^�[</param>
	/// <param name="model">���f���̃|�C���^�[</param>
	/// <param name="commonState">�R�����X�e�[�g�̃|�C���^�[</param>
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
	/// �A�N�e�B�u��Ԃ̐ݒ�
	/// </summary>
	/// <param name="active">�A�N�e�B�u</param>
	void SetActive(bool active) { m_active = active; }
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

	/// <summary>
	/// �p�x�̐ݒ�
	/// </summary>
	/// <param name="rotate">�p�x</param>
	void SetRotation(DirectX::SimpleMath::Quaternion rotate) { m_rotation = rotate; }
	/// <summary>
	/// �p�x�̐ݒ�
	/// </summary>
	/// <param name="rotation">�p�x</param>
	void SetRotation(DirectX::SimpleMath::Vector3 rotation) { m_rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rotation); }
	/// <summary>
	/// �p�x�̎擾
	/// </summary>
	/// <returns>�p�x</returns>
	DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }

	/// <summary>
	/// �����蔻��pAABB�̎擾
	/// </summary>
	/// <returns>AABB�̐��|�C���^</returns>
	AABBFor3D* GetAABB() const { return m_AABBObject.get(); }




	/// <summary>
	/// ���[���h�s��̐ݒ�
	/// </summary>
	/// <param name="world">���[���h�s��</param>
	void SetWorld(DirectX::SimpleMath::Matrix& world) { m_world = world; }
	/// <summary>
	/// ���[���h�s��̎擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	DirectX::SimpleMath::Matrix GetWorld() const { return m_world; }

	/// <summary>
	/// ���[���h�s��̌v�Z
	/// </summary>
	/// <returns>���[���h�s��</returns>
	DirectX::SimpleMath::Matrix CalculationWorld();

	/// <summary>
	/// �J�v�Z���̐ݒ�
	/// </summary>
	/// <param name="capsule">�J�v�Z���̃��j�[�N�|�C���^</param>
	void SetCapsule(std::unique_ptr<Capsule>& capsule) { m_capsule = std::move(capsule); }
	/// <summary>
	/// �����蔻��p�J�v�Z���̎擾
	/// </summary>
	/// <returns>�J�v�Z���̐��|�C���^</returns>
	Capsule* GetCapsule() { return m_capsule.get(); }
	
	/// <summary>
	/// �X�P�[���ݒ�
	/// </summary>
	/// <param name="scale">�X�P�[��</param>
	void SetScale(DirectX::SimpleMath::Vector3 scale) { m_scale = scale; }

	/// <summary>
	/// �X�P�[���擾
	/// </summary>
	/// <returns>�X�P�[��</returns>
	DirectX::SimpleMath::Vector3 GetScale() { return m_scale; }

	/// <summary>
	/// �r�w�C�r�A�[�ݒ�
	/// </summary>
	/// <param name="behavior">�r�w�C�r�A�[</param>
	void SetBehavior(IBehavior* behavior) { m_behavia = behavior; }
	/// <summary>
	/// �r�w�C�r�A�[�̎擾
	/// </summary>
	/// <returns>�r�w�C�r�A�[</returns>
	IBehavior* GetBehavior() { return m_behavia; }

	/// <summary>
	/// ���f���ݒ�
	/// </summary>
	/// <param name="model">���f��</param>
	void SetModel(DirectX::Model* model) { m_pModel = model; }

	/// <summary>
	/// ���f���擾
	/// </summary>
	/// <returns>���f��</returns>
	DirectX::Model* GetModel() { return m_pModel; }

	/// <summary>
	/// �R�����X�e�[�g�̐ݒ�
	/// </summary>
	/// <param name="state">�R�����X�e�[�g</param>
	void SetCommonState(DirectX::CommonStates* state) { m_commonState = state; }
	/// <summary>
	/// �R�����X�e�[�g�̎擾
	/// </summary>
	/// <returns>�R�����X�e�[�g</returns>
	DirectX::CommonStates* GetCommonState() { return m_commonState; }

	/// <summary>
	/// �e����
	/// </summary>
	/// <param name="shadow">�V���h�E�}�b�v�̐��|�C���^</param>
	/// <param name="view">�r���[�s��</param>
	/// <param name="projection">�v���W�F�N�V�����s��</param>
	void CreateShadow(ShadowMap* shadow, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

private:
	
	/// <summary>
	/// AABB�̍쐬
	/// </summary>
	void CreateAABB();

	/// <summary>
	/// �J�v�Z���̐���
	/// </summary>
	void CreateCapsule();
};