/*
* 2023/04/23
* Player.cpp
* �v���C���[
* �����@��
*/
#include"pch.h"
#include"Player.h"
#include"DeviceResources.h"
#include<Keyboard.h>
#include<Mouse.h>
#include<Effects.h>
#include"Libraries/MyLibraries/ModelManager.h"
#include"Libraries/MyLibraries/TextureManager.h"

//	1�b�Ԃɐi�ރ}�X�̐�
const float Player::MOVE_SPEED = 9.0f;
//	1�b�Ԃɗ�����}�X�̐�
const float Player::GRAVITY_FORCE = -1.4f;
//	�W�����v��
const float Player::JUMP_FORCE = 0.50f;
//���G����
const float Player::INVINCIBLE_TIME_SECONDS = 3.0f;

//���AABB
const DirectX::SimpleMath::Vector3 Player::AABB_AREA_UP = DirectX::SimpleMath::Vector3(0.5f, 2.0f, 0.5f);
//����AABB
const DirectX::SimpleMath::Vector3 Player::AABB_AREA_DOWN = DirectX::SimpleMath::Vector3(-0.5f, -0.0f, -0.5f);

//��̃J�v�Z���̒���
const float Player::CAPSULE_AREA_UP = 1.5f;
//���̃J�v�Z���̒���
const float Player::CAPSULE_AREA_DOWN = 0.5f;

//�������ۂ̎��S���鍂��
const float Player::FALL_DEAD_AREA = -50.0f;
//���f���^�C���̍ő吔
const float Player::MAX_MODEL_TIME_S = 4.0f;
//���f���^�C���̑��x
const float Player::MODEL_TIME_SPEED = 10.0f;

//�v���C���[�̃V�[���h��ʕ\���ʒu
const DirectX::SimpleMath::Vector2 Player::PLAYERS_SHIELD_TEXTURE_POSITION[2] = {
	DirectX::SimpleMath::Vector2(20.0f,10.0f),
	DirectX::SimpleMath::Vector2(1260.0f,10.0f)
};

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player()
	:
	Actor(),
	m_invincbleTime(0.0f),
	m_playerModel{},
	m_playerModelNum{0,1,0,2,3},
	m_modelTime_s(0),
	m_pAdx2(nullptr),
	m_jumpMusicID(0),
	m_flyVelocity{}, 
	m_shieldCount(0),
	m_invincibleCountCoolDownTime_s(0),
	m_shieldTexture{},
	m_itemManager(nullptr),
	m_stageManager(nullptr),
	m_playerID(0),
	m_blink(nullptr),
	m_keys{},
	m_modelFiles{},
	m_obstacleManager{}
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
	
}

