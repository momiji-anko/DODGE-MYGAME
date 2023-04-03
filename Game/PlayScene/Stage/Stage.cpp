#include"pch.h"
#include"Stage.h"
#include"../MyRandom.h"
#include"DeviceResources.h"

//HACK::TEST
#include<Keyboard.h>

const float  Stage::MOVE_SPEED = 4.0f;
const float  Stage::MOVE_TIME = 5.0f;

//ƒRƒ“ƒXƒgƒ‰ƒNƒ^
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

//ƒfƒXƒgƒ‰ƒNƒ^
Stage::~Stage()
{
}

// ‰Šú‰»
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

	m_previousRotetion = m_rotation;
	m_isRotetion = false;
	m_rotationTime_s = 10;
}

// XV
void Stage::Update(const DX::StepTimer& timer)
{
	float time = timer.GetElapsedSeconds();

	//HACK::TEST ƒL[“ü—Íî•ñ‚ðŽæ“¾‚·‚é
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	static const float ROT_SPEED = 5.0f;

	static float ROTATION_ANGLE_coefficient = (5.0f);

	static int rotdis = 0;

	static float rotang;

	if (m_behavia != nullptr)
	{
		m_behavia->Execute(timer, this);
	}
	else
	{
		switch (m_type)
		{
		case Stage::StageType::Stage1_1:
			m_time += time;

			Stage1_1();

			break;
		case Stage::StageType::Stage1_2:
			m_time += time;

			Stage1_2();

			break;
		case Stage::StageType::Stage1_3:
			m_time += time;

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
			//HACK::TEST
			static const float COUNT_TIME_S;

			m_rotationTime_s -= time;

			if (m_rotationTime_s <= 0)
			{
				if (!m_isRotetion)
				{
					//‰ñ“]‚·‚é•ûŒü‚ðŒˆ‚ß‚éi‚O‚ª‰œ‚É‰ñ“]A‚P‚ª‰E‚É‰ñ“]A‚Q‚ªŽèŠÔ‚É‰ñ“]A‚R‚ª¶‚É‰ñ“]j
					rotdis = MyRandom::GetIntRange(0, 3);
					//‰ñ“]Šp“x
					rotang = ROTATION_ANGLE_coefficient * MyRandom::GetIntRange(1, 2);
				}

				m_time += time;

				m_isRotetion = true;

				switch (rotdis)
				{
				case 0:
					m_rotation.x = Lerp(m_previousRotetion.x, m_previousRotetion.x - rotang, m_time / MOVE_TIME);

					break;
				case 1:

					m_rotation.z = Lerp(m_previousRotetion.z, m_previousRotetion.z + rotang, m_time / MOVE_TIME);

					break;

				case 2:

					m_rotation.x = Lerp(m_previousRotetion.x, m_previousRotetion.x + rotang, m_time / MOVE_TIME);

					break;

				case 3:

					m_rotation.z = Lerp(m_previousRotetion.z, m_previousRotetion.z - rotang, m_time / MOVE_TIME);

					break;
				default:
					break;
				}

				if (m_time > MOVE_TIME)
				{
					m_rotationTime_s = 10.0f;
					m_previousRotetion = m_rotation;
					m_time = 0;
					m_isRotetion = false;
				}
			}

			break;
		case Stage::StageType::Stage3_1:
			if (keyState.J)
			{
				m_rotation.z += ROT_SPEED * time;
			}
			if (keyState.L)
			{
				m_rotation.z -= ROT_SPEED * time;
			}

			if (keyState.K)
			{
				m_rotation.x += ROT_SPEED * time;
			}
			if (keyState.I)
			{
				m_rotation.x -= ROT_SPEED * time;
			}
			break;
		}
	}

	m_AABBObject->SetData(DirectX::SimpleMath::Vector3(m_position.x - 6.0f, m_position.y - 0.5f, m_position.z - 6.0f), DirectX::SimpleMath::Vector3(m_position.x + 6.0f, m_position.y + 0.5f, m_position.z + 6.0f));
}

// •`‰æ
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
	//m_AABBObject->Draw(DirectX::SimpleMath::Matrix::Identity, camera->GetViewMatrix(), camera->GetProjectionMatrix(), DirectX::SimpleMath::Color(1, 1, 0, 1));

	if (m_shadowMap != nullptr)
	{
		//ƒ‚ƒfƒ‹•‰e•`‰æ
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

// I—¹ˆ—
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
			m_endFlag = true;
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
			m_endFlag = true;
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
	}
}

void Stage::Stage1_3()
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

		break;
	case 25:
		m_time = 0;
		m_routine++; ;
	case 26:

		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_routine++;
			m_endFlag = true;
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
	case 27:
		break;
	}
}

void Stage::Stage1_4()
{
}