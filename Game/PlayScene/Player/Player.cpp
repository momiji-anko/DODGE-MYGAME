#include"pch.h"
#include"Player.h"
#include"DeviceResources.h"
#include<Keyboard.h>
#include<Mouse.h>
#include<Effects.h>
#include"Libraries/MyLibraries/ModelManager.h"
#include"Libraries/MyLibraries/TextureManager.h"

const float Player::MOVE_SPEED = 9.0f;
const float Player::GRAVITY_FORCE = -1.4f;
const float Player::JUMP_FORCE = 0.50f;
const float Player::INVINCIBLE_TIME_SECONDS = 8.0f;

//�R���X�g���N�^
Player::Player() 
	:
	Actor(),
	m_invincbleTime(0.0f),
	m_playerState(PlayerState::NORMAL),
	m_playerModelNum{0,1,0,2,3},
	m_flyVelocity{}, 
	m_shieldCount(0),
	m_isShield(true),
	m_blink(nullptr),
	m_playerModel{},
	m_pAdx2(nullptr),
	m_jumpMusicID(0),
	m_stageManager(nullptr),
	m_itemManager(nullptr),
	m_invincibleCountCoolDownTime_s(0),
	m_modelTime_s(0),
	m_playerID(0),
	m_keys{}
{

}
//�f�X�g���N�^	
Player::~Player() 
{
	
}

