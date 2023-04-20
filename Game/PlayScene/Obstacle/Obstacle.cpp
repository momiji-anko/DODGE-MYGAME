#include"pch.h"
#include"Obstacle.h"
#include"DeviceResources.h"
#include<random>


//�R���X�g���N�^
Obstacle::Obstacle()
	:
	Actor(),
	m_rotSpeed(0.3f),
	m_type(ObstacleType::NORMAL), 
	m_force{ DirectX::SimpleMath::Vector3::Zero },
	m_maxSpeed{ 0.0f },
	m_maxForce{ 0.0f }
{
	
}
//�f�X�g���N�^
Obstacle::~Obstacle()
{

}

// ������
void Obstacle::Initialize(const DirectX::SimpleMath::Vector3& velocity,const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation,bool active,IBehavior* behavia,DirectX::Model* model,DirectX::CommonStates* commonState)
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	SetMaxSpeed(1.0f);
	SetMaxForce(4.0f);
	

	
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

	//�R�����X�e�[�g
	SetCommonState(commonState);

	DirectX::SimpleMath::Vector3 vel = GetVelocity();

	
	

	GetAABB()->SetData({100,100,100}, {100,100,100});
	m_seekTime_s = 0.f;


}

// �X�V
void Obstacle::Update(const DX::StepTimer& timer)
{
	m_seekTime_s += timer.GetElapsedSeconds();

	if (m_effect != nullptr)
	{
		m_effect->Update(timer);
	}
	
	
	if (GetBehavior() != nullptr)
	{
		GetBehavior()->Execute(timer, this);
	}


	if (CheckInArea())
	{
		Reset();
	}
}

// �`��
void Obstacle::Draw(Camera* camera)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	

	CalculationWorld();

	if (m_type != ObstacleType::NORMAL && m_type != ObstacleType::MEANDERING)
	{
		GetModel()->Draw(context, *GetCommonState(), GetWorld(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
	}
	else if(m_type == ObstacleType::NORMAL || m_type == ObstacleType::MEANDERING)
	{
		m_effect->SetRenderState(camera->GetEyePosition(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
		m_effect->SetOffsetPosition(GetPosition());
		m_effect->Render();
	}


	
}

// �I������
void Obstacle::Finalize()
{

}

void Obstacle::Reset()
{
	SetActive(false);
}

void Obstacle::ObstacleShadow(ShadowMap* shadow, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	if (GetModel() != nullptr)
	{

		GetModel()->Draw(context, *GetCommonState(), GetWorld(), view, projection, false, [&]()
			{
				shadow->DrawShadowMap(context);
			}
		);
	}

}



DirectX::SimpleMath::Vector3 Obstacle::Seek(const DirectX::SimpleMath::Vector3& targetPosition)
{
	DirectX::SimpleMath::Vector3 desiredVelocity = targetPosition - GetPosition();
	desiredVelocity.Normalize();
	desiredVelocity *= GetMaxSpeed();
	DirectX::SimpleMath::Vector3 steeringForce = desiredVelocity - GetVelocity();

	return steeringForce;
}

bool Obstacle::CheckInArea()
{
	static const float Area = 30.0f;
	DirectX::SimpleMath::Vector3 position = GetPosition();

	if ((position.y <= -Area || position.y >= Area)||(position.z <= -Area || position.z >= Area))
	{
		return true;
	}
	
	return false;
}
