/*
* 2023/05/25
* Stage.cpp
* �X�e�[�W
* �����@��
*/
#include"pch.h"
#include"Stage.h"
#include"../MyRandom.h"
#include"DeviceResources.h"
#include"Game/PlayScene/GameContext/GameContext.h"

const float Stage::ROTARION_TIME_S = 10.0f;


/// <summary>
/// �R���X�g���N�^
/// </summary>
Stage::Stage()
	:
	Actor(),
	m_routine(0),
	m_offsetPosition(DirectX::SimpleMath::Vector3::Zero),
	m_time(0.0f),
	m_shadowMap(nullptr),
	m_endFlag(false),
	m_rotationTime_s(0),
	m_isRotation(false)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Stage::~Stage()
{
}

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
void Stage::Initialize(const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, bool active, IBehavior* behavia, DirectX::Model* model)
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

	//�r�w�C�r�A�[
	SetBehavior(behavia);
	//���f��
	SetModel(model);

	//�p�x�ݒ�
	SetRotation(rotation);

	//���Z�b�g
	Reset();

	//�����̒��_���W�̔z��m��
	m_nowVertices.resize(m_baseVertices.size());

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void Stage::Update(const DX::StepTimer& timer)
{
	//�r�w�C�r�A�[��NULL�łȂ���Ύ��s
	if (GetBehavior() != nullptr)
	{
		GetBehavior()->Execute(timer, this);
	}

	//���_���W�̍X�V
	UpdateVertices();

}


/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̐��|�C���^</param>
void Stage::Draw(Camera* camera)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	
	//���[���h�s��v�Z
	DirectX::SimpleMath::Matrix world = CalculateWorld();

	//�h����v�Z
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(m_offsetPosition);
	//�V���h�E�}�b�v������΃��f���Ɖe��\��
	if (m_shadowMap != nullptr)
	{
		//���f�����e�`��
		GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), world, camera->GetViewMatrix(), camera->GetProjectionMatrix(), false, [&]()
			{
				m_shadowMap->DrawShadow(context, false);
			}
		);
	}
	else
	{
		//���f���`��
		GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), world, camera->GetViewMatrix(), camera->GetProjectionMatrix());
	}

}

/// <summary>
/// �I������
/// </summary>
void Stage::Finalize()
{
}

/// <summary>
/// ���Z�b�g
/// </summary>
void Stage::Reset()
{
	m_endFlag = false;
	m_routine = 0;
	m_time = 0;
	m_offsetPosition = DirectX::SimpleMath::Vector3::Zero;
	m_isRotation = false;
	m_rotationTime_s = ROTARION_TIME_S;
	m_previousRotation = GetRotation().ToEuler();
	m_nextRotation = DirectX::SimpleMath::Vector3::Zero;
}

/// <summary>
/// �����̒��_���W�̍X�V
/// </summary>
void Stage::UpdateVertices()
{
	//vertex�̐�����
	for (int j = 0; j < m_baseVertices.size(); j++)
	{
		//���݂̓����蔻��p�̒��_���W�s��
		DirectX::SimpleMath::Matrix nowVertexMatrix = DirectX::SimpleMath::Matrix::Identity;
		//Stage��X��Z�̊p�x�̌v�Z
		DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation());
		//���݂̒��_���W�̌v�Z
		nowVertexMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_baseVertices[j]) * rot * DirectX::SimpleMath::Matrix::CreateTranslation(GetPosition());
		//�v�Z�����s������݂̒��_���W�v�Z�ɑ������
		m_nowVertices[j] = DirectX::SimpleMath::Vector3(nowVertexMatrix._41, nowVertexMatrix._42, nowVertexMatrix._43);
	}

}
