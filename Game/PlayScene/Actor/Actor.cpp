/*
* 2023/04/09
* Actor.cpp 
* �A�N�^�[
* �����@��
*/
#include"pch.h"
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
	

}


DirectX::SimpleMath::Matrix Actor::CalculationWorld()
{
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

