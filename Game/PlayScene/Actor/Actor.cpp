/*
* 2023/04/09
* Actor.cpp 
* �A�N�^�[
* �����@��
*/
#include"pch.h"
#include"DeviceResources.h"
#include"Actor.h"

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
	m_commonState(nullptr),
	m_AABBObject(nullptr)
{
	CreateAABB();
	CreateCapsule();

}


DirectX::SimpleMath::Matrix Actor::CalculationWorld()
{
	//���[���h�s��ɒP�ʍs�������
	m_world = DirectX::SimpleMath::Matrix::Identity;
	//�ړ�
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	//��]
	DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_rotation);
	//�g�k
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
	//���[���h�s��̌v�Z
	m_world *= scale * rot * trans;

	return m_world;
}

void Actor::CreateShadow(ShadowMap* shadow, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	//�f�o�C�X���\�[�X�擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//�f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//���f��������Ήe�𐶐�����
	if (GetModel() != nullptr)
	{
		//���[���h�s����v�Z����
		CalculationWorld();

		//�e����
		GetModel()->Draw(context, *GetCommonState(), GetWorld(), view, projection, false, [&]()
			{
				shadow->DrawShadowMap(context);
			}
		);
	}

}

void Actor::CreateAABB()
{
	//AABB�����蔻��쐬
	m_AABBObject = std::make_unique<AABBFor3D>();
	//AABB������
	m_AABBObject->Initialize();

	DirectX::SimpleMath::Vector3 length = { 0.5f,0.5f,0.5f };

	//AABB�̓����蔻���ݒ�
	m_AABBObject->SetData(m_position + length, m_position - length);


}

void Actor::CreateCapsule()
{
	//�J�v�Z���̍쐬
	m_capsule = std::make_unique<Capsule>();
	//���a�ݒ�
	m_capsule->r = 0.5f;
}

