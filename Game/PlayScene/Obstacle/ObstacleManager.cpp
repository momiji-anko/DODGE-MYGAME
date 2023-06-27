/*
* 2023/04/15
* ObstacleManeger.cpp
* ��Q���Ǘ��N���X
* �����@��
*/
#include"pch.h"
#include"ObstacleManager.h"
#include<Effects.h>
#include"DeviceResources.h"
#include"../MyRandom.h"
#include"../Player/Player.h"
#include"../../GameMain.h"
#include"Libraries/MyLibraries/ModelManager.h"

//��Q���ő吔
const int ObstacleManager::OBSTACLE_MAX_NUM = 100;
//�X�|�[���N�[���^�C��
const int ObstacleManager::EFFECT_MAX_NUM = 5;
//���G�t�F�N�g�̍ő吔
const float ObstacleManager::SPAWN_COOL_TIME_S = 3.0f;
//�X�|�[���N�[���^�C���̌��鐔
const float ObstacleManager::SPAWNE_COOL_TIME_DECREASE_AMOUNT = 0.003f;

/// <summary>
/// �R���X�g���N�^
/// </summary>
ObstacleManager::ObstacleManager()
	:
	m_obstacles{},
	m_spawners{},
	m_models{},
	m_normalSpawnePosition{ 
		{DirectX::SimpleMath::Vector3(19,0.0f,-19) },
		{DirectX::SimpleMath::Vector3(19,0.0f,19) },
		{DirectX::SimpleMath::Vector3(-19,0.0f,19) },
		{DirectX::SimpleMath::Vector3(-19,0.0f,-19) },
	},
	m_stickSpawnePosition{ 
		{DirectX::SimpleMath::Vector3(19.0f,0.0f,0.0f) },
		{DirectX::SimpleMath::Vector3(0.0f,0.0f,-19.0f) },
		{DirectX::SimpleMath::Vector3(-19.0f,0.0f,0.0f) },
		{DirectX::SimpleMath::Vector3(0.0f,0.0f,19) },
	},
	m_birdSpawnPosition{
		{DirectX::SimpleMath::Vector3(19.0f ,  3.0f ,0.0f) },
		{DirectX::SimpleMath::Vector3(0.0f  ,  3.0f ,-19.0f) },
		{DirectX::SimpleMath::Vector3(-19.0f,  3.0f ,0.0f) },
		{DirectX::SimpleMath::Vector3(0.0f  ,  3.0f ,19.0f) },
	},
	m_hitvel{0,0,0},
	m_time_s(0.0f),
	m_spawneCoolTime_s(0.0f),
	m_spawneTime_s(0.0f),
	m_hitType(Obstacle::ObstacleType::NONE)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ObstacleManager::~ObstacleManager()
{
	
}

