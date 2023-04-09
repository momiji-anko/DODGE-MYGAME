#include"pch.h"
#include"Stage.h"
#include"../MyRandom.h"
#include"DeviceResources.h"

//HACK::TEST
#include<Keyboard.h>

const float  Stage::MOVE_SPEED = 4.0f;
const float  Stage::MOVE_TIME = 5.0f;

//コンストラクタ
Stage::Stage()
	:
	Actor(),
	m_offsetPosition(DirectX::SimpleMath::Vector3::Zero),
	m_type(StageType::NONE),
	m_time(0.0f),
	m_shadowMap(nullptr),
	m_routine(0),
	m_endFlag(false),
	m_rotationTime_s(0),
	m_behavior(nullptr)
{
}

//デストラクタ
Stage::~Stage()
{
}

// 初期化
void Stage::Initialize(const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& position, bool active, float angle, IBehavior* behavia, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	m_commonState = commonState;
	m_velocity = velocity;
	m_position = position;

	m_active = active;

	m_angle = angle;

	m_behavia = behavia;

	m_pModel = model;

	m_time = 0.0f;
	m_routine = 0;
	m_AABBObject = std::make_unique<AABBFor3D>();
	m_AABBObject->Initialize();
	m_AABBObject->SetData(DirectX::SimpleMath::Vector3(m_position.x - 6.0f, m_position.y - 0.5f, m_position.z - 6.0f), DirectX::SimpleMath::Vector3(m_position.x + 6.0f, m_position.y + 0.5f, m_position.z + 6.0f));

	m_previousRotation = m_rotation;
	m_isRotation = false;
	m_rotationTime_s = 10;
}

// 更新
void Stage::Update(const DX::StepTimer& timer)
{
	float time = timer.GetElapsedSeconds();

	//HACK::TEST キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	static const float ROT_SPEED = 5.0f;

	static float ROTATION_ANGLE_coefficient = (2.5f);

	static int rotdis = 0;

	static float rotang;

	if (m_behavia != nullptr)
	{
		m_behavia->Execute(timer, this);
	}
	

	m_AABBObject->SetData(DirectX::SimpleMath::Vector3(m_position.x - 6.0f, m_position.y - 0.5f, m_position.z - 6.0f), DirectX::SimpleMath::Vector3(m_position.x + 6.0f, m_position.y + 0.5f, m_position.z + 6.0f));
}

// 描画
void Stage::Draw(Camera* camera)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;

	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_position + m_offsetPosition);
	DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::CreateRotationX(m_rotation.x / 180.0f * 3.14f) * DirectX::SimpleMath::Matrix::CreateRotationZ(m_rotation.z / 180.0f * 3.14f) * DirectX::SimpleMath::Matrix::CreateRotationY(m_rotation.y / 180.0f * 3.14f);

	if (m_type == Stage::StageType::Stage2_1)
	{
		world *= DirectX::SimpleMath::Matrix::CreateScale(0.32f);
	}

	world *= rot * trans;

	if (m_shadowMap != nullptr)
	{
		//モデル＆影描画
		m_pModel->Draw(context, *m_commonState, world, camera->GetViewMatrix(), camera->GetProjectionMatrix(), false, [&]()
			{
				m_shadowMap->DrawShadow(context, false);
			}
		);
	}
	else
	{
		m_pModel->Draw(context, *m_commonState, world, camera->GetViewMatrix(), camera->GetProjectionMatrix(), false);
	}
}

// 終了処理
void Stage::Finalize()
{
}

bool Stage::PlayerCheckGruondHit(AABBFor3D* playerAABB)
{
	return m_AABBObject->HitCheck(playerAABB);
}

void Stage::Reset()
{
	m_endFlag = false;
	m_routine = 0;
	m_time = 0;
	m_offsetPosition = DirectX::SimpleMath::Vector3::Zero;
}
