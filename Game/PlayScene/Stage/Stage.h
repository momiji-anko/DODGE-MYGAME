#pragma once 
#include"../Actor/Actor.h"
#include"../ShadowMap.h"
#include"Game/PlayScene/IBehavior.h"


/// <summary>
/// �X�e�[�W
/// </summary>
class Stage :public Actor
{
public:
	/// <summary>
	/// �^�C���̐ݒ�
	/// </summary>
	/// <param name="time"></param>
	void SetTime(float time) { m_time = time; }
	/// <summary>
	/// �^�C���̎擾
	/// </summary>
	/// <returns>�^�C��</returns>
	float GetTime() { return m_time; }

	/// <summary>
	/// �s���I�����Ă��邩�ݒ�
	/// </summary>
	/// <param name="flag">true=�I�����Ă���Afalse=�I�����Ă��Ȃ�</param>
	void SetMoveEndFlag(bool flag) { m_endFlag = flag; }
	/// <summary>
	/// �s���I�����Ă��邩�擾
	/// </summary>
	/// <returns>true=�I�����Ă���Afalse=�I�����Ă��Ȃ�</returns>
	bool GetMoveEndFlag() { return m_endFlag; }

	/// <summary>
	/// ���[�e�[�V�����^�C���̐ݒ�
	/// </summary>
	/// <param name="time">���[�e�[�V�����^�C��</param>
	void SetRotationTime(float time) { m_rotationTime_s = time; }
	/// <summary>
	/// ���[�e�[�V�����^�C���̎擾
	/// </summary>
	/// <returns>���[�e�[�V�����^�C��</returns>
	float GetRotationTime() { return m_rotationTime_s; }

	/// <summary>
	/// �I�t�Z�b�g���W�ݒ�
	/// </summary>
	/// <param name="position">�I�t�Z�b�g���W</param>
	void SetOffsetPosition(const DirectX::SimpleMath::Vector3& position) { m_offsetPosition = position; }
	/// <summary>
	/// �I�t�Z�b�g���W�擾
	/// </summary>
	/// <returns>�I�t�Z�b�g���W</returns>
	DirectX::SimpleMath::Vector3 GetOffsetPosition() { return m_offsetPosition; }

	/// <summary>
	/// ���[�`���ݒ�
	/// </summary>
	/// <param name="routine">���[�`��</param>
	void SetRoutine(int routine) { m_routine = routine; }
	/// <summary>
	/// ���[�`���擾
	/// </summary>
	/// <returns>���[�`��</returns>
	int GetRoutine() { return m_routine; }

	/// <summary>
	/// ���̊p�x�ݒ�
	/// </summary>
	/// <param name="rotetion">���̊p�x</param>
	void SetNextRotation(const DirectX::SimpleMath::Vector3& rotetion) { m_nextRotation = rotetion; }
	/// <summary>
	/// ���̊p�x�擾
	/// </summary>
	/// <returns>���̊p�x</returns>
	DirectX::SimpleMath::Vector3 GetNextRotation()const { return m_nextRotation; }

	/// <summary>
	/// �O�̊p�x�ݒ�
	/// </summary>
	/// <param name="rotetion">�O�̊p�x</param>
	void SetPreviousRotation(const DirectX::SimpleMath::Vector3& rotetion) { m_previousRotation = rotetion; }
	/// <summary>
	/// �O�̊p�x�擾
	/// </summary>
	/// <returns>�O�̊p�x</returns>
	DirectX::SimpleMath::Vector3 GetPreviousRotation()const { return m_previousRotation; }

	/// <summary>
	/// ��]���Ă��邩
	/// </summary>
	/// <returns>true=��]���Ă���Afalse=��]���Ă��Ȃ�</returns>
	bool  IsRotation() const{ return m_isRotation; }
	/// <summary>
	/// ��]��Ԃ̐ݒ�
	/// </summary>
	/// <param name="flag">true=��]���Ă���Afalse=��]���Ă��Ȃ�</param>
	void SetIsRotation(bool flag) { m_isRotation = flag; }