/// <summary>
/// ������
/// </summary>
/// <param name="stage">�X�e�[�W�̔ԍ�</param>
void ObstacleManager::Initialize(StageManager::StageSelect stage)
{


	//�G�t�F�N�g�z����G�t�F�N�g�̍ő吔�Ń��T�C�Y
	m_effectlist.resize(EFFECT_MAX_NUM);
	//�G�t�F�N�g�̍쐬
	for (int i = 0; i < m_effectlist.size(); i++)
	{
		m_effectlist[i] = std::make_unique<FireEffectManager>();
		m_effectlist[i]->Create();
		//�G�t�F�N�g�̏�����
		m_effectlist[i]->Initialize(2.0f, DirectX::SimpleMath::Vector3::Zero);
	}

	//��Q���̔z�����Q���̍ő吔�Ń��T�C�Y
	m_obstacles.resize(OBSTACLE_MAX_NUM);
	//��Q���̍쐬
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		//��Q���̍쐬
		std::unique_ptr<Obstacle> obs = std::make_unique<Obstacle>();
		//�����_���ŉ��̃G�t�F�N�g����Q���ɓn��
		int num = MyRandom::GetIntRange(0, EFFECT_MAX_NUM - 1);
		//�G�t�F�N�g��n��
		obs->SetEffect(m_effectlist[num].get());
		//��Q���^����A�N�^�[�^�ɂ���
		obstacle = std::move(obs);
		
	}
	//��Q���X�|�i�쐬
	CreateSpawner();
	//��Q���r�w�C�r�A�[�쐬
	CreateBehavior();
	
	//�X�|�[���N�[���^�C���ݒ�
	m_spawneCoolTime_s = SPAWN_COOL_TIME_S;
	//�X�|�[���^�C���ݒ�
	m_spawneTime_s = SPAWN_COOL_TIME_S;

	//��Q�����f���쐬
	CreateModel();
	
	//Stage1��Stage2�ł���Ή�]����_�𐶐�����
	if (stage == StageManager::StageSelect::Stage1 || stage == StageManager::StageSelect::Stage2)
	{
		//��]����_�̐���
		CreateObstacle(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), Obstacle::ObstacleType::ROTATESTICK, DirectX::SimpleMath::Vector3::Zero);
		CreateObstacle(DirectX::SimpleMath::Vector3(0.0f, 3.0f, 0.0f), Obstacle::ObstacleType::REVERSE_ROTATESTICK, DirectX::SimpleMath::Vector3::Zero);
	}

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void ObstacleManager::Update(const DX::StepTimer& timer)
{
	if (m_hitType != Obstacle::ObstacleType::NONE)
	{
		return;
	}

	//�o�ߎ���
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	//�^�C�����o�ߎ��Ԃő���
	m_time_s += elapsedTime;

	//�X�|�[���^�C�����o�ߎ��Ԃň���
	m_spawneTime_s -= elapsedTime;

	//�X�|�[���^�C�����O�ɂȂ������Q���𐶐�����
	if (m_spawneTime_s <= 0.0f)
	{
		static const float LEVEL1_CREATE_OBSTACLE_TIME_S = 50.0f;

		//��Q���̊p�x�Əo���ʒu�����߂�
		int obstacleRand = MyRandom::GetIntRange(2, 3);

		//�p�x
		DirectX::SimpleMath::Vector3 rot = DirectX::SimpleMath::Vector3::Zero;
		//�p�x�ݒ�		
		rot.y = (DirectX::XM_PI / 2.0f) * obstacleRand;



		//�O����P�O�O�̗��������
		int createObsRand = MyRandom::GetIntRange(0, 100);

		//�^�C����50�ȉ��ł���΃m�[�}���̉��Ɩ_�̏�Q���������_���Ő�������
		if (m_time_s <= LEVEL1_CREATE_OBSTACLE_TIME_S)
		{
			

			//�����ŏo���������Q���̃{�[�_�[���C��
			static const int NOERMAL_FIRE_SPAWNE_NUM = 80;
			static const int STICK_SPAWNE_NUM = 100;

			//������80�ȉ��ł���Ε��ʂ̉��𐶐�����
			if(createObsRand <= NOERMAL_FIRE_SPAWNE_NUM)
			{
				//�o��������ʒu����v���C���[�̃x�N�g�������������ݒ肷��
				rot.y = atan2(m_normalSpawnePosition[obstacleRand].x - m_playerPosition.x, m_normalSpawnePosition[obstacleRand].z - m_playerPosition.z);
				//���ʂ̉�����
				CreateObstacle(m_normalSpawnePosition[obstacleRand], Obstacle::ObstacleType::NORMAL, rot);
			}
			//�������W�P�ȏ�P�O�O�ȉ��ł���Ζ_�̏�Q�����쐬
			else if (createObsRand <= STICK_SPAWNE_NUM)
			{
				//�_�̏�Q������
				CreateObstacle(m_stickSpawnePosition[obstacleRand], Obstacle::ObstacleType::STICK, rot);
			}
		}
		//�^�C����50���傫�Ȃ���΃m�[�}���̉��Ɩ_�ƒ��Ǝ֍s���鉊�̏�Q���������_���Ő�������
		else if(m_time_s > LEVEL1_CREATE_OBSTACLE_TIME_S)
		{
			//�����ŏo���������Q���̃{�[�_�[���C��
			static const int NOERMAL_FIRE_SPAWNE_NUM = 30;
			static const int STICK_SPAWNE_NUM = 45;
			static const int BIRD_SPAWNE_NUM = 60;
			static const int MEANDERING_FIRE_SPAWNE_NUM = 100;

			//�������R�O�ȉ��ł���Ε��ʂ̉��𐶐�����
			if (createObsRand <= NOERMAL_FIRE_SPAWNE_NUM)
			{
				//�o��������ʒu����v���C���[�̃x�N�g�������������ݒ肷��
				rot.y = atan2(m_normalSpawnePosition[obstacleRand].x - m_playerPosition.x, m_normalSpawnePosition[obstacleRand].z - m_playerPosition.z);
				//���ʂ̉�����
				CreateObstacle(m_normalSpawnePosition[obstacleRand], Obstacle::ObstacleType::NORMAL, rot);

			}
			//�������R�P�ȏ�S�T�ȉ��ł���Ζ_�̏�Q�����쐬
			else if (createObsRand <= STICK_SPAWNE_NUM)
			{
				//�_�̏�Q������
				CreateObstacle(m_stickSpawnePosition[obstacleRand], Obstacle::ObstacleType::STICK, rot);
			}
			//�������S�U�ȏ�T�O�ȉ��ł���Β��̏�Q�����쐬
			else if (createObsRand <= BIRD_SPAWNE_NUM)
			{
				//���̏�Q�����쐬
				CreateObstacle(m_birdSpawnPosition[obstacleRand], Obstacle::ObstacleType::BIRD, rot);
			
			}
			//�������T�P�ȏ�P�O�O�ȉ��ł���Ύ֍s���鉊�̏�Q�����쐬
			else if (createObsRand <= MEANDERING_FIRE_SPAWNE_NUM)
			{
				//�֍s���鉊�̏�Q�����쐬
				CreateObstacle(m_normalSpawnePosition[obstacleRand], Obstacle::ObstacleType::MEANDERING, rot);
			}
		}
		

		//�X�|�[���^�C���ɃX�|�[���N�[���^�C��������
		m_spawneTime_s = m_spawneCoolTime_s;
		//���񂾂�X�|�[���N�[���^�C�������炵�ăX�|�[���^�C���𑁂�����
		m_spawneCoolTime_s -= SPAWNE_COOL_TIME_DECREASE_AMOUNT;
	}

	
	//��Q���̍X�V
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		//��A�N�e�B�u��Ԃł���΍X�V���Ȃ�
		if (obstacle->IsActive() == false)
			continue;
		
		//�X�V
		obstacle->Update(timer);
	}
}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̐��|�C���^</param>
void ObstacleManager::Draw(Camera* camera)
{
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		//��A�N�e�B�u��Ԃł���Ε`�悵�Ȃ�
		if (obstacle->IsActive() == false)
			continue;

		//��Q���̕`��
		obstacle->Draw(camera);
	}

}

