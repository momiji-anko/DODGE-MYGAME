/*
* 2023/04/20
* Obstacle.cpp
* ��Q���N���X
* �����@��
*/
#include"pch.h"
#include"Obstacle.h"
#include"DeviceResources.h"
#include"Game/PlayScene/GameContext/GameContext.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Obstacle::Obstacle()
	:
	Actor(),
	m_type(ObstacleType::NONE), 
	m_effect(nullptr),
	m_rotSpeed(0.3f),
	m_mass(0.0f),
	m_maxSpeed{ 1.0f },
	m_maxForce{ 4.0f },
	m_force{ DirectX::SimpleMath::Vector3::Zero },
	m_playerPosition{ DirectX::SimpleMath::Vector3::Zero },
	m_meandelingTime_s(0.0f)
{
	
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Obstacle::~Obstacle()
{
	Reset();
}

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
void Obstacle::Initialize(const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, bool active, IBehavior* behavia, DirectX::Model* model)
{

	//�p�����[�^�̐ݒ�
	//�ړ����x
	SetVelocity(velocity);

	//���W
	SetPosition(position);

	//�X�P�[��
	SetScale(scale);

	//�A�N�e�B�u
	SetActive(active);

	//�p�x
	SetRotation(rotation);

	//�r�w�C�r�A�[
	SetBehavior(behavia);
	//���f��
	SetModel(model);



}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void Obstacle::Update(const DX::StepTimer& timer)
{

	//�G�t�F�N�g��NULL�łȂ���΍X�V����
	if (m_effect != nullptr)
	{
		//�G�t�F�N�g�X�V
		m_effect->Update(timer);
	}
	
	//�r�w�C�r�A�[��NULL�łȂ���Ύ��s����
	if (GetBehavior() != nullptr)
	{
		//���s����
		GetBehavior()->Execute(timer, this);
	}

	//�G���A���ɂ��邩�m�F�G���A�O�ł���Ύ��S
	if (CheckInArea())
	{
		//���Z�b�g
		Reset();
	}
}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̐��|�C���^</param>
void Obstacle::Draw(Camera* camera)
{
	//�f�o�C�X���\�[�X�擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//�f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	
	//���[���h�s����v�Z����
	CalculateWorld();

	//���łȂ���΃��f���\������
	if (m_type != ObstacleType::NORMAL && m_type != ObstacleType::MEANDERING)
	{
		//���f���\������
		GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorld(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
	}
	//���ł���΃G�t�F�N�g�\������
	else if(m_type == ObstacleType::NORMAL || m_type == ObstacleType::MEANDERING)
	{
		//�G�t�F�N�g�\������
		m_effect->SetRenderState(camera->GetEyePosition(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
		m_effect->SetOffsetPosition(GetPosition());
		m_effect->Render();
	}
}

/// <summary>
/// �I������
/// </summary>
void Obstacle::Finalize()
{
	Reset();
}

/// <summary>
/// ���Z�b�g
/// </summary>
void Obstacle::Reset()
{
	SetActive(false);
}

/// <summary>
/// �T���s��
/// </summary>
/// <param name="targetPosition">�ڕW���W</param>
/// <returns>�ڕW�ւ̃x�N�g��</returns>
DirectX::SimpleMath::Vector3 Obstacle::Seek(const DirectX::SimpleMath::Vector3& targetPosition)
{
	//��������^�[�Q�b�g�ւ̈ړ���
	DirectX::SimpleMath::Vector3 desiredVelocity = targetPosition - GetPosition();
	//���K�������������ɂ���
	desiredVelocity.Normalize();
	//���x��������
	desiredVelocity *= GetMaxSpeed();
	//�ڕW�ւ̈ړ��ʌv�Z
	DirectX::SimpleMath::Vector3 steeringForce = desiredVelocity - GetVelocity();
	//�ڕW�ւ̈ړ��ʂ�Ԃ�
	return steeringForce;
}

/// <summary>
/// ��Q�����G���A�O�ɂ��邩�m�F
/// </summary>
/// <returns>true = �G���A�O�Afalse = �G���A��</returns>
bool Obstacle::CheckInArea()
{
	//�G���A
	static const float Area = 30.0f;
	//���W�ݒ�
	DirectX::SimpleMath::Vector3 position = GetPosition();

	//�G���A�O�ɂ��邩�m�F
	if ((position.x <= -Area || position.x >= Area)||(position.z <= -Area || position.z >= Area))
	{
		//�G���A�O
		return true;
	}
	//�G���A��
	return false;
}
