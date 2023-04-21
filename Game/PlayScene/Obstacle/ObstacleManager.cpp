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
const int ObstacleManeger::OBSTACLE_MAX_NUM = 100;
//�X�|�[���N�[���^�C��
const int ObstacleManeger::EFFECT_MAX_NUM = 5;
//���G�t�F�N�g�̍ő吔
const float ObstacleManeger::SPAWN_COOL_TIME_S = 3.0f;

/// <summary>
/// �R���X�g���N�^
/// </summary>
ObstacleManeger::ObstacleManeger()
	:
	m_obstacles{},
	m_spawners{},
	m_models{},
	m_commonState(nullptr),
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
	m_spawneTime_s(0.0f)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ObstacleManeger::~ObstacleManeger()
{
	
}

/// <summary>
/// ������
/// </summary>
/// <param name="commonState">�R�����X�e�[�g�̐��|�C���^</param>
/// <param name="stage">�X�e�[�W�̔ԍ�</param>
void ObstacleManeger::Initialize(DirectX::CommonStates* commonState, StageManager::StageSelect stage)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	m_commonState = commonState;


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
void ObstacleManeger::Update(const DX::StepTimer& timer)
{
	//�o�ߎ���
	float elapsedtime = timer.GetElapsedSeconds();
	//�^�C�����o�ߎ��Ԃő���
	m_time_s += elapsedtime;

	//�X�|�[���^�C�����o�ߎ��Ԃň���
	m_spawneTime_s -= elapsedtime;

	//�X�|�[���^�C�����O�ɂȂ������Q���𐶐�����
	if (m_spawneTime_s <= 0.0f)
	{
		
		//�o���������Q���������_���Ō��߂�
		int type = MyRandom::GetIntRange(0, 3);

		//�p�x
		DirectX::SimpleMath::Vector3 rot = DirectX::SimpleMath::Vector3::Zero;
		//�p�x�ݒ�		
		rot.y = (DirectX::XM_PI / 2.0f) * type;

		static const float LEVEL1_CREATE_OBSTACLE_TIME_S = 50.0f;




		//�^�C����LEVEL1_CREATE_OBSTACLE_TIME_S�ȉ��ł���΃m�[�}���̉��Ɩ_�̏�Q���탉���_���Ő�������
		if (m_time_s <= LEVEL1_CREATE_OBSTACLE_TIME_S)
		{
			//�O����P�O�O�̗��������
			int createObsRand = MyRandom::GetIntRange(0, 100);

			//������80�ȉ��ł���Ε��ʂ̉��𐶐�����
			if(createObsRand <= 80)
			{
				//�o��������ʒu����v���C���[�̃x�N�g�������������ݒ肷��
				rot.y = atan2(m_normalSpawnePosition[type].x - m_playerPosition.x, m_normalSpawnePosition[type].z - m_playerPosition.z);
				//���ʂ̉�����
				CreateObstacle(m_normalSpawnePosition[type], Obstacle::ObstacleType::NORMAL, rot);
			}
			//�������W�P�ȏ�P�O�O�ȉ��ł���Ζ_�̏�Q�����쐬
			else if (createObsRand <= 100)
			{
				//�_�̏�Q������
				CreateObstacle(m_stickSpawnePosition[type], Obstacle::ObstacleType::STICK, rot);
			}
		}
		else 
		{

			//�O����P�O�O�̗��������
			int createObsRand = MyRandom::GetIntRange(0, 100);
			//�������R�O�ȉ��ł���Ε��ʂ̉��𐶐�����
			if (createObsRand <= 30)
			{
				//�o��������ʒu����v���C���[�̃x�N�g�������������ݒ肷��
				rot.y = atan2(m_normalSpawnePosition[type].x - m_playerPosition.x, m_normalSpawnePosition[type].z - m_playerPosition.z);
				//���ʂ̉�����
				CreateObstacle(m_normalSpawnePosition[type], Obstacle::ObstacleType::NORMAL, rot);

			}
			//�������R�P�ȏ�S�T�ȉ��ł���Ζ_�̏�Q�����쐬
			else if (createObsRand <= 45)
			{
				//�_�̏�Q������
				CreateObstacle(m_stickSpawnePosition[type], Obstacle::ObstacleType::STICK, rot);
			}
			//�������S�U�ȏ�T�O�ȉ��ł���Β��̏�Q�����쐬
			else if (createObsRand <= 50)
			{
				//���̏�Q�����쐬
				CreateObstacle(m_birdSpawnPosition[type], Obstacle::ObstacleType::BIRD, rot);
			
			}
			//�������T�P�ȏ�P�O�O�ȉ��ł���Ύ֍s���鉊�̏�Q�����쐬
			else if (createObsRand <= 100)
			{
				//�֍s���鉊�̏�Q�����쐬
				CreateObstacle(m_normalSpawnePosition[type], Obstacle::ObstacleType::MEANDERING, rot);
			}
		}
		

		//�X�|�[���^�C���ɃX�|�[���N�[���^�C��������
		m_spawneTime_s = m_spawneCoolTime_s;
		//���񂾂�X�|�[���N�[���^�C�������炵�ăX�|�[���^�C���𑁂�����
		m_spawneCoolTime_s -= 0.003f;
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
void ObstacleManeger::Draw(Camera* camera)
{
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		if (obstacle->IsActive() == false)
			continue;

		obstacle->Draw(camera);
	}

}

/// <summary>
/// �I������
/// </summary>
void ObstacleManeger::Finalize()
{

}

/// <summary>
/// �X�|�i�[�쐬
/// </summary>
void ObstacleManeger::CreateSpawner()
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
void ObstacleManeger::CreateBehavior()
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
void ObstacleManeger::SetPlayerPosition(DirectX::SimpleMath::Vector3 position)
{
	//�v���C���[�̍��W�ݒ�
	m_playerPosition = position;

	//�S�Ă̏�Q���Ƀv���C���[�̍��W�ݒ�
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		//Actor�^����Obstacle�^�Ƀ_�C�i�~�b�N�L���X�g
		Obstacle* obs = dynamic_cast<Obstacle*> (obstacle.get());
		//NULL�`�F�b�N�iNULL�ł���Ύ��s���Ȃ��j
		if (obs == nullptr)
			continue;

		//�v���C���[�̍��W�ݒ�
		obs->SetPlayerPosition(m_playerPosition);
	}

}