/// <summary>
/// �I������
/// </summary>
void ObstacleManager::Finalize()
{

}

/// <summary>
/// �X�|�i�[�쐬
/// </summary>
void ObstacleManager::CreateSpawner()
{
	//���ʂ̉�
	m_spawners[Obstacle::ObstacleType::NORMAL] = std::make_unique<NormalObstacleSpawner>();
	//�_
	m_spawners[Obstacle::ObstacleType::STICK] = std::make_unique<StickObstacleSpawner>();
	//覐�
	m_spawners[Obstacle::ObstacleType::METEORITE] = std::make_unique<MeteoriteObstacleSpawner>();
	//��]����_
	m_spawners[Obstacle::ObstacleType::ROTATESTICK] = std::make_unique<RotateStickObstacleSpawner>();
	//
	m_spawners[Obstacle::ObstacleType::REVERSE_ROTATESTICK] = std::make_unique<ReverseRotateStickObstacleSpawner>();
	//�֍s���鉊
	m_spawners[Obstacle::ObstacleType::MEANDERING] = std::make_unique<MeanderingObstacleSpawner>();
	//��
	m_spawners[Obstacle::ObstacleType::BIRD] = std::make_unique<BirdObstacleSpawner>();
}


/// <summary>
/// �r�w�C�r�A�[�쐬
/// </summary>
void ObstacleManager::CreateBehavior()
{
	//���ʂ̉�
	m_behavior[Obstacle::ObstacleType::NORMAL] = std::make_unique<NormalBehavior>();
	//�_
	m_behavior[Obstacle::ObstacleType::STICK] = std::make_unique<StickBehavior>();
	//覐�
	m_behavior[Obstacle::ObstacleType::METEORITE] = std::make_unique<MeteoriteBehavior>();
	//��]����_
	m_behavior[Obstacle::ObstacleType::ROTATESTICK] = std::make_unique<RotateStickBehavior>();
	//���v���ɉ�]����_
	m_behavior[Obstacle::ObstacleType::REVERSE_ROTATESTICK] = std::make_unique<ReverseRotateStickBehavior>();
	//�֍s���鉊
	m_behavior[Obstacle::ObstacleType::MEANDERING] = std::make_unique<MeanderingBehavior>();
	//��
	m_behavior[Obstacle::ObstacleType::BIRD] = std::make_unique<BirdBehavior>();

}

