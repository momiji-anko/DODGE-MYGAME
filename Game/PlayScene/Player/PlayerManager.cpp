/*
* 2023/06/12
* PlayerManager.cpp
* �v���C���[�}�l�[�W���[
* �����@��
*/#include"pch.h"
#include"PlayerManager.h"
#include"Game/PlayScene/MyRandom.h"
#include"Game/PlayScene/GameContext/GameContext.h"
#include"Game/PlayScene/AliveTimer.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="playerMode">�v���C���[���[�h</param>
/// <param name="stageManager">�X�e�[�W�}�l�[�W���[</param>
/// <param name="itemManager">�A�C�e���}�l�[�W���[</param>
/// <param name="obstacleManager">��Q���}�l�[�W���[</param>
PlayerManager::PlayerManager(GameMain::PlayerMode playerMode,StageManager* stageManager, ItemManager* itemManager, ObstacleManager* obstacleManager)
	:
	m_players{},
	m_playerMode(playerMode),
	m_stageManager(stageManager),
	m_itemManager(itemManager),
	m_obstacleManager(obstacleManager)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerManager::~PlayerManager()
{
}

/// <summary>
/// ������
/// </summary>
void PlayerManager::Initialize()
{
	CreatePlayer();
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void PlayerManager::Update(const DX::StepTimer& timer)
{
	//�v���C���[�̍X�V
	for (std::unique_ptr<Player>& player : m_players)
	{
		player->Update(timer);
	}

	//�S�v���C���[�̏��̐���-1�ł���΃^�C�}�[���~�߂�
	TimerStopCheck();

}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̐��|�C���^</param>
void PlayerManager::Draw(Camera* camera)
{
	//�v���C���[�̕`��
	for (std::unique_ptr<Player>& player : m_players)
	{
		player->Draw(camera);
	}

	//�v���C���[���[�h���}���`���[�h�̎�
	if (m_playerMode == GameMain::PlayerMode::Player2)
	{
		DirectX::SimpleMath::Vector3 c1;
		DirectX::SimpleMath::Vector3 c2;
		//�v���C���[���m���������Ă����ꍇ������������
		if (CapsuleHit::HitCheck_Capsule2Capsule(*m_players[0]->GetCapsule(), *m_players[1]->GetCapsule(), c1, c2))
		{
			PlayerCapuslePenetration(m_players[0].get(), c2 - c1, m_players[0]->GetCapsule()->r, m_players[1]->GetCapsule()->r);
			PlayerCapuslePenetration(m_players[1].get(), c1 - c2, m_players[1]->GetCapsule()->r, m_players[0]->GetCapsule()->r);
		}
	}
}

/// <summary>
/// �I������
/// </summary>
void PlayerManager::Finalize()
{
}

/// <summary>
/// �P�����Q���̂ǂ��炩�̍��W���擾
/// </summary>
/// <returns>�v���C���[���W</returns>
DirectX::SimpleMath::Vector3 PlayerManager::GetRandomPlayerPosition()
{
	//�v���C���[�̔ԍ�
	size_t randomPlayer = static_cast<size_t>(MyRandom::GetIntRange(0, static_cast<int>(m_players.size() - 1)));
	//�v���C���[���W��Ԃ�
	return m_players[randomPlayer]->GetPosition();
}

/// <summary>
/// �v���C���[���S�����S���Ă���m�F
/// </summary>
/// <returns>true=�S�����S,false=�S�����S���Ă��Ȃ�</returns>
bool PlayerManager::GetPlayerIsAllDaed()
{
	//�v���C���[�̃f�X�J�E���g
	int  deathCount = 0;

	//�S�Ẵv���C���[������ł��邩�m�F
	for (std::vector<std::unique_ptr<Player>>::iterator player = m_players.begin(); player != m_players.end(); ++player)
	{
		//�A�N�e�B�u��ԏ�Ԃł͂Ȃ���΃J�E���g�𑝂₷
		if (!(*player)->IsActive())
		{
			deathCount++;
		}
	}

	//�f�X�J�E���g���v���C���[�̓������ł���ΑS�Ẵv���C���[������ł���
	if (deathCount == m_players.size())
	{
		//�S�Ẵv���C���[������ł���
		return true;
	}

	//�S�Ẵv���C���[������ł��Ȃ�
	return false;
}

void PlayerManager::PlayerShadow(ShadowMap* shadowMap, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	//�v���C���[�̍X�V
	for (std::unique_ptr<Player>& player : m_players)
	{
		if (!player->IsActive())
			continue;

		player->CreateShadow(shadowMap, view, projection);
	}

}

/// <summary>
/// �v���C���[�쐬
/// </summary>
void PlayerManager::CreatePlayer()
{
	//�v���C���[�̃L�[�f�[�^
	std::vector<std::vector<DirectX::Keyboard::Keys>> playerKeyData =
	{
		//�v���C���[�P�̃L�[�f�[�^(�E�A���A�O�A���A�W�����v)
		{DirectX::Keyboard::Keys::Right,DirectX::Keyboard::Keys::Left,DirectX::Keyboard::Keys::Up,DirectX::Keyboard::Keys::Down,DirectX::Keyboard::Keys::RightShift},
		//�v���C���[�Q�̃L�[�f�[�^(�E�A���A�O�A���A�W�����v)
		{DirectX::Keyboard::Keys::D,DirectX::Keyboard::Keys::A,DirectX::Keyboard::Keys::W,DirectX::Keyboard::Keys::S,DirectX::Keyboard::Keys::Space}
	};

	//�v���C���[�̃��f���t�@�C���p�X
	std::vector<std::vector<std::wstring>>playerModelFile = {
		//�v���C���[�P�̃��f���t�@�C���p�X�i�A�C�h����ԁA�����o���Ă����ԁA�E���o���Ă����ԁA�W�����v���Ă����ԁj
		{ L"Resources/Models/playeraidoru.cmo",L"Resources/Models/playerhidari.cmo",L"Resources/Models/playermigiasi.cmo",L"Resources/Models/playerjanp.cmo"},
		//�v���C���[�Q�̃��f���t�@�C���p�X�i�A�C�h����ԁA�����o���Ă����ԁA�E���o���Ă����ԁA�W�����v���Ă����ԁj
		{L"Resources/Models/Player2idoru.cmo",L"Resources/Models/Player2hidari.cmo",L"Resources/Models/Player2Migi.cmo",L"Resources/Models/Player2Janp.cmo"}
	};

	//��l�p�v���C���[�X�^�[�g���W
	DirectX::SimpleMath::Vector3 playersStartPos[2] =
	{
		DirectX::SimpleMath::Vector3{3.0f,0.0f,6.0f} ,
		DirectX::SimpleMath::Vector3{-3.0f,0.0f,6.0f}
	};

	//�v���C���[���[�h��1�l���[�h�Ȃ��l�p�̃X�^�[�g���W�A�L�[�f�[�^�ɂ���
	if (m_playerMode == GameMain::PlayerMode::Player1)
	{
		playersStartPos[0] = DirectX::SimpleMath::Vector3(0.0f, 7.0f, 6.0f);
		playerKeyData[0] = { DirectX::Keyboard::Keys::Right,DirectX::Keyboard::Keys::Left,DirectX::Keyboard::Keys::Up,DirectX::Keyboard::Keys::Down,DirectX::Keyboard::Keys::Space };
	}
	//�v���C���[���[�h�̐��v���C���[���쐬
	for (int i = 0; i < static_cast<int>(m_playerMode); i++)
	{
		m_players.push_back(std::make_unique<Player>());

		//�X�e�[�W�}�l�[�W���[�ݒ�
		m_players[i]->SetStageManeger(m_stageManager);
		//�A�C�e���}�l�[�W���[�ݒ�
		m_players[i]->SetIteManeger(m_itemManager);
		//��Q���}�l�[�W���[�ݒ�
		m_players[i]->SetObstacleManager(m_obstacleManager);

		//�v���C���[���f���t�@�C���p�X�ݒ�
		m_players[i]->SetPlayerModelFile(playerModelFile[i]);
		//�v���C���[�̈ړ��L�[�ݒ�
		m_players[i]->SetKeys(playerKeyData[i]);
		//�v���C���[ID�ݒ�
		m_players[i]->SetTypeInt(i);

		//������
		m_players[i]->Initialize(DirectX::SimpleMath::Vector3::Zero, playersStartPos[i], DirectX::SimpleMath::Vector3(2.0f, 2.0f, 2.0f), DirectX::SimpleMath::Vector3::Zero, true, nullptr, nullptr);
	}
}



/// <summary>
/// �v���C���[�̂߂荞�ݏ���
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="cupseleToCupseVector">���������J�v�Z���ƃJ�v�Z���̃x�N�g���i�v���C���[�J�v�Z�������Q���J�v�Z���̃x�N�g���j</param>
/// <param name="playerCapsleRadius">�v���C���[�̃J�v�Z���̔��a</param>
/// <param name="obstacleCupsleRadius">�v���C���[�Q�̃J�v�Z���̔��a</param>
void PlayerManager::PlayerCapuslePenetration(Actor* player, const DirectX::SimpleMath::Vector3& cupseleToCupseVector, float playerCapsleRadius, float obstacleCupsleRadius)
{
	//���������J�v�Z���ƃJ�v�Z���̋���
	float cupseleToCupselLenth = cupseleToCupseVector.Length();
	//�v���C���[�Ə�Q���̔��a�̍��v
	float radius = playerCapsleRadius + obstacleCupsleRadius;

	DirectX::SimpleMath::Vector3 capToCapVector = cupseleToCupseVector;

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
/// �^�C�}�[���~�߂邩�m�F����
/// </summary>
void PlayerManager::TimerStopCheck()
{
	int count = 0;

	//�v���C���[�̍X�V
	for (std::unique_ptr<Player>& player : m_players)
	{
		//�v���C���[�̏��̐���-1�ł���΃J�E���g�𑝂₷
		if (player->GetShield() <= -1)
		{
			count++;
		}
		
		//�S�Ẵv���C���[�����̐���-1�ł���΃^�C�}�[���~�߂�
		if (count == m_players.size())
		{
			//�Q�[���R���e�L�X�g�Ƀv���C���[���S�����Ɛݒ�
			GameContext::GetInstance().SetIsPlayerDeath(true);

			//�^�C�}�[���~�߂�
			AliveTimer::GetInstance().SetTimerStop(true);

		}
		
	}
}

