#include"pch.h"
#include"NormalObstacle.h"
#include"DeviceResources.h"

//コンストラクタ
NormalObstacle::NormalObstacle()
{

}

//デストラクタ
NormalObstacle::~NormalObstacle()
{

}

// 初期化
void NormalObstacle::Initialize(const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& position, bool active, float angle, IBehavior* behavia, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	m_velocity = velocity;
	m_position = position;

	m_active = active;

	m_angle = angle;

	m_behavia = behavia;

	m_commonState = commonState;

	m_pModel = model;
}

// 更新
void NormalObstacle::Update(const DX::StepTimer& timer)
{
	m_behavia->Execute(timer, this);
}

// 描画
void NormalObstacle::Draw(Camera* camera)
{
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();


	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::CreateRotationY(m_rotation.y / 180.0f * 3.14f);


	world *= rot * trans;
	//m_AABBObject->Draw(world, camera->GetViewMatrix(), camera->GetProjectionMatrix(), DirectX::SimpleMath::Color(1, 0, 0, 1));

	m_pModel->Draw(context, *m_commonState, world, camera->GetViewMatrix(), camera->GetProjectionMatrix());
}

// 終了処理
void NormalObstacle::Finalize()
{

}

void NormalObstacle::Reset()
{
}