/// <summary>
/// �v���C���[�̍��W�ݒ�
/// </summary>
/// <param name="position">�v���C���[�̍��W</param>
void ObstacleManager::SetPlayerPosition(const DirectX::SimpleMath::Vector3& position)
{
	//�v���C���[�̍��W�ݒ�
	m_playerPosition = position;

	//�S�Ă̏�Q���Ƀv���C���[�̍��W�ݒ�
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{

		//�v���C���[�̍��W�ݒ�
		obstacle->SetPlayerPosition(m_playerPosition);
	}

}

/// <summary>
/// �v���C���[�Ə�Q���̃J�v�Z���̓����蔻��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <returns>true = �������� , false = �����Ă��Ȃ�</returns>
bool ObstacleManager::PlayerHitCheck(AABBFor3D* playerAABB)
{
	//�S�Ă̏�Q���Ƀv���C���[�����蔻��
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		//��Q�����A�N�e�B�u��ԂłȂ���Ώ������Ȃ�
		if (obstacle->IsActive() == false)
			continue;

		//��Q�����v���C���[�Ɠ����蔻������
		if (obstacle->GetAABB()->HitCheck(playerAABB))
		{

			//�v���C���[�ɓ���������Q���̃^�C�v�ݒ�
			m_hitType = static_cast<Obstacle::ObstacleType>(obstacle->GetTypeInt());
			
			//�������Ă���
			return true;
		}
		
	}
	//�S�Ă̏�Q���𓖂��Ă��Ȃ�
	return false;
}


/// <summary>
/// ��Q���̉e�쐬
/// </summary>
/// <param name="shadowMap">�V���h�E�}�b�v�̐��|�C���^</param>
/// <param name="view">�r���[�s��</param>
/// <param name="projection">�v���W�F�N�V�����s��</param>
void ObstacleManager::ObstacleShadow( ShadowMap* shadowMap, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	//�S�Ă̏�Q���̉e�𐶐�����
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		//��Q�����A�N�e�B�u��ԂłȂ���Ώ������Ȃ�
		if (obstacle->IsActive() == false)
			continue;

		//�e�̐���
		obstacle->CreateShadow(shadowMap, view, projection);
	}

}

/// <summary>
/// �v���C���[�Ə�Q���̃J�v�Z���̓����蔻��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <returns>true = �������� , false = �����Ă��Ȃ�</returns>
bool ObstacleManager::PlayerCapsuleHitCheck(Actor* player, DirectX::SimpleMath::Vector3* flyVelocity)
{
	//��Q���ƃv���C���[�̃J�v�Z���̓����蔻��
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		//��A�N�e�B�u��Ԃł���Ύ��s���Ȃ�
		if (obstacle->IsActive() == false)
			continue;

		//��Q���̊p�x�擾
		Capsule* cap = obstacle->GetCapsule();
		//�������Ă���Ύ��s����
		if (CapsuleHit::HitCheck_Capsule2Capsule(*cap, *player->GetCapsule(),m_capsuleHitC1,m_capsuleHitC2))
		{
			bool isRoteStick = obstacle->GetTypeInt() == static_cast<int>(Obstacle::ObstacleType::ROTATESTICK);

			//���̉�]�_�ɓ����������̏���
			if ( isRoteStick)
			{
				*flyVelocity = RotaStickHit(obstacle.get(), player);
			}

			bool isReveseRoteStick = obstacle->GetTypeInt() == static_cast<int>(Obstacle::ObstacleType::REVERSE_ROTATESTICK);

			//��̉�]����_��Q���ɓ����������̏���
			if ( isReveseRoteStick)
			{
				*flyVelocity = ReverseRotaStickHit(obstacle.get(), player);

			}
			return true;
		}

	}
	return false;
}