/// <summary>
/// �v���C���[�Ə�Q���̃J�v�Z���̓����蔻��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <returns>true = �������� , false = �����Ă��Ȃ�</returns>
bool ObstacleManeger::PlayerHitCheck(AABBFor3D* playerAABB)
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
void ObstacleManeger::ObstacleShadow( ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	//�S�Ă̏�Q���̉e�𐶐�����
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		//��Q�����A�N�e�B�u��ԂłȂ���Ώ������Ȃ�
		if (obstacle->IsActive() == false)
			continue;


		obstacle->CreateShadow(shadowMap, view, projection);
	}

}

/// <summary>
/// �v���C���[�Ə�Q���̃J�v�Z���̓����蔻��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <returns>true = �������� , false = �����Ă��Ȃ�</returns>
bool ObstacleManeger::PlayerCapsuleHitCheck(Player* player)
{
	for (std::unique_ptr<Actor>& obstacle : m_obstacles)
	{
		if (obstacle->IsActive() == false)
			continue;

		Capsule* cap = obstacle->GetCapsule();

		if (HitCheck_Capsule2Capsule(*cap, *player->GetCapsule()))
		{
			DirectX::SimpleMath::Vector3 rot = obstacle->GetRotation().ToEuler();
			Obstacle* ob = dynamic_cast<Obstacle*>(obstacle.get());

			bool isRoteStick = ob->GetObstacleType() == Obstacle::ObstacleType::ROTATESTICK;

			if (ob != nullptr && isRoteStick)
			{
				
				m_hitvel = m_capsuleHitC2 - m_capsuleHitC1;

				m_hitvel.Normalize();

				float playerR = player->GetCapsule()->r;
				float obstacleR = cap->r;
				DirectX::SimpleMath::Vector3 vec = m_capsuleHitC1 - m_capsuleHitC2;
				float len = vec.Length();
				if (len < playerR + obstacleR)
				{
					float a = len - (playerR + obstacleR);
					vec.Normalize();
					vec *= a;
					player->SetPosition(player->GetPosition() + vec);
				}
				float dot = 0;;

				if (ob->GetObstacleType() == Obstacle::ObstacleType::REVERSE_ROTATESTICK)
				{
					dot = m_hitvel.Dot(DirectX::SimpleMath::Vector3(cos(rot.y + (-90)), 0, -sin(rot.y + (-90))));
				}
				else
				{
					dot = m_hitvel.Dot(DirectX::SimpleMath::Vector3(cos(rot.y + (90)), 0, -sin(rot.y + (90))));
				}
				 
				float speed = ob->GetRotSpeed();

				if (speed <	 0)
				{
					speed * -1;
				}

				if (dot > 0.0f)
				{
					player->SetFlyVelocity(DirectX::SimpleMath::Vector3(m_hitvel) * (0.5f + (speed * 2.0f)));

				}
			}

			if (ob != nullptr && ob->GetObstacleType() == Obstacle::ObstacleType::REVERSE_ROTATESTICK)
			{
				m_hitvel = m_capsuleHitC2 + m_capsuleHitC1;

				m_hitvel.Normalize();
				m_hitvel *= cap->r;

				DirectX::SimpleMath::Vector3 targetVector;
				DirectX::SimpleMath::Vector3 playerVector;
				targetVector = m_hitvel + player->GetCapsule()->b;
				playerVector = player->GetCapsule()->a + player->GetCapsule()->b;

				targetVector.Normalize();
				playerVector.Normalize();
				
				float dot = targetVector.Dot(playerVector);
				float sita = acos(dot);
				if (sita < 0)
				{
					sita *= -1.0f;
				}

				if (sita >= 0.0877f)
				{
					 DirectX::SimpleMath::Vector3 playerVec = player->GetVelocity();
					 player->SetVelocity(DirectX::SimpleMath::Vector3(playerVec.x, -0.1f, playerVec.z));
				}
				else 
				{
					m_hitvel = m_capsuleHitC2 - m_capsuleHitC1;

					m_hitvel.Normalize();

					float speed = ob->GetRotSpeed();
					if (speed < 0)
					{
						speed *= -1;
					}

					player->SetFlyVelocity(DirectX::SimpleMath::Vector3(m_hitvel) * (0.5f + (speed * 2.0f)));
				}

				float playerR = player->GetCapsule()->r;
				float obstacleR = cap->r;
				DirectX::SimpleMath::Vector3 vec = m_capsuleHitC1 - m_capsuleHitC2;
				float length = vec.Length();
				if (length < playerR + obstacleR)
				{
					float a = length - (playerR + obstacleR);
					vec.Normalize();
					vec *= a;
					player->SetPosition(player->GetPosition() + vec);
				}

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
bool ObstacleManeger::CreateObstacle(const DirectX::SimpleMath::Vector3& position, Obstacle::ObstacleType type, DirectX::SimpleMath::Vector3 rot)
{
	return m_spawners[type]->Create(m_obstacles, position, rot, m_behavior[type].get(), m_models[type], m_commonState);
}

/// <summary>
/// ��Q���̃��f������
/// ��Q���̃��f������
/// </summary>
void ObstacleManeger::CreateModel()
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
/// �N�����v�֐�
/// </summary>
/// <param name="n">�ۂߏ������s���������l</param>
/// <param name="min">�ۂߏ������s���ŏ��l</param>
/// <param name="max">�ۂߏ������s���ő�l</param>
/// <returns>�ۂߏ������s�������ʂƂȂ鐔�l</returns>
float ObstacleManeger::Clamp(float n, float min, float max)
{
	return std::min(std::max(min, n), max);
}

/// <summary>
/// �Q�̐����̍ŒZ�����̕�����Ԃ��֐�
/// </summary>
/// <param name="p1">���� 1 �̎n�_</param>
/// <param name="q1">���� 1 �̏I�_</param>
/// <param name="p2">���� 2 �̎n�_</param>
/// <param name="q2">���� 2 �̏I�_</param>
/// <param name="s">�����P��̍ŒZ�ʒu��\���W��</param>
/// <param name="t">���� 2 ��̍ŒZ�ʒu��\���W��</param>
/// <param name="c1">�����P��̍ŒZ�����̈ʒu </param>
/// <param name="c2">���� 2 ��̍ŒZ�����̈ʒu</param>
/// <returns>�Q�̐����̍ŒZ�����̕���</returns>
float ObstacleManeger::ClosestPtSegmentSegment(DirectX::SimpleMath::Vector3 p1, DirectX::SimpleMath::Vector3 q1, DirectX::SimpleMath::Vector3 p2, DirectX::SimpleMath::Vector3 q2, float& s, float& t, DirectX::SimpleMath::Vector3& c1, DirectX::SimpleMath::Vector3& c2)
{
	// p1��q1 �̃x�N�g�����Z�o
	DirectX::SimpleMath::Vector3 d1 = q1 - p1;
	// p2��q2 �̃x�N�g�����Z�o
	DirectX::SimpleMath::Vector3 d2 = q2 - p2;
	// ��̃J�v�Z�����������́A�n�_�Ԃ̃x�N�g���𔼌a�̍��v�Ƃ��ĎZ�o
	DirectX::SimpleMath::Vector3 r = p1 - p2;
	// p1��q1 �̃x�N�g���̒����� 2 ����Z�o
	float a = d1.Dot(d1);
	// p2��q2 �̃x�N�g���̒����� 2 ����Z�o
	float e = d2.Dot(d2);
	// p1��q1 �̃x�N�g���Ɣ��a�̍��v�ƂȂ�x�N�g������ς���
	float f = d2.Dot(r);
	if (a <= FLT_EPSILON && e <= FLT_EPSILON)
	{
		// a �� e �̗������ق� 0 ��������A��̃J�v�Z���͕��s�ł���
		s = t = 0.0f;
		// ���Ƃ��Ċe�n�_���l������
		c1 = p1;
		c2 = p2;
		// ���擾�����_���m�̋��������߂�
		return (c1 - c2).Dot(c1 - c2);
	}
	if (a <= FLT_EPSILON)
	{
		// a ���ق� 0 �������ꍇ�́Af/e �� t �Ƃ��Čv�Z����
		// �������A0�`1 �̊Ԃɕ␳����
		s = 0.0f;
		t = f / e;
		t = Clamp(t, 0.0f, 1.0f);
	}
	else
	{
		// p1��q1 �̃x�N�g���Ɣ��a�̍��v����ς���
		float c = d1.Dot(r);
		if (e <= FLT_EPSILON)
		{
			// e ���ق� 0 �������ꍇ�́As �Ƃ���-c/a ���v�Z����B
			// �������A0�`1 �̊Ԃɕ␳����
			t = 0.0f;
			s = Clamp(-c / a, 0.0f, 1.0f);
		}
		else
		{
			// ���� 0 ���Z���N����댯�͂Ȃ��̂ŁA���ʂ�̌v�Z���s��
			// �e�J�v�Z���̃x�N�g���œ��ς���
			float b = d1.Dot(d2);
			// ���ꂼ��̃J�v�Z���̒����� 2 ����|�����l�ƁA
			// �e�J�v�Z���̃x�N�g���œ��ς������ʂ� 2 ����ׂ�
			float denom = a * e - b * b;
			if (denom != 0.0f)
			{
				// ����������Ă���΁A���ʂ�̌v�Z���s��
				s = Clamp((b * f - c * e) / denom, 0.0f, 1.0f);
			}
			else
			{
				// ��̃J�v�Z���̌������ق� 90 �x�̏ꍇ�As �� 0 �Ƃ���
				s = 0.0f;
			}
			// b * s + f ���v�Z���Č��؂���
			float tnom = (b * s + f);
			if (tnom < 0.0f)
			{
				// �}�C�i�X�̒l�ɂȂ��Ă�����A-c / a �̒l�� s �Ƃ���
				t = 0.0f;
				s = Clamp(-c / a, 0.0f, 1.0f);
			}
			else if (tnom > e)
			{
				// e ���傫����΁A(b - c) / a �̒l�� s �Ƃ���
				t = 1.0f;
				s = Clamp((b - c) / a, 0.0f, 1.0f);
			}
			else
			{
				// ���ʂ�̌v�Z���s��
				t = tnom / e;
			}
		}
	}
	// ���ꂼ��̃J�v�Z���ɂ�����A��ԋ߂��_���Z�o����
	c1 = p1 + d1 * s;
	c2 = p2 + d2 * t;

	m_hitvel = (c1 - c2);

	m_capsuleHitC1 = c1;
	m_capsuleHitC2 = c2;

	// ��ԋ߂���_�̋����̓���Ԃ�
	return (c1 - c2).Dot(c1 - c2);
}

/// <summary>
/// �J�v�Z���ƃJ�v�Z���̏Փ˔���֐�
/// </summary>
/// <param name="capsule1">�����蔻�����肽���J�v�Z�� 1 ��</param>
/// <param name="capsule2">�����蔻�����肽���J�v�Z�� 2 ��</param>
/// <returns>true= ���������Afalse=�������ĂȂ�</returns>
bool ObstacleManeger::HitCheck_Capsule2Capsule(Capsule capsule1, Capsule capsule2)
{
	float s, t;
	DirectX::SimpleMath::Vector3 c1, c2;
	// �J�v�Z���̒��S�̐����Ԃ̋����̕������v�Z
	// s, t, c1, c2 �͖߂�l
	float dist2 = ClosestPtSegmentSegment(capsule1.a, capsule1.b, capsule2.a, capsule2.b, s, t, c1, c2);
	float radius = capsule1.r + capsule2.r;
	return dist2 <= radius * radius;
}
