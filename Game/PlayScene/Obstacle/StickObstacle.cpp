#include"pch.h"
#include"StickObstacle.h"


//�R���X�g���N�^
StickObstacle::StickObstacle()
{

}

//�f�X�g���N�^
StickObstacle::~StickObstacle()
{

}

// ������
void StickObstacle::Initialize(const DirectX::SimpleMath::Vector3& velocity,const DirectX::SimpleMath::Vector3& position,bool active,float angle,IBehavior* behavia,DirectX::Model* model,DirectX::CommonStates* commonState)
{
	m_velocity = velocity;
	m_position = position;

	m_active = active;

	m_angle = angle;

	m_behavia = behavia;

	m_commonState = commonState;

	m_AABBObject = std::make_unique<AABBFor3D>();
	m_AABBObject->Initialize();
	//m_AABBObject->SetData

}

// �X�V
void StickObstacle::Update(const DX::StepTimer& timer)
{
	
	
}

// �`��
void StickObstacle::Draw(Camera* camera)
{

}

// �I������
void StickObstacle::Finalize()
{

}

void StickObstacle::Reset()
{
}
