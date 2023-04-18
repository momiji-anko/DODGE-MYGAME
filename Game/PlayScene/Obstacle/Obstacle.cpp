#include"pch.h"
#include"Obstacle.h"
#include"DeviceResources.h"
#include<random>

const float  Obstacle::MOVE_SPEED = 4.0f;

//コンストラクタ
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
//デストラクタ
Obstacle::~Obstacle()
{

}

// 初期化
void Obstacle::Initialize(const DirectX::SimpleMath::Vector3& velocity,const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale,bool active,float angle,IBehavior* behavia,DirectX::Model* model,DirectX::CommonStates* commonState)
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	SetMaxSpeed(1.0f);
	SetMaxForce(4.0f);
	m_wanderRadius = 1.0f;
	m_wanderDistance = 2.0f;
	m_wanderAngularVelocity = DirectX::XMConvertToRadians(30.0f);
	m_wanderAngle = angle;

	
	//パラメータの設定
	//移動速度
	SetVelocity(velocity);

	//座標
	SetPosition(position);

	//スケール
	SetScale(scale);

	//アクティブ
	SetActive(active);

	//アングル
	SetAngle(angle);

	//ビヘイビアー
	SetBehavior(behavia);
	//モデル
	SetModel(model);

	//コモンステート
	SetCommonState(commonState);

	m_wanderAngle = atan2(position.x-m_playerPosition.x, position.z - m_playerPosition.z);
	switch (m_type)
	{
	case Obstacle::ObstacleType::MEANDERING:
		m_velocity = DirectX::SimpleMath::Vector3::Zero;
		m_velocity = Seek(m_playerPosition);
		break;
	case Obstacle::ObstacleType::NORMAL:

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
	

	m_AABBObject = std::make_unique<AABBFor3D>();
	m_AABBObject->Initialize();
	m_AABBObject->SetData({ 100,100,100 }, {100,100,100});
	m_seekTime_s = 0.f;
}

// 更新
void Obstacle::Update(const DX::StepTimer& timer)
{
	m_seekTime_s += timer.GetElapsedSeconds();

	switch (m_type)
	{
	case Obstacle::ObstacleType::NORMAL:
	case Obstacle::ObstacleType::MEANDERING:

		m_effect->Update(timer);

		break;
	}

	GetBehavior()->Execute(timer, this);

	if (CheckInArea())
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





	switch (m_type)
	{
	case Obstacle::ObstacleType::NORMAL:
	case Obstacle::ObstacleType::MEANDERING:
		m_world = rot * trans;
		m_effect->SetRenderState(camera->GetEyePosition(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
		m_effect->SetOffsetPosition(m_position);
		m_effect->Render();
		break;
	case Obstacle::ObstacleType::STICK:
		m_world = rot * trans;
		m_pModel->Draw(context, *m_commonState, m_world, camera->GetViewMatrix(), camera->GetProjectionMatrix());
		break;
	case Obstacle::ObstacleType::ROTATESTICK:
	case Obstacle::ObstacleType::REVERSE_ROTATESTICK:
		m_world = scale * rot * trans;

		m_pModel->Draw(context, *m_commonState, m_world, camera->GetViewMatrix(), camera->GetProjectionMatrix());

		break;
	case Obstacle::ObstacleType::BIRD:
		scale = DirectX::SimpleMath::Matrix::CreateScale(0.010f);
		m_world = scale * rot  * trans;

		m_pModel->Draw(context, *m_commonState, m_world, camera->GetViewMatrix(), camera->GetProjectionMatrix());
		break;
	case Obstacle::ObstacleType::METEORITE:
		scale = DirectX::SimpleMath::Matrix::CreateScale(1);
		m_world = scale * rot  * trans;

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

	if ((m_position.y <= -Area || m_position.y >= Area)||(m_position.z <= -Area || m_position.z >= Area))
	{
		return true;
	}
	
	return false;
}
