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
void Stage::Initialize(const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, bool active, IBehavior* behavia, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	//パラメータの設定
	//移動速度
	SetVelocity(velocity);

	//座標
	SetPosition(position);

	//スケール
	SetScale(scale);

	//アクティブ
	SetActive(active);

	//ビヘイビアー
	SetBehavior(behavia);
	//モデル
	SetModel(model);

	//コモンステート
	SetCommonState(commonState);

	//角度設定
	SetRotation(rotation);

	m_time = 0.0f;
	m_routine = 0;

	GetAABB()->SetData(DirectX::SimpleMath::Vector3(position.x - 6.0f, position.y - 0.5f, position.z - 6.0f), DirectX::SimpleMath::Vector3(position.x + 6.0f, position.y + 0.5f, position.z + 6.0f));

	m_previousRotation = GetRotation().ToEuler();
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

	if (GetBehavior() != nullptr)
	{
		GetBehavior()->Execute(timer, this);
	}
	DirectX::SimpleMath::Vector3 pos = GetPosition();

	GetAABB()->SetData(DirectX::SimpleMath::Vector3(pos.x - 6.0f, pos.y - 0.5f, pos.z - 6.0f), DirectX::SimpleMath::Vector3(pos.x + 6.0f, pos.y + 0.5f, pos.z + 6.0f));
}

// 描画
void Stage::Draw(Camera* camera)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	


	CalculationWorld();
	DirectX::SimpleMath::Matrix world = GetWorld();
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(m_offsetPosition);
	if (m_shadowMap != nullptr)
	{
		//モデル＆影描画
		GetModel()->Draw(context, *GetCommonState(), world, camera->GetViewMatrix(), camera->GetProjectionMatrix(), false, [&]()
			{
				m_shadowMap->DrawShadow(context, false);
			}
		);
	}
	else
	{
		//モデル＆影描画
		GetModel()->Draw(context, *GetCommonState(), world, camera->GetViewMatrix(), camera->GetProjectionMatrix());
	}

}

// 終了処理
void Stage::Finalize()
{
}

bool Stage::PlayerCheckGruondHit(AABBFor3D* playerAABB)
{
	return GetAABB()->HitCheck(playerAABB);
}

void Stage::Reset()
{
	m_endFlag = false;
	m_routine = 0;
	m_time = 0;
	m_offsetPosition = DirectX::SimpleMath::Vector3::Zero;
}
