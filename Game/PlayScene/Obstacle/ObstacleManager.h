/*
* 2023/04/11
* ObstacleManeger.h
* ��Q���Ǘ��N���X
* �����@��
*/
#pragma once

#include<map>
#include<vector>
#include"Obstacle.h"
#include<Model.h>
#include"../ShadowMap.h"

#include"../../Shader/FireEffectManager.h"
#include"Game/PlayScene/Stage/StageManager.h"

//�r�w�C�r�A�[�C���N���[�h
#include"ObstacelBehaviors/BirdBehavior.h"
#include"ObstacelBehaviors/MeanderingeBehavior.h"
#include"ObstacelBehaviors/MeteoriteBehavior.h"
#include"ObstacelBehaviors/NormalBehavior.h"
#include"ObstacelBehaviors/ReverseRotateStickBehavior.h"
#include"ObstacelBehaviors/RotateStickBehavior.h"
#include"ObstacelBehaviors/StickBehavior.h"

//�X�|�i�[�C���N���[�h
#include"ObstacleSpawners/BirdObstacleSpawner.h"
#include"ObstacleSpawners/MeanderingObstacleSpawner.h"
#include"ObstacleSpawners/MeteoriteObstacleSpawner.h"
#include"ObstacleSpawners/NormalObstacleSpawner.h"
#include"ObstacleSpawners/ReverseRotateStickObstacleSpawner.h"
#include"ObstacleSpawners/RotateStickObstacleSpawner.h"
#include"ObstacleSpawners/StickObstacleSpawner.h"

//�O���錾
class Player;

/// <summary>
/// ��Q���Ǘ��N���X
/// </summary>
class ObstacleManager
{
public:
	/// <summary>
	/// �v���C���[�̍��W�ݒ�
	/// </summary>
	/// <param name="position">�v���C���[�̍��W</param>
	void SetPlayerPosition(const DirectX::SimpleMath::Vector3& position);