/// <summary>
/// ��Q���̐���
/// </summary>
/// <param name="position">�������W</param>
/// <param name="type">��Q���̃^�C�v</param>
/// <param name="rot">�A���O��</param>
/// <returns>true = �������� , false = �������s</returns>
bool ObstacleManager::CreateObstacle(const DirectX::SimpleMath::Vector3& position, Obstacle::ObstacleType type, const DirectX::SimpleMath::Vector3& rot)
{
	return m_spawners[type]->Create(m_obstacles, position, rot, m_behavior[type].get(), m_models[type]);
}

/// <summary>
/// ��Q���̃��f������
/// ��Q���̃��f������
/// </summary>
void ObstacleManager::CreateModel()
{
	//���f���}�l�[�W���[�擾
	ModelManager& modelManager = ModelManager::GetInstance();

	//	覐΃��f��
	m_models[Obstacle::ObstacleType::METEORITE] = modelManager.LoadModel(L"Resources/Models/star.cmo");

	//	�_���f��
	m_models[Obstacle::ObstacleType::STICK] = modelManager.LoadModel(L"Resources/Models/stickObstale.cmo"); 
	
	//�@���_���f��
	m_models[Obstacle::ObstacleType::ROTATESTICK] = modelManager.LoadModel(L"Resources/Models/roll.cmo"); 

	//	�����f��
	m_models[Obstacle::ObstacleType::BIRD] = modelManager.LoadModel(L"Resources/Models/bule.cmo"); 

	//	���΂ɉ��_���f��
	m_models[Obstacle::ObstacleType::REVERSE_ROTATESTICK] = modelManager.LoadModel(L"Resources/Models/roll2.cmo"); 
}


/// <summary>
/// �v���C���[�𐁂���΂�	
/// </summary>
/// <param name="rotSpeed">��Q���̉�]���x</param>
/// <param name="player">�v���C���[</param>
DirectX::SimpleMath::Vector3 ObstacleManager::FlyPlayer(float rotSpeed)
{

	//��΂����߂ɃX�s�[�h�ɂQ�|����
	float speedTwice = 2.0f;
	//��΂����߂̕␳
	float speedCorrection = 0.5f;
	//��ԗ�
	float flyAmount = speedCorrection + (std::abs(rotSpeed) * speedTwice);

	return m_hitvel * flyAmount;

}

/// <summary>
/// �v���C���[�̂߂荞�ݏ���
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="cupseleToCupseVector">���������J�v�Z���ƃJ�v�Z���̃x�N�g��</param>
/// <param name="playerCapsleRadius">�v���C���[�̃J�v�Z���̔��a</param>
/// <param name="obstacleCupsleRadius">��Q���̃J�v�Z���̔��a</param>
void ObstacleManager::PlayerCapuslePenetration(Actor* player, const DirectX::SimpleMath::Vector3& cupseleToCupseVector, float playerCapsleRadius, float obstacleCupsleRadius)
{
	//���������J�v�Z���ƃJ�v�Z���̋���
	float cupseleToCupselLenth = cupseleToCupseVector.Length();
	//�v���C���[�Ə�Q���̔��a�̍��v
	float radius = playerCapsleRadius + obstacleCupsleRadius;

	DirectX::SimpleMath::Vector3 capToCapVector= cupseleToCupseVector;

	//���������J�v�Z���ƃJ�v�Z���̋������v���C���[�Ə�Q���̔��a�̍��v��菬������΂߂荞�ݏ���������
	if (cupseleToCupselLenth < radius)
	{
		//�ǂꂾ���߂荞��ł��邩�v�Z
		float capToCapLengthToRasiusDifference = cupseleToCupselLenth - radius;
		//���������J�v�Z���ƃJ�v�Z���̃x�N�g���𐳋K��
		capToCapVector.Normalize();
		//���������J�v�Z���ƃJ�v�Z���̃x�N�g���ɂ߂荞�񂾗ʂ�������		
		DirectX::SimpleMath::Vector3 playerPenetrationVelocity = capToCapVector * capToCapLengthToRasiusDifference;
		//�v���C���[�̂߂荞�񂾗ʂ������o��
		player->SetPosition(player->GetPosition() + playerPenetrationVelocity);
	}


}

