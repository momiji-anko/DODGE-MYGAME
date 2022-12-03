#include"pch.h"
#include"Obstacle.h"
#include"DeviceResources.h"
const float  Obstacle::MOVE_SPEED = 4.0f;

//コンストラクタ
Obstacle::Obstacle()
	:
	Actor(),
	m_rotSpeed(0.3f),
	m_type(ObstacleType::NORMAL)
{
	
}
//デストラクタ
Obstacle::~Obstacle()
{

}

// 初期化
void Obstacle::Initialize(const DirectX::SimpleMath::Vector3& velocity,const DirectX::SimpleMath::Vector3& position,bool active,float angle,IBehavior* behavia,DirectX::Model* model,DirectX::CommonStates* commonState)
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	
	m_velocity = velocity;
	m_position = position;

	m_active = active;

	m_angle = angle;

	m_behavia = behavia;

	m_commonState = commonState;
	m_pModel = model;

	switch (m_type)
	{
	case Obstacle::ObstacleType::NORMAL:
	case Obstacle::ObstacleType::MEANDERING:

		m_velocity.x = -sin(m_angle) * MOVE_SPEED;
		m_velocity.z = -cos(m_angle) * MOVE_SPEED;

		break;
	case Obstacle::ObstacleType::STICK:

		m_rotation.y = m_angle;
		//障害物を回転させる（発射角度を計算する行列を作る）
		DirectX::SimpleMath::Matrix rotetion = DirectX::SimpleMath::Matrix::CreateRotationY(m_angle);
		//速度を計算する（発射角度を加味した移動速度を計算する）
		m_velocity = DirectX::SimpleMath::Vector3::TransformNormal(m_velocity, rotetion);
		break;

	}
	m_geo = DirectX::GeometricPrimitive::CreateGeoSphere(pDR->GetD3DDeviceContext(), 1.0f, 2, false);
	
	m_capsule = std::make_unique<Capsule>();
	m_capsule->r = 0.0f;

	m_AABBObject = std::make_unique<AABBFor3D>();
	m_AABBObject->Initialize();
	//m_AABBObject->SetData(DirectX::SimpleMath::Vector3(m_position.x + 0.5f, m_position.y + 0.5f, m_position.z + 0.5f), DirectX::SimpleMath::Vector3(m_position.x - 0.5f, m_position.y - 0.5f, m_position.z - 0.5f));
	m_AABBObject->SetData({ 100,100,100 }, {100,100,100});

}

// 更新
void Obstacle::Update(const DX::StepTimer& timer)
{
	m_world = DirectX::SimpleMath::Matrix::Identity;

	switch (m_type)
	{
	case Obstacle::ObstacleType::NORMAL:

		m_AABBObject->SetData(DirectX::SimpleMath::Vector3(m_position.x - 0.3f, m_position.y - 0.5f, m_position.z - 0.3f), DirectX::SimpleMath::Vector3(m_position.x + 0.3f, m_position.y + 0.5f, m_position.z + 0.3f));
		m_effect->Update(timer);
		
		break;
	case Obstacle::ObstacleType::STICK:
	
		if (m_angle ==0.0f|| m_angle == DirectX::XM_PI)
		{
			m_AABBObject->SetData(DirectX::SimpleMath::Vector3(m_position.x - 0.5f, m_position.y - 0.0f, m_position.z - 12.0f), DirectX::SimpleMath::Vector3(m_position.x + 0.5f, m_position.y + 1.5f, m_position.z + 12.0f));

		}	
		else if (m_angle == DirectX::XM_PI /2.0f|| m_angle == DirectX::XM_PI+(DirectX::XM_PI/2.0f))
		{
			m_AABBObject->SetData(DirectX::SimpleMath::Vector3(m_position.x - 12.0f, m_position.y - 0.0f, m_position.z - 0.4f), DirectX::SimpleMath::Vector3(m_position.x + 12.0f, m_position.y + 1.0f, m_position.z + 0.4f));

		}

		break;

	}

	m_behavia->Execute(timer, this);

	if (m_position.y <= -30.0f || m_position.y >= 30.0f)
	{
		Reset();
	}
	if (m_position.z <= -30.0f || m_position.z >= 30.0f)
	{
		Reset();
	}
}

// 描画
void Obstacle::Draw(Camera* camera)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_commonState->DepthRead(), 0);
	

	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::CreateRotationY(m_rotation.y);
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(0.005f);

	//if(m_type!= Obstacle::ObstacleType::ROTATESTICK)


	//m_AABBObject->Draw(DirectX::SimpleMath::Matrix::Identity, camera->GetViewMatrix(), camera->GetProjectionMatrix(), DirectX::SimpleMath::Color(1, 1, 0, 1));

	

	switch (m_type)
	{
	case Obstacle::ObstacleType::NORMAL:
	case Obstacle::ObstacleType::MEANDERING:
		m_world *= rot * trans;
		m_effect->SetRenderState(camera->GetEyePosition(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
		m_effect->SetOffsetPosition(m_position);
		m_effect->Render();
		break;
	case Obstacle::ObstacleType::STICK:
		m_world *= rot * trans;
		m_pModel->Draw(context, *m_commonState, m_world, camera->GetViewMatrix(), camera->GetProjectionMatrix());
		break;
	case Obstacle::ObstacleType::ROTATESTICK:
	case Obstacle::ObstacleType::REVERSE_ROTATESTICK:
		m_world *= scale * rot * trans;

		m_pModel->Draw(context, *m_commonState, m_world, camera->GetViewMatrix(), camera->GetProjectionMatrix());
		/*m_geo->Draw(DirectX::SimpleMath::Matrix::Identity, camera->GetViewMatrix(), camera->GetProjectionMatrix(), DirectX::Colors::White, nullptr, true);

		m_geoWorld = DirectX::SimpleMath::Matrix::Identity;
		m_geoWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(m_capsule->b));*/


		//m_geo->Draw(m_geoWorld, camera->GetViewMatrix(), camera->GetProjectionMatrix(), DirectX::Colors::White, nullptr, true);
		break;
	case Obstacle::ObstacleType::STRAIGHT_MOVE_BOTTOM_TOP:
	case Obstacle::ObstacleType::STRAIGHT_MOVE_LEFT_RIGHT:
	case Obstacle::ObstacleType::STRAIGHT_MOVE_RIGHT_LEFT:
	case Obstacle::ObstacleType::STRAIGHT_MOVE_TOP_BOTTOM:
		m_world *= rot * scale * trans;

		m_pModel->Draw(context, *m_commonState, m_world, camera->GetViewMatrix(), camera->GetProjectionMatrix());
		break;
	case Obstacle::ObstacleType::BIRD:
		scale = DirectX::SimpleMath::Matrix::CreateScale(0.010f);
		m_world *= scale * rot  * trans;

		m_pModel->Draw(context, *m_commonState, m_world, camera->GetViewMatrix(), camera->GetProjectionMatrix());
		break;
	}
	
}

// 終了処理
void Obstacle::Finalize()
{

}

void Obstacle::Reset()
{
	m_active = false;
}

void Obstacle::ObstacleShadow(ShadowMap* shadow, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{

	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_commonState->DepthRead(), 0);

	if (m_pModel != nullptr)
	{


		m_pModel->Draw(context, *m_commonState, m_world, view, projection, false, [&]()
			{
				shadow->DrawShadowMap(context);
			}
		);
	}

}
