/*
* 2023/04/20
* Obstacle.h
* ��Q���N���X
* �����@��
*/
#pragma once 
#include"../Actor/Actor.h"
#include"../../Shader/FireEffectManager.h"
#include"../ShadowMap.h"

/// <summary>
/// ��Q��
/// </summary>
class Obstacle :public Actor
{
public:
	/// <summary>
	/// �^�C�v�𐮐��^�Ŏ擾
	/// </summary>
	/// <returns>�A�C�e���^�C�v</returns>
	int GetTypeInt() const override { return static_cast<int>(m_type); }
	/// <summary>
	/// �^�C�v�𐮐��^�Őݒ�
	/// </summary>
	/// <param name="type">�^�C�v</param>
	void SetTypeInt(int type) override { m_type = static_cast<ObstacleType>(type); }


	/// <summary>
	/// �G�t�F�N�g�ݒ�
	/// </summary>
	/// <param name="effect">�G�t�F�N�g�̐��|�C���^</param>
	void SetEffect(FireEffectManager* effect) override { m_effect = effect; }
	/// <summary>
	/// �G�t�F�N�g�̎擾
	/// </summary>
	/// <returns>�G�t�F�N�g</returns>
	FireEffectManager* GetEffect()const override { return m_effect; }


	/// <summary>
	/// ��]���x�擾
	/// </summary>
	/// <returns>��]���x</returns>
	float GetRotSpeed()const override { return m_rotSpeed; }
	/// <summary>
	/// ��]���x�̐ݒ�
	/// </summary>
	/// <param name="speed">��]���x</param>
	void SetRotSpeed(float speed) override { m_rotSpeed = speed; }

	/// <summary>
	/// �v���C���[�̍��W�擾
	/// </summary>
	/// <returns>�v���C���[�̍��W</returns>
	DirectX::SimpleMath::Vector3 GetPlayerPosition()const override { return m_playerPosition; }
	/// <summary>
	/// �v���C���[�̍��W�ݒ�
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	void SetPlayerPosition(const DirectX::SimpleMath::Vector3& playerPos)override { m_playerPosition = playerPos; }

	/// <summary>
	/// �T���s��
	/// </summary>
	/// <param name="targetPosition">�ڕW���W</param>
	/// <returns>�ڕW�ւ̃x�N�g��</returns>
	DirectX::SimpleMath::Vector3 Seek(const DirectX::SimpleMath::Vector3& targetPosition);

	/// <summary>
	/// Max�X�s�[�h�ݒ�
	/// </summary>
	/// <param name="speed">Max�X�s�[�h</param>
	void SetMaxSpeed(float speed) { m_maxSpeed = speed; }
	/// <summary>
	/// Max�X�s�[�h�擾
	/// </summary>
	/// <returns>Max�X�s�[�h</returns>
	float GetMaxSpeed() const { return m_maxSpeed; }

	/// <summary>
	/// Max�t�H�[�X�ݒ�
	/// </sumary>
	/// <param name="force"></param>
	void SetMaxForce(float force) { m_maxForce = force; }
	/// <summary>
	/// Max�t�H�[�X�擾
	/// </summary>
	/// <returns>Max�t�H�[�X</returns>
	float GetMaxForce() const { return m_maxForce; }

	/// <summary>
	/// �֍s�^�C���ݒ�
	/// </summary>
	/// <param name="time">�֍s�^�C��</param>
	void SetTime(float time) override { m_meandelingTime_s = time; }
	/// <summary>
	/// �֍s�^�C���擾
	/// </summary>
	/// <returns>�֍s�^�C��</returns>
	float GetTime()const override { return m_meandelingTime_s; }

public :
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Obstacle();
	 /// <summary>
	 /// �f�X�g���N�^
	 /// </summary>
	 ~Obstacle()override;

	 /// <summary>
	 /// ������
	 /// </summary>
	 /// <param name="velocity">�ړ��x���V�e�B</param>
	 /// <param name="position">���W</param>
	 /// <param name="scale">�X�P�[��</param>
	 /// <param name="rotation">�p�x</param>
	 /// <param name="active">�A�N�e�B�u</param>
	 /// <param name="behavia">�r�w�C�r�A�[�̐��|�C���^</param>
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
	/// ���Z�b�g
	/// </summary>
	void Reset()override;

private:

	/// <summary>
	/// ��Q�����G���A�O�ɂ��邩�m�F
	/// </summary>
	/// <returns>true = �G���A�O�Afalse = �G���A��</returns>
	bool  CheckInArea();



public:
	//��Q���^�C�v�̗񋓑�
	enum class ObstacleType
	{
		//���ʂ̉�
		NORMAL,
		//�_
		STICK,
		//覐�
		METEORITE, 
		//���v���ɉ�]����_
		ROTATESTICK,
		//�����v���ɉ�]����_
		REVERSE_ROTATESTICK,
		//�֍s���鉊
		MEANDERING,
		//��
		BIRD,
		
		NONE
		
	};
private:
				 
	//�ϐ�					   
	//��Q���̃^�C�v			   
	ObstacleType                 m_type;
	//���̃G�t�F�N�g			   
	FireEffectManager*           m_effect;
					 
	//��]�X�s�[�h							 
	float                        m_rotSpeed;
	// ����
	float                        m_mass;           
	// �ő�ړ����x(�X�s�[�h)
	float                        m_maxSpeed;       
	// �K�p�����ő�̗�
	float                        m_maxForce;       
	// ���̂ɉ�����
	DirectX::SimpleMath::Vector3 m_force;       
				 
	// �v���C���[�̍��W							 
	DirectX::SimpleMath::Vector3 m_playerPosition;
	//�֍s�p�^�C��
	float                        m_meandelingTime_s;

};