	/// <summary>
	/// �v���C���[�Ɠ���������Q���̃^�C�v
	/// </summary>
	/// <returns>����������Q���̃^�C�v</returns>
	Obstacle::ObstacleType GetHitType() { return m_hitType; }

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ObstacleManager();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ObstacleManager();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="stage">�X�e�[�W�̔ԍ�</param>
	void Initialize(StageManager::StageSelect stage );

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	void Update(const DX::StepTimer& timer);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="camera">�J�����̐��|�C���^</param>
	void Draw(Camera* camera);

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	/// <summary>
	/// �X�|�i�[�쐬
	/// </summary>
	void CreateSpawner();
	
	/// <summary>
	/// �r�w�C�r�A�[�쐬
	/// </summary>
	void CreateBehavior();
	
	/// <summary>
	/// �v���C���[�Ə�Q����AABB�̓����蔻��
	/// </summary>
	/// <param name="playerAABB">�v���C���[��AABB</param>
	/// <returns>true = �������� , false = �����Ă��Ȃ�</returns>
	bool PlayerHitCheck(AABBFor3D* playerAABB);

	/// <summary>
	/// ��Q���̉e�쐬
	/// </summary>
	/// <param name="shadowMap">�V���h�E�}�b�v�̐��|�C���^</param>
	/// <param name="view">�r���[�s��</param>
	/// <param name="projection">�v���W�F�N�V�����s��</param>
	void ObstacleShadow( ShadowMap* shadowMap, const DirectX::SimpleMath::Matrix& view,const DirectX::SimpleMath::Matrix& projection);

	
	
	
	/// <summary>
	/// �v���C���[�Ə�Q���̃J�v�Z���̓����蔻��
	/// </summary>
	/// <param name="player">�v���C���[</param>
	/// <param name="flyVelocity">�v���C���[�𐁂���΂��x���V�e�B</param>
	/// <returns>true = �������� , false = �����Ă��Ȃ�</returns>
	bool PlayerCapsuleHitCheck(Actor* player, DirectX::SimpleMath::Vector3* flyVelocity);

private:
	/// <summary>
	/// ��Q���̐���
	/// </summary>
	/// <param name="position">�o�����W</param>
	/// <param name="type">��Q�̃^�C�v</param>
	/// <param name="rot">�p�x</param>
	/// <returns>true = �������� , false = �������s</returns>
	bool CreateObstacle(const DirectX::SimpleMath::Vector3& position, Obstacle::ObstacleType type, const DirectX::SimpleMath::Vector3& rot);

	/// <summary>
	/// ��Q���̃��f���쐬
	/// </summary>
	void CreateModel();
	


	/// <summary>
	/// �v���C���[�𐁂���΂�	
	/// </summary>
	/// <param name="rotSpeed">��Q���̉�]���x</param>
	/// <param name="player">�v���C���[</param>
	DirectX::SimpleMath::Vector3 FlyPlayer(float rotSpeed);
	
	/// <summary>
	/// �v���C���[�̂߂荞�ݏ���
	/// </summary>
	/// <param name="player">�v���C���[</param>
	/// <param name="cupseleToCupseVector">���������J�v�Z���ƃJ�v�Z���̃x�N�g���i�v���C���[�J�v�Z�������Q���J�v�Z���̃x�N�g���j</param>
	/// <param name="playerCapsleRadius">�v���C���[�̃J�v�Z���̔��a</param>
	/// <param name="obstacleCupsleRadius">��Q���̃J�v�Z���̔��a</param>
	void PlayerCapuslePenetration(Actor* player,const DirectX::SimpleMath::Vector3& cupseleToCupseVector, float playerCapsleRadius, float obstacleCupsleRadius);

	/// <summary>
	/// ��]����_�ɓ����������̏���
	/// </summary>
	/// <param name="obstacle">��]����_�̏�Q��</param>
	/// <param name="player">�v���C���[</param>
	DirectX::SimpleMath::Vector3 RotaStickHit(Actor* obstacle, Actor* player);

	/// <summary>
	/// ���Ή�]����_�ɓ����������̏���
	/// </summary>
	/// <param name="obstacle">��]����_�̏�Q��</param>
	/// <param name="player">�v���C���[</param>	
	DirectX::SimpleMath::Vector3 ReverseRotaStickHit(Actor* obstacle, Actor* player);

public:
	//��Q���ő吔
	static const int OBSTACLE_MAX_NUM;
	//�X�|�[���N�[���^�C��
	static const float SPAWN_COOL_TIME_S;
	//���G�t�F�N�g�̍ő吔
	static const int EFFECT_MAX_NUM;
	//�X�|�[���N�[���^�C���̌��鐔
	static const float SPAWNE_COOL_TIME_DECREASE_AMOUNT;

private:
	//��Q��
	std::vector< std::unique_ptr<Actor>>                         m_obstacles;
	//��Q���̃X�|�i�[
	std::map< Obstacle::ObstacleType, std::unique_ptr<ISpawner>> m_spawners;
	//��Q���̃��f��
	std::map< Obstacle::ObstacleType, DirectX::Model*>           m_models;
	//��Q���̃r�w�C�r�A�[
	std::map< Obstacle::ObstacleType, std::unique_ptr<IBehavior>>m_behavior;
	//���̏�Q���̏o���ʒu
	std::vector<DirectX::SimpleMath::Vector3>                    m_normalSpawnePosition;
	//�_�̏�Q���̏o���ʒu
	std::vector<DirectX::SimpleMath::Vector3>                    m_stickSpawnePosition;
	//���̏�Q���̏o���ʒu
	std::vector<DirectX::SimpleMath::Vector3>                    m_birdSpawnPosition;
	//�v���C���[�̃|�W�V����
	DirectX::SimpleMath::Vector3                                 m_playerPosition;

	//���G�t�F�N�g�̔z��
	std::vector<std::unique_ptr<FireEffectManager>>              m_effectlist;

	//�v���C���[����]����_�ɓ����������̃v���C���[�����]����_�̋���
	DirectX::SimpleMath::Vector3                                 m_hitvel;
	//���������J�v�Z���̏ꏊ�i��Q���j
	DirectX::SimpleMath::Vector3                                 m_capsuleHitC1;
	//���������J�v�Z���̏ꏊ�i�v���C���[�j
	DirectX::SimpleMath::Vector3                                 m_capsuleHitC2;

	//�^�C�}�[
	float                                                        m_time_s;
	//��Q���̃X�|�[���^�C�}�[
	float                                                        m_spawneTime_s;
	//��Q���̃X�|�[���N�[���^�C��
	float                                                        m_spawneCoolTime_s;

	//����������Q���̃^�C�v
	Obstacle::ObstacleType m_hitType;

	
};