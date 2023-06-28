/*
* 2023/04/09
* Actor.cpp 
* �A�N�^�[
* �����@��
*/
#include"pch.h"
#include"Game/PlayScene/Item/Item.h"
#include"DeviceResources.h"
#include"Actor.h"
#include"Game/PlayScene/GameContext/GameContext.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Actor::Actor():
	m_position(DirectX::SimpleMath::Vector3::Zero),
	m_velocity(DirectX::SimpleMath::Vector3::Zero),
	m_rotation(DirectX::SimpleMath::Vector3::Zero),
	m_active(false),
	m_angle(0.0f),
	m_behavia(nullptr),
	m_pModel(nullptr),
	m_AABBObject(nullptr)
{
	CreateAABB();
	CreateCapsule();

}

/// <summary>
/// ���[���h�s��̌v�Z
/// </summary>
/// <returns>���[���h�s��</returns>
DirectX::SimpleMath::Matrix Actor::CalculateWorld()
{
	//���[���h�s��ɒP�ʍs�������
	m_world = DirectX::SimpleMath::Matrix::Identity;
	//�ړ�
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	//��]
	DirectX::SimpleMath::Matrix rotation = DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_rotation);
	//�g�k
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
	//���[���h�s��̌v�Z
	m_world *= scale * rotation * trans;

	return m_world;
}

/// <summary>
/// �e����
/// </summary>
/// <param name="shadow">�V���h�E�}�b�v�̐��|�C���^</param>
/// <param name="view">�r���[�s��</param>
/// <param name="projection">�v���W�F�N�V�����s��</param>
void Actor::CreateShadow(ShadowMap* shadow, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	//�f�o�C�X���\�[�X�擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//�f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//���f��������Ήe�𐶐�����
	if (GetModel() != nullptr)
	{
		//���[���h�s����v�Z����
		CalculateWorld();

		//�e����
		GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorld(), view, projection, false, [&]()
			{
				shadow->DrawShadowMap(context);
			}
		);
	}

}


/// <summary>
/// AABB�̍쐬
/// </summary>
void Actor::CreateAABB()
{
	//AABB�����蔻��쐬
	m_AABBObject = std::make_unique<AABBFor3D>();
	//AABB������
	m_AABBObject->Initialize();

	DirectX::SimpleMath::Vector3 length = { 0.5f,0.5f,0.5f };

	//AABB�̓����蔻���ݒ�
	m_AABBObject->SetData(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);


}

/// <summary>
/// �J�v�Z���̐���
/// </summary>
void Actor::CreateCapsule()
{
	//�J�v�Z���̍쐬
	m_capsule = std::make_unique<Capsule>();
	//���a�ݒ�
	m_capsule->r = 1.0f;
}