/// <summary>
/// ������
/// </summary>
/// <param name="velocity">�ړ���</param>
/// <param name="position">�������W</param>
/// <param name="active">���݂��Ă��邩</param>
/// <param name="angle">�p�x</param>
/// <param name="behavia">�r�w�C�r�A�[�iPlayr�ł͎g��Ȃ��̂�NULL��OK�j</param>
/// <param name="model">�v���C���[�̃��f������NULL��OK</param>
/// <param name="commonState">�R�����X�e�[�g</param>
void Player::Initialize(const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, bool active, IBehavior* behavia, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	//�f�o�C�X���\�[�X�̎擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//�f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	//�f�o�C�X�̎擾
	ID3D11Device1* device = pDR->GetD3DDevice();

	//�p�����[�^�̐ݒ�
	//�ړ����x
	SetVelocity(velocity);

	//���W
	SetPosition(position);

	//�X�P�[��
	SetScale(scale);

	//�A�N�e�B�u

	SetActive(active);


	//�r�w�C�r�A�[
	SetBehavior(behavia);
	//���f��
	SetModel(model);

	//�R�����X�e�[�g
	SetCommonState(commonState);

	//���f���̐���
	CreatePlayerModel();

	
	//�����蔻��̗̈�̐ݒ�
	GetAABB()->SetData(DirectX::SimpleMath::Vector3(position.x - 0.5f, position.y - 0.9f, position.z - 0.5f), DirectX::SimpleMath::Vector3(position.x + 0.5f, position.y + 0.5f, position.z + 0.5f));


	//ADX2�̃C���X�^���X�擾
	m_pAdx2 = &ADX2::GetInstance();

	//�����蔻��̗̈�̐ݒ�
	GetCapsule()->a = DirectX::SimpleMath::Vector3(position.x, position.y + 0.5f, position.z);
	GetCapsule()->b = DirectX::SimpleMath::Vector3(position.x, position.y + 1.5f, position.z);

	//���̃e�N�X�`���ǂݍ���
	m_shieldTexture = TextureManager::GetInstance().LoadTexture(L"Resources/Textures/haet.png");

	//�u�����N����
	m_blink = std::make_unique<Blink>();
	m_blink->Initialize(0.15f, 12, 0.01f, true);

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void Player::Update(const DX::StepTimer& timer)
{

	Item::ItemType itemType = m_itemManager->PlayerHitItemType(GetAABB());

	//�A�N�e�B�u�łȂ���Ώ������I���
	if (!IsActive())
		return;

	if (itemType == Item::ItemType::SHIELD_ITEM)
	{
		ShieldCountUp();
	}
	
	
	

	PlayerMove(timer);


	if (m_flyVelocity.Length() != 0.0f)
	{
		SetPosition(GetPosition() + m_flyVelocity);

		m_flyVelocity *= DirectX::SimpleMath::Vector3(0.91f, 0.91f, 0.91f);
	}


	if (m_playerState == PlayerState::INVINCIBLE)
	{
		m_invincbleTime += timer.GetElapsedSeconds();

		if (m_invincbleTime > INVINCIBLE_TIME_SECONDS)
		{
			m_playerState = PlayerState::NORMAL;
		}
	}
	else
	{
		m_invincbleTime = 0;
	}


	DirectX::SimpleMath::Vector3 pos = GetPosition();

	GetAABB()->SetData(DirectX::SimpleMath::Vector3(pos.x - 0.5f, pos.y - 0, pos.z - 0.5f), DirectX::SimpleMath::Vector3(pos.x + 0.5f, pos.y + 2.0f, pos.z + 0.5f));

	GetCapsule()->a = DirectX::SimpleMath::Vector3(pos.x, pos.y + 0.5f, pos.z);
	GetCapsule()->b = DirectX::SimpleMath::Vector3(pos.x, pos.y + 1.5f, pos.z);



	if (GetPosition().y < -50.0f)
	{
		SetActive(false);

	}

	m_blink->Update(timer);

}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̃|�C���^�[</param>
void Player::Draw(Camera* camera)
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	
	if (!IsActive())
		return;

	
	CalculationWorld();

	if (m_blink->IsBlink())
	{
		int modelTime = static_cast<int>(m_modelTime_s);

		m_playerModel[m_playerModelNum[modelTime]]->Draw(context, *GetCommonState(), GetWorld(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
	}

}

void Player::TextureDraw(DirectX::SpriteBatch* spriteBatch)
{
	for (int i = 0; i < m_shieldCount; i++)
	{
		spriteBatch->Draw(m_shieldTexture.Get(), DirectX::SimpleMath::Vector2(20.0f, 10.0f + (i * 64.0f)), nullptr);
	}

}

// �I������
void Player::Finalize()
{

}

void Player::PlayerShadow( ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	if (m_blink->IsBlink())
	{
		int modelTime = static_cast<int >(m_modelTime_s);

		m_playerModel[m_playerModelNum[modelTime]]->Draw(context, *GetCommonState(), GetWorld(), view, projection, false, [&]()
			{
				shadowMap->DrawShadowMap(context);
			}
		);
	}
}


void Player::ShieldCountUp()
{
	m_pAdx2->Play(CRI_CUESHEET_0_COIN04_);
	m_shieldCount++;
}

void Player::ShieldCountDown()
{
	if (m_invincibleCountCoolDownTime_s <= 0.0f)
	{
		m_shieldCount--;
		m_pAdx2->Play(CRI_CUESHEET_0_DAMAGE1);
		m_invincibleCountCoolDownTime_s = 3.0f;
		m_blink->Start();
		if (m_shieldCount <= -1)
		{
			SetActive(false);
		}
	}

}



void Player::CreatePlayerModel()
{
	m_playerModel.resize(m_modelFiles.size());

	for (int i = 0; i < m_modelFiles.size();i++)
	{
		m_playerModel[i] = ModelManager::GetInstance().LoadModel(m_modelFiles[i].c_str());
	}

}



void Player::PlayerMove(const DX::StepTimer& timer)
{

	const DirectX::Keyboard::Keys& right =    m_keys[0];
	const DirectX::Keyboard::Keys& left =     m_keys[1];
	const DirectX::Keyboard::Keys& forward =  m_keys[2];
	const DirectX::Keyboard::Keys& backward = m_keys[3];
	const DirectX::Keyboard::Keys& jump =     m_keys[4];

	float elapsedTime = timer.GetElapsedSeconds();

	// �L�[���͏����擾����
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	DirectX::SimpleMath::Vector3 velocity = GetVelocity();
	DirectX::SimpleMath::Vector3 position = GetPosition();
	DirectX::SimpleMath::Vector3 rotation = GetRotation().ToEuler();

	velocity.x = 0.0f;
	velocity.z = 0.0f;


	if (keyState.IsKeyDown(right))
	{
		velocity.x += MOVE_SPEED * elapsedTime;

		rotation.y = -90;

	}

	if (keyState.IsKeyDown(left))
	{
		velocity.x -= MOVE_SPEED * elapsedTime;

		rotation.y = 90;
	}

	if (keyState.IsKeyDown(backward))
	{
		velocity.z += MOVE_SPEED * elapsedTime;

		rotation.y = 180;
	}

	if (keyState.IsKeyDown(forward))
	{
		velocity.z -= MOVE_SPEED * elapsedTime;

		rotation.y = 0;
	}




	if ((keyState.IsKeyDown(right)) && (keyState.IsKeyDown(backward)))
		rotation.y = DirectX::XMConvertToRadians(-(90 + 45));

	if ((keyState.IsKeyDown(right)) && (keyState.IsKeyDown(forward)))
		rotation.y = DirectX::XMConvertToRadians(-(45));

	if ((keyState.IsKeyDown(left)) && (keyState.IsKeyDown(backward)))
		rotation.y = DirectX::XMConvertToRadians((90 + 45));

	if ((keyState.IsKeyDown(left)) && (keyState.IsKeyDown(forward)))
		rotation.y = DirectX::XMConvertToRadians((45));


	if (keyState.IsKeyDown(right) || keyState.IsKeyDown(left) || keyState.IsKeyDown(forward) || keyState.IsKeyDown(backward))
	{
		m_modelTime_s += elapsedTime * 10;

		if (m_modelTime_s >= 4.0f)
		{
			m_modelTime_s = 0;
		}
	}
	else
	{
		m_modelTime_s = 0;
	}


	if (m_stageManager->PlayerStageAABBHitCheck(this))
	{

		velocity.y = 0;


		if (keyState.IsKeyDown(jump))
		{

			velocity.y += JUMP_FORCE;

			m_pAdx2->Play(CRI_CUESHEET_0_JUMP08);

		}

	}
	else
	{
		m_modelTime_s = 4;
		velocity.y += GRAVITY_FORCE * timer.GetElapsedSeconds();

	}

	SetVelocity(velocity);
	SetPosition(GetPosition() + velocity);

	SetRotation(DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rotation));
}
