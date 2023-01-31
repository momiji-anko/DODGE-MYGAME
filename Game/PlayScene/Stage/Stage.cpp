#include"pch.h"
#include"Stage.h"
#include"../MyRandom.h"
#include"DeviceResources.h"
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
	m_endFlag(false)
{

}

//デストラクタ
Stage::~Stage()
{

}

// 初期化
void Stage::Initialize(const DirectX::SimpleMath::Vector3& velocity,const DirectX::SimpleMath::Vector3& position,bool active,float angle,IBehavior* behavia,DirectX::Model* model,DirectX::CommonStates* commonState)
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



}

// 更新
void Stage::Update(const DX::StepTimer& timer)
{
	float time = timer.GetElapsedSeconds();

	static const float MOVE_TIME = 5.0f;
	m_time += time;

	switch (m_type)
	{
	case Stage::StageType::Stage1_1:

		Stage1_1();

		break;
	case Stage::StageType::Stage1_2:
		Stage1_2();

		break;
	case Stage::StageType::Stage1_3:
	
		Stage1_3();

		break;
	case Stage::StageType::Stage1_4:
		switch (m_routine)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
	
		}
		m_endFlag = true;

		break;
	case Stage::StageType::Stage2_1:
		break;
	default:
		break;
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
	DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::CreateRotationY(m_rotation.y / 180.0f * 3.14f);
	
	if (m_type == Stage::StageType::Stage2_1)
	{
		world *= DirectX::SimpleMath::Matrix::CreateScale(0.012f);
	}

	world *= rot * trans;
	//m_AABBObject->Draw(DirectX::SimpleMath::Matrix::Identity, camera->GetViewMatrix(), camera->GetProjectionMatrix(), DirectX::SimpleMath::Color(1, 1, 0, 1));


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
}

void Stage::Stage1_1()
{
	switch (m_routine)
	{
	case 0:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}

		break;
	case 1:
		m_time = 0;
		m_routine++;
	case 2:


		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}

		
		break;
	case 3:
		m_time = 0;
		m_routine++;
	case 4:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}
		m_offsetPosition.x = MyRandom::GetFloatRange(-0.2f, 0.2f);
		m_offsetPosition.y = MyRandom::GetFloatRange(-0.2f, 0.2f);
		m_offsetPosition.z = MyRandom::GetFloatRange(-0.2f, 0.2f);
		break;
	case 5:
		m_time = 0;
		m_routine++;
	case 6:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}

		m_position.y = Lerp(-1.0f, -100.0f, m_time / MOVE_TIME);
		m_offsetPosition.x = 0.0f;
		m_offsetPosition.y = 0.0f;
		m_offsetPosition.z = 0.0f;

		break;
	case 7:
		m_time = 0;
		m_routine++; ;
	case 8:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}
		m_position.y = -1.0f;


		if (m_position.x < 0)
		{
			m_position.x = -100.0f;
		}
		else
		{
			m_position.x = 100.0f;

		}
		break;
	case 9:
		m_time = 0;
		m_routine++; ;
	case 10:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}
		break;
	case 11:
		m_time = 0;
		m_routine++;
	case 12:
		if (m_time > MOVE_TIME )
		{
			m_time = MOVE_TIME;
			m_routine++;
		}

		break;
	case 13:

		m_time = 0;
		m_routine++;
	case 14:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}


	case 15:
		m_time = 0;
		m_routine++; ;
	case 16:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}


		break;
	case 17:
		m_time = 0;
		m_routine++; ;
	case 18:

		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}
		break;

	case 19:
		m_time = 0;
		m_routine++; ;
	case 20:

		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}



		break;
	case 21:
		m_time = 0;
		m_routine++; ;
	case 22:

		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}



		break;
	case 23:
		m_time = 0;
		m_routine++; ;
	case 24:

		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}



		break;
	case 25:
		m_time = 0;
		m_routine++; ;
	case 26:

		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}
		if (m_position.x < 0)
		{
			m_position.x = Lerp(-100.0f, -6.0f, m_time / MOVE_TIME);
		}
		else
		{
			m_position.x = Lerp(100.0f, 6.0f, m_time / MOVE_TIME);
		}
		m_endFlag = true;
		break;
	}

}