/// <summary>
/// ��]����_�ɓ����������̏���
/// </summary>
/// <param name="obstacle">��]����_�̏�Q��</param>
/// <param name="player">�v���C���[</param>
DirectX::SimpleMath::Vector3 ObstacleManager::RotaStickHit(Actor* obstacle, Actor* player)
{
	//��Q���̊p�x�擾
	DirectX::SimpleMath::Vector3 rot = obstacle->GetRotation().ToEuler();

	//��Q���J�v�Z������v���C���[�J�v�Z���̃x�N�g��
	m_hitvel = m_capsuleHitC2 - m_capsuleHitC1;

	//�v���C���[�J�v�Z���̔��a
	float playerR = player->GetCapsule()->r;
	//��Q���J�v�Z���̔��a
	float obstacleR = obstacle->GetCapsule()->r;
	//�߂荞�ݏ���
	PlayerCapuslePenetration(player, -m_hitvel, playerR, obstacleR);

	DirectX::SimpleMath::Vector3 flyVel = DirectX::SimpleMath::Vector3::Zero;

	//���p
	static const float NINETY_ANGLE = DirectX::XMConvertToDegrees(DirectX::XM_PI / 2.0f);

	//��Q���̉�]�����̌v�Z
	DirectX::SimpleMath::Vector3 rotDir = DirectX::SimpleMath::Vector3(cos(rot.y + (NINETY_ANGLE)), 0, -sin(rot.y + (NINETY_ANGLE)));

	//��Q���J�v�Z������v���C���[�J�v�Z���̃x�N�g�����K������
	m_hitvel.Normalize();

	//��Q���J�v�Z������v���C���[�J�v�Z���̃x�N�g���Ə�Q���̉�]�����œ��ς����
	float dot = m_hitvel.Dot(rotDir);

	//���ς��O�ȏ�ł���΃v���C���[�𐁂���΂�
	if (dot > 0.0f)
	{
		flyVel = FlyPlayer(obstacle->GetRotSpeed());
	}

	return flyVel;

}

/// <summary>
/// ���Ή�]����_�ɓ����������̏���
/// </summary>
/// <param name="obstacle">��]����_�̏�Q��</param>
/// <param name="player">�v���C���[</param>	
DirectX::SimpleMath::Vector3 ObstacleManager::ReverseRotaStickHit(Actor* obstacle, Actor* player)
{

	//���������J�v�Z�����m�̍��W�𑫂�
	m_hitvel = m_capsuleHitC2 + m_capsuleHitC1;

	//���K��
	m_hitvel.Normalize();
	//��Q���̃J�v�Z���̔��a�𑫂��v���C���[�Ə�Q���̐ڐG�����ꏊ���v�Z
	m_hitvel *= obstacle->GetCapsule()->r;

	//�v���C���[�Ə�Q���̐ڐG���W�Ƀv���C���[�̑����̍��W�𑫂�
	DirectX::SimpleMath::Vector3 targetVector = targetVector = m_hitvel + player->GetCapsule()->a;
	//�v���C���[�̑����̍��W�̃v���C���[�̓����W�𑫂�
	DirectX::SimpleMath::Vector3 playerVector = player->GetCapsule()->b + player->GetCapsule()->a;

	DirectX::SimpleMath::Vector3 flyVel = DirectX::SimpleMath::Vector3::Zero;


	//���K��
	targetVector.Normalize();
	playerVector.Normalize();

	//�p�x������o��
	float dot = targetVector.Dot(playerVector);
	float sita = acos(std::abs(dot));

	//���ł��p�x
	float headAngle = 0.0877f;

	//�p�x�����ł��p�x���傫�������瓪��ł������Ƃɂ��A�v���C���[�𑦎�����������
	if (sita >= headAngle)
	{
		DirectX::SimpleMath::Vector3 playerVec = player->GetVelocity();
		player->SetVelocity(DirectX::SimpleMath::Vector3(playerVec.x, 0.0f, playerVec.z));
	}
	else
	{
		//�p�x�����ł��p�x��菬����������v���C���[�𐁂���΂�					
		m_hitvel = m_capsuleHitC2 - m_capsuleHitC1;

		//�v���C���[�J�v�Z���̔��a
		float playerR = player->GetCapsule()->r;
		//��Q���J�v�Z���̔��a
		float obstacleR = obstacle->GetCapsule()->r;

		//�߂荞�ݏ���
		PlayerCapuslePenetration(player, -m_hitvel, playerR, obstacleR);

		//���K��
		m_hitvel.Normalize();

		//��Q���̉�]���x�擾
		float rotSpeed = obstacle->GetRotSpeed();

		//�v���C���[�𐁂���΂�
		flyVel = FlyPlayer(rotSpeed);
	}

	return  flyVel;

}
