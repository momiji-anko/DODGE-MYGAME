/*
* 2023/04/09
* Actor.cpp
* �A�N�^�[
* �����@��
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
	/// �A�N�e�B�u��Ԃ̐ݒ�
	/// </summary>
	/// <param name="active">�A�N�e�B�u</param>
	virtual void SetActive(bool active) = 0;
	/// <summary>
	/// �A�N�e�B�u��Ԃ���A�N�e�B�u��Ԃ�
	/// </summary>
	/// <returns>true = �A�N�e�B�u�Afalse = ��A�N�e�B�u</returns>
	virtual bool IsActive() = 0;

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	virtual void SetPosition(DirectX::SimpleMath::Vector3 position) = 0;
	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	virtual DirectX::SimpleMath::Vector3 GetPosition()const = 0;
	/// <summary>
	/// �ړ��ʂ̐ݒ�
	/// </summary>
	/// <param name="velocity">�ړ���</param>
	virtual void SetVelocity(DirectX::SimpleMath::Vector3 velocity) = 0;
	/// <summary>
	/// �ړ��ʂ̎擾
	/// </summary>
	/// <returns>���݂̈ړ���</returns>
	virtual DirectX::SimpleMath::Vector3 GetVelocity() const = 0;

	/// <summary>
	/// �p�x�̐ݒ�
	/// </summary>
	/// <param name="rotate">�p�x</param>
	virtual void SetRotation(DirectX::SimpleMath::Quaternion rotate) = 0;
	/// <summary>
	/// �p�x�̐ݒ�
	/// </summary>
	/// <param name="rotation">�p�x</param>
	virtual void SetRotation(DirectX::SimpleMath::Vector3 rotation) = 0;
	/// <summary>
	/// �p�x�̎擾
	/// </summary>
	/// <returns>�p�x</returns>
	virtual DirectX::SimpleMath::Quaternion GetRotation() const = 0;

	/// <summary>
	/// �����蔻��pAABB�̎擾
	/// </summary>
	/// <returns>AABB�̐��|�C���^</returns>
	virtual AABBFor3D* GetAABB() const = 0;

	/// <summary>
	/// �A���O���ݒ�
	/// </summary>
	/// <param name="angle">�A���O��</param>
	virtual void SetAngle(float angle) = 0;

	/// <summary>
	/// �A���O���̎擾
	/// </summary>
	/// <returns>�A���O��</returns>
	virtual float GetAngle()const = 0;

	/// <summary>
	/// ���[���h�s��̐ݒ�
	/// </summary>
	/// <param name="world">���[���h�s��</param>
	virtual void SetWorld(DirectX::SimpleMath::Matrix& world) = 0;
	/// <summary>
	/// ���[���h�s��̎擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	virtual DirectX::SimpleMath::Matrix GetWorld() const = 0;


	/// <summary>
	/// �J�v�Z���̐ݒ�
	/// </summary>
	/// <param name="capsule">�J�v�Z���̃��j�[�N�|�C���^</param>
	virtual void SetCapsule(std::unique_ptr<Capsule>& capsule) = 0;
	/// <summary>
	/// �����蔻��p�J�v�Z���̎擾
	/// </summary>
	/// <returns>�J�v�Z���̐��|�C���^</returns>
	virtual Capsule* GetCapsule() = 0;

	/// <summary>
	/// �X�P�[���ݒ�
	/// </summary>
	/// <param name="scale">�X�P�[��</param>
	virtual void SetScale(DirectX::SimpleMath::Vector3 scale) = 0;
	/// <summary>
	/// �X�P�[���擾
	/// </summary>
	/// <returns>�X�P�[��</returns>
	virtual DirectX::SimpleMath::Vector3 GetScale() = 0;

	/// <summary>
	/// �r�w�C�r�A�[�ݒ�
	/// </summary>
	/// <param name="behavior">�r�w�C�r�A�[</param>
	virtual void SetBehavior(IBehavior* behavior) = 0;
	/// <summary>
	/// �r�w�C�r�A�[�̎擾
	/// </summary>
	/// <returns>�r�w�C�r�A�[</returns>
	virtual IBehavior* GetBehavior() = 0;

	/// <summary>
	/// ���f���ݒ�
	/// </summary>
	/// <param name="model">���f��</param>
	virtual void SetModel(DirectX::Model* model) = 0;

	/// <summary>
	/// ���f���擾
	/// </summary>
	/// <returns>���f��</returns>
	virtual DirectX::Model* GetModel() = 0;

	/// <summary>
	/// �R�����X�e�[�g�̐ݒ�
	/// </summary>
	/// <param name="state">�R�����X�e�[�g</param>
	virtual void SetCommonState(DirectX::CommonStates* state) = 0;
	/// <summary>
	/// �R�����X�e�[�g�̎擾
	/// </summary>
	/// <returns>�R�����X�e�[�g</returns>
	virtual DirectX::CommonStates* GetCommonState() = 0;





public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~IActor() = default;

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="velocity">�x���V�e�B</param>
	/// <param name="position">���W</param>
	/// <param name="scale">�X�P�[��</param>
	/// <param name="rotation">�p�x</param>
	/// <param name="active">�A�N�e�B�u</param>
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
	/// ���[���h�s��̌v�Z
	/// </summary>
	/// <returns>���[���h�s��</returns>
	virtual DirectX::SimpleMath::Matrix CalculationWorld() = 0;

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	virtual void Reset() = 0;


};