void Stage::Stage1_2()
{

	switch (m_routine)
	{
	case 0:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}

		break;
	case 1:
		m_time = 0;
		m_routine++;
	case 2:


		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}

		break;
	case 3:
		m_time = 0;
		m_routine++;
	case 4:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}
		break;
	case 5:
		m_time = 0;
		m_routine++;
	case 6:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}


		break;
	case 7:
		m_time = 0;
		m_routine++; ;
	case 8:

		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}

		
		break;
	case 9:
		m_time = 0;
		m_routine++; ;
	case 10:



		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}
		m_offsetPosition.x = MyRandom::GetFloatRange(-0.2f, 0.2f);
		m_offsetPosition.y = MyRandom::GetFloatRange(-0.2f, 0.2f);
		m_offsetPosition.z = MyRandom::GetFloatRange(-0.2f, 0.2f);

		break;
	case 11:
		m_time = 0;
		m_routine++;
	case 12:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}
		m_position.y = Lerp(-1.0f, -100.0f, m_time / MOVE_TIME);
		m_offsetPosition.x = 0.0f;
		m_offsetPosition.y = 0.0f;
		m_offsetPosition.z = 0.0f;
		break;
	case 13:

		m_time = 0;
		m_routine++;
	case 14:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}

		m_position.y = -1.0f;

		if (m_position.x < 0)
		{
			m_position.x = -100.0f;
		}
		else
		{
			m_position.x = 100.0f;

		}
	case 15:
		m_time = 0;
		m_routine++; ;
	case 16:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}


		break;
	case 17:
		m_time = 0;
		m_routine++; ;
	case 18:

		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}
		break;

	case 19:
		m_time = 0;
		m_routine++; ;
	case 20:

		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}



		break;
	case 21:
		m_time = 0;
		m_routine++; ;
	case 22:

		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}



		break;
	case 23:
		m_time = 0;
		m_routine++; ;
	case 24:

		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}



		break;
	case 25:
		m_time = 0;
		m_routine++; ;
	case 26:

		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}

		if (m_position.x < 0)
		{
			m_position.x = Lerp(-100.0f, -6.0f, m_time / MOVE_TIME);
		}
		else
		{
			m_position.x = Lerp(100.0f, 6.0f, m_time / MOVE_TIME);
		}
		m_endFlag = true;
		break;

	}

}


void Stage::Stage1_3()
{
	switch (m_routine)
	{
	case 0:
		if (m_time > MOVE_TIME )
		{
			m_time = MOVE_TIME;
			m_routine++;
		}

		break;
	case 1:
		m_time = 0;
		m_routine++;
	case 2:


		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}

		break;
	case 3:
		m_time = 0;
		m_routine++;
	case 4:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}

		break;
	case 5:
		m_time = 0;
		m_routine++;
	case 6:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}
		break;
	case 7:
		m_time = 0;
		m_routine++; ;
	case 8:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}
		break;
	case 9:
		m_time = 0;
		m_routine++; ;
	case 10:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}
		break;

	case 11:
		m_time = 0;
		m_routine++;
	case 12:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}

		break;
	case 13:

		m_time = 0;
		m_routine++;
	case 14:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}


		break;
	case 15:
		m_time = 0;
		m_routine++; ;
	case 16:
		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}
		

		break;
	case 17:
		m_time = 0;
		m_routine++; ;

	case 18:


		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}
		
		m_offsetPosition.x = MyRandom::GetFloatRange(-0.2f, 0.2f);
		m_offsetPosition.y = MyRandom::GetFloatRange(-0.2f, 0.2f);
		m_offsetPosition.z = MyRandom::GetFloatRange(-0.2f, 0.2f);
		break;
	case 19:
		m_time = 0;
		m_routine++; ;
	case 20:

		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}

		m_position.y = Lerp(-1.0f, -100.0f, m_time / MOVE_TIME);
		m_offsetPosition.x = 0.0f;
		m_offsetPosition.y = 0.0f;
		m_offsetPosition.z = 0.0f;

		break;
	case 21:
		m_time = 0;
		m_routine++; ;
	case 22:

		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}

		m_position.y = -1.0f;
		if (m_position.x < 0)
		{
			m_position.x = -100.0f;
		}
		else
		{
			m_position.x = 100.0f;

		}
		break;
	case 23:
		m_time = 0;
		m_routine++; ;
	case 24:

		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}

		if (m_position.x < 0)
		{
			m_position.x = Lerp(-100.0f, -6.0f, m_time / MOVE_TIME);
		}
		else
		{
			m_position.x = Lerp(100.0f, 6.0f, m_time / MOVE_TIME);
		}

		break;
	case 25:
		m_time = 0;
		m_routine++; ;
	case 26:

		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
		}

		m_endFlag = true;

		break;
	case 27:
		break;
	}

}

void Stage::Stage1_4()
{
}