	/// <summary>
	/// �x�[�X���_���W�ݒ�
	/// </summary>
	/// <param name="baseVertices">�x�[�X���_���W</param>
	void SetBaseVertices(const std::vector<DirectX::SimpleMath::Vector3>& baseVertices) { m_baseVertices = baseVertices; }

	/// <summary>
	/// ���݂̒��_���W�擾
	/// </summary>
	/// <returns>���݂̒��_���W</returns>
	std::vector<DirectX::SimpleMath::Vector3> GetNowVertices()const { return m_nowVertices; }

	/// <summary>
	/// �C���f�b�N�X�ݒ�
	/// </summary>
	/// <param name="indices">�C���f�b�N�X</param>
	void SetIndices(const std::vector<std::vector<int>>& indices) { m_indices = indices; }
	/// <summary>
	/// �C���f�b�N�X�擾
	/// </summary>
	/// <returns>�C���f�b�N�X</returns>
	std::vector<std::vector<int>> GetIndices()const { return m_indices; }

	/// <summary>
	/// �V���h�E�}�b�v�̐ݒ�
	/// </summary>
	/// <param name="shadowMap">�V���h�E�}�b�v�̐��|�C���^</param>
	void SetShadowMap(ShadowMap* shadowMap) { m_shadowMap = shadowMap; }


public:
	

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Stage();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Stage()override;

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="velocity">�x���V�e�B</param>
	/// <param name="position">���W</param>
	/// <param name="scale">�g�嗦</param>
	/// <param name="rotation">�p�x</param>
	/// <param name="active">�A�N�e�B�u</param>
	/// <param name="behavia">�r�w�C�r�A�[</param>
	/// <param name="model">���f���̐��|�C���^</param>
	void Initialize(
		 const DirectX::SimpleMath::Vector3& velocity,
		 const DirectX::SimpleMath::Vector3& position,
		 const DirectX::SimpleMath::Vector3& scale,
		 const DirectX::SimpleMath::Vector3& rotation,
		 bool active,
		 IBehavior* behavia,
		 DirectX::Model* model
	 )override;
	
	 /// <summary>
	 /// �X�V
	 /// </summary>
	 /// <param name="timer">�^�C�}�[</param>
	 void Update(const DX::StepTimer& timer)override;

	 /// <summary>
	 /// �`��
	 /// </summary>
	 /// <param name="camera">�J�����̐��|�C���^</param>
	 void Draw(Camera* camera)override;

	 /// <summary>
	 /// �I������
	 /// </summary>
	 void Finalize()override;

	 /// <summary>
	 /// ���[�v
	 /// </summary>
	 /// <param name="start">�J�n���l</param>
	 /// <param name="end">�I�����l</param>
	 /// <param name="t">���݂̈ʒu</param>
	 /// <returns>���݂̐��l</returns>
	 float Lerp(float start, float end, float t)
	 {
		 return start + (end - start) * t;
	 }


	 /// <summary>
	 /// ���Z�b�g
	 /// </summary>
	 void Reset()override;
	
	 /// <summary>
	 /// �����̒��_���W�̍X�V
	 /// </summary>
	 void UpdateVertices();

private:

	static const float ROTARION_TIME_S;

	//�ϐ�
	//���[�`��
	int m_routine;
	//�I�t�Z�b�g���W
	DirectX::SimpleMath::Vector3 m_offsetPosition;
	//�^�C��
	float m_time;
	//�V���h�E�}�b�v�̐��|�C���^
	ShadowMap* m_shadowMap;
	//�s�����I��������
	bool m_endFlag;
	//��]�^�C��
	float m_rotationTime_s;
	//�O�̊p�x
	DirectX::SimpleMath::Vector3 m_previousRotation;
	//���̊p�x
	DirectX::SimpleMath::Vector3 m_nextRotation;
	//��]���Ă邩
	bool m_isRotation;
	
	//���_���W
	std::vector<DirectX::SimpleMath::Vector3>   m_baseVertices;
	//���݂̒��_���W
	std::vector<DirectX::SimpleMath::Vector3>   m_nowVertices;
	//�C���f�b�N�X
	std::vector<std::vector<int>>               m_indices;

};