/// <summary>
/// ������
/// </summary>
/// <param name="velocity">�ړ���</param>
/// <param name="position">�������W</param>
/// <param name="rotation">�p�x</param>
/// <param name="active">���݂��Ă��邩</param>
/// <param name="behavia">�r�w�C�r�A�[�iPlayr�ł͎g��Ȃ��̂�NULL��OK�j</param>
/// <param name="model">�v���C���[�̃��f������NULL��OK</param>
/// <param name="commonState">�R�����X�e�[�g</param>
void Player::Initialize(const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, bool active, IBehavior* behavia, DirectX::Model* model, DirectX::CommonStates* commonState)
{


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

	//�p�x�ݒ�
	SetRotation(rotation);

	//���f���̐���
	CreatePlayerModel();

	
	//�����蔻��̗̈�X�V
	HitAreaUpdate();

	//ADX2�̃C���X�^���X�擾
	m_pAdx2 = &ADX2::GetInstance();


	//���̃e�N�X�`���ǂݍ���
	m_shieldTexture = TextureManager::GetInstance().LoadTexture(L"Resources/Textures/haet.png");
	
	//�_�ő��x
	float blinkTime_s = 0.25f;
	//�_�ŉ�
	int blinkCount = 12;
	//���񂾂񑁂�����_�ő��x
	float blinkFastTime_s = 0.01f;
	//�u�����N����
	m_blink = std::make_unique<Blink>();
	//������
	m_blink->Initialize(blinkTime_s, blinkCount, blinkFastTime_s, true);

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void Player::Update(const DX::StepTimer& timer)
{
	//�A�N�e�B�u�łȂ���Ώ��������Ȃ�
	if (!IsActive())
		return;

	//�A�C�e���Ɠ����蔻������
	Item::ItemType itemType = m_itemManager->PlayerHitItemType(GetAABB());

	//���������A�C�e�����V�[���h�ł������ꍇ�A�V�[���h�J�E���g�𑝂₷
	if (itemType == Item::ItemType::SHIELD_ITEM)
	{
		ShieldCountUp();
	}

	//�v���C���[�̈ړ�
	PlayerMove(timer);


	DirectX::SimpleMath::Vector3 flyVelocity = DirectX::SimpleMath::Vector3::Zero;

	//��]����_�Ɠ����蔻����,�������Ă����ꍇ�v���C���[�𐁂���΂�
	if (m_obstacleManager->PlayerCapsuleHitCheck(this, &flyVelocity))
	{
		m_flyVelocity = flyVelocity;
	}

	//��Q���Ɠ������Ă����ꍇ�V�[���h�����炷
	if (m_obstacleManager->PlayerHitCheck(GetAABB()))
	{
		ShieldCountDown();
	}

	//������΂���Ă����Ԃł���΂��񂾂񌸑�����
	if (m_flyVelocity.Length() != 0.0f)
	{
		SetPosition(GetPosition() + m_flyVelocity);
		//����
		m_flyVelocity *= DirectX::SimpleMath::Vector3(0.91f, 0.91f, 0.91f);
	}


	//�����蔻��̗̈�̍X�V
	HitAreaUpdate();

	//��������Y���W��-50�ɂȂ����玀�S����
	if (GetPosition().y < FALL_DEAD_AREA)
	{
		SetActive(false);
	}

	//�u�����N�̍X�V
	m_blink->Update(timer);

}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̃|�C���^�[</param>
void Player::Draw(Camera* camera)
{
	//�f�o�C�X���\�[�X�̎擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//�f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//�A�N�e�B�u�łȂ���Ώ��������Ȃ�
	if (!IsActive())
		return;

	//���[���h�s��v�Z
	CalculationWorld();

	//�u�����N���Ă��Ȃ���΃��f���\��
	if (m_blink->IsBlink())
	{
		//���݂̃��f���̏��
		int modelTime = static_cast<int>(m_modelTime_s);

		m_playerModel[m_playerModelNum[modelTime]]->Draw(context, *GetCommonState(), GetWorld(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
	}

}

/// <summary>
/// ��UI�̕`��
/// </summary>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void Player::TextureDraw(DirectX::SpriteBatch* spriteBatch)
{
	//��������Ώ��`��
	for (int i = 0; i < m_shieldCount; i++)
	{
		//�����m�̗���Ă��鋗��
		DirectX::SimpleMath::Vector2 shieldTexDistance = DirectX::SimpleMath::Vector2(0.0f, 64.0f * i);

		spriteBatch->Draw(m_shieldTexture.Get(), PLAYERS_SHIELD_TEXTURE_POSITION[m_playerID] + shieldTexDistance, nullptr);
	}

}

/// <summary>
/// �I������
/// </summary>
void Player::Finalize()
{
	Reset();
}

/// <summary>
/// �����Ă��鏂�̐����P���₷
/// </summary>
void Player::ShieldCountUp()
{
	//�V�[���h��������特��炷
	m_pAdx2->Play(CRI_CUESHEET_0_COIN04_);
	//�V�[���h�̐����P���₷
	m_shieldCount++;
}


/// <summary>
/// �����Ă��鏂�̐����P���炷�@������������Ă��Ȃ��ꍇ���S������
/// </summary>
void Player::ShieldCountDown()
{
	//���G���Ԃł͂Ȃ��ꍇ�V�[���h���P���炷
	if (m_invincibleCountCoolDownTime_s <= 0.0f)
	{
		//�V�[���h���P���炷
		m_shieldCount--;
		//�_���[�W�����o��
		m_pAdx2->Play(CRI_CUESHEET_0_DAMAGE1);
		
		//���G����
		m_invincibleCountCoolDownTime_s = INVINCIBLE_TIME_SECONDS;
		
		//�_�ł���
		m_blink->Start();

		//�V�[���h���}�C�i�X�ɂȂ����玀�S����
		if (m_shieldCount <= -1)
		{
			SetActive(false);
		}

	}

}


/// <summary>
/// �e����
/// </summary>
/// <param name="shadow">�V���h�E�}�b�v�̐��|�C���^</param>
/// <param name="view">�r���[�s��</param>
/// <param name="projection">�v���W�F�N�V�����s��</param>
void Player::CreateShadow(ShadowMap* shadow, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	//�f�o�C�X���\�[�X�擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//�f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//���f��������Ήe�𐶐�����
	if (m_playerModel[m_playerModelNum[m_modelTime_s]] != nullptr)
	{
		//���[���h�s����v�Z����
		CalculationWorld();

		//�e����
		m_playerModel[m_playerModelNum[m_modelTime_s]]->Draw(context, *GetCommonState(), GetWorld(), view, projection, false, [&]()
			{
				shadow->DrawShadowMap(context);
			}
		);
	}


}

/// <summary>
/// ���Z�b�g
/// </summary>
void Player::Reset()
{
	SetActive(false);
}

/// <summary>
/// �v���C���[�̃��f���쐬
/// </summary>
void Player::CreatePlayerModel()
{
	//���f���t�@�C���p�X�̐����A���f���z��m��
	m_playerModel.resize(m_modelFiles.size());

	//���f���̓ǂݍ���
	for (int i = 0; i < m_modelFiles.size();i++)
	{
		m_playerModel[i] = ModelManager::GetInstance().LoadModel(m_modelFiles[i].c_str());
	}

}

/// <summary>
/// �����蔻��̗̈�̍X�V
/// </summary>
void Player::HitAreaUpdate()
{
	DirectX::SimpleMath::Vector3 position = GetPosition();
	//AABB�����蔻��̗̈�̐ݒ�
	GetAABB()->SetData(position + AABB_AREA_DOWN, position + AABB_AREA_UP);

	//�J�v�Z�������蔻��̗̈�̐ݒ�
	GetCapsule()->a = DirectX::SimpleMath::Vector3(position.x, position.y + CAPSULE_AREA_DOWN, position.z);
	GetCapsule()->b = DirectX::SimpleMath::Vector3(position.x, position.y + CAPSULE_AREA_UP, position.z);

}

/// <summary>
/// �v���C���[�̓���
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void Player::PlayerMove(const DX::StepTimer& timer)
{
	//�L�[�z�񂩂炻�ꂼ��̊��蓖�Ă�ꂽ�L�[���擾
	//�E�L�[
	const DirectX::Keyboard::Keys& right =    m_keys[0];
	//���L�[
	const DirectX::Keyboard::Keys& left =     m_keys[1];
	//�O�L�[
	const DirectX::Keyboard::Keys& forward =  m_keys[2];
	//���L�[
	const DirectX::Keyboard::Keys& back = m_keys[3];
	//�W�����v�L�[
	const DirectX::Keyboard::Keys& jump =     m_keys[4];

	//�o�ߎ���
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	// �L�[���͏����擾����
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	//�x���V�e�B�擾
	DirectX::SimpleMath::Vector3 velocity = GetVelocity();
	//���W�擾
	DirectX::SimpleMath::Vector3 position = GetPosition();
	//�p�x�擾
	DirectX::SimpleMath::Vector3 rotation = GetRotation().ToEuler();

	//�x���V�e�B�̂w�Ƃy���O�ɂ���
	velocity.x = 0.0f;
	velocity.z = 0.0f;

	//�ړ����Ă��邩
	bool IsMove = false;

	//90�x
	static const float NINETY_ANGLE = DirectX::XM_PI / 2.0f;

	//�E�L�[�������Ă����ꍇ�E�Ɉړ����E������
	if (keyState.IsKeyDown(right))
	{
		velocity.x += MOVE_SPEED * elapsedTime;

		rotation.y = -NINETY_ANGLE;

		IsMove = true;

	}
	//���L�[�������Ă����ꍇ�E�Ɉړ�����������
	else if (keyState.IsKeyDown(left))
	{
		velocity.x -= MOVE_SPEED * elapsedTime;

		rotation.y = NINETY_ANGLE;

		IsMove = true;
	}

	//���L�[�������Ă����ꍇ�E�Ɉړ�����������
	 if (keyState.IsKeyDown(back))
	{
		velocity.z += MOVE_SPEED * elapsedTime;

		rotation.y = NINETY_ANGLE * 2.0f;

		IsMove = true;
	}
	//�O�L�[�������Ă����ꍇ�E�Ɉړ����O������
	else if (keyState.IsKeyDown(forward))
	{
		velocity.z -= MOVE_SPEED * elapsedTime;

		rotation.y = 0;

		IsMove = true;
	}

	 //���L�[�ƑO�L�[�������Ă����ꍇ���O������
	 if ((keyState.IsKeyDown(left)) && (keyState.IsKeyDown(forward)))
	 {
		 //45
		 rotation.y = NINETY_ANGLE / 2.0f;
	 }

	 //���L�[�ƌ��L�[�������Ă����ꍇ����������
	 if ((keyState.IsKeyDown(left)) && (keyState.IsKeyDown(back)))
	 {
		 //90+45
		 rotation.y = NINETY_ANGLE + NINETY_ANGLE / 2.0f;
	 }

	//�E�L�[�ƑO�L�[�������Ă����ꍇ�E�O������
	if ((keyState.IsKeyDown(right)) && (keyState.IsKeyDown(forward)))
	{
		//-45
		rotation.y = -NINETY_ANGLE / 2.0f;
	}

	//�E�L�[�ƌ��L�[�������Ă����ꍇ�E��������
	if ((keyState.IsKeyDown(right)) && (keyState.IsKeyDown(back)))
	{
		//-(90+45)
		rotation.y = -(NINETY_ANGLE + NINETY_ANGLE / 2.0f);
	}
		

	//�ړ����Ă���ꍇ���f���^�C���𑝂₷
	if (IsMove)
	{
		m_modelTime_s += MODEL_TIME_SPEED * elapsedTime;;

		//���f���^�C�����S�𒴂�����O�Ƀ��Z�b�g
		if (m_modelTime_s >= MAX_MODEL_TIME_S)
		{
			m_modelTime_s = 0.0f;
		}
	}
	//�ړ����Ă��Ȃ��ꍇ���f���^�C���͂O�ɂ���
	else
	{
		m_modelTime_s = 0.0f;
	}


	//�X�e�[�W�ɓ������Ă���ꍇ�������Ȃ�
	if (m_stageManager->StageToActorHitCheck(this))
	{
		//�x���V�e�B�x���O�ɂ���
		velocity.y = 0;

		//�W�����v�L�[����������W�����v����
		if (keyState.IsKeyDown(jump))
		{
			
			velocity.y += JUMP_FORCE;

			//�W�����v�����特���o��
			m_pAdx2->Play(CRI_CUESHEET_0_JUMP08);

		}

	}
	//�X�e�[�W�ɓ������Ă��Ȃ��ꍇ��������
	else
	{
		//���f���^�C����MAX�ɂ���
		m_modelTime_s = MAX_MODEL_TIME_S;
		//��������
		velocity.y += GRAVITY_FORCE * static_cast<float>(timer.GetElapsedSeconds());

	}

	//�x���V�e�B�̐ݒ�
	SetVelocity(velocity);
	//�ړ�����
	SetPosition(GetPosition() + velocity);
	//�p�x�ݒ�
	SetRotation(rotation);
}
