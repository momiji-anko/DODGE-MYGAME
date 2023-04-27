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
	void SetPlayerPosition(DirectX::SimpleMath::Vector3 position);



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
	/// <param name="commonState">�R�����X�e�[�g�̐��|�C���^</param>
	/// <param name="stage">�X�e�[�W�̔ԍ�</param>
	void Initialize(DirectX::CommonStates* commonState, StageManager::StageSelect stage );

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
	void ObstacleShadow( ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	
	
	
	/// <summary>
	/// �v���C���[�Ə�Q���̃J�v�Z���̓����蔻��
	/// </summary>
	/// <param name="player">�v���C���[</param>
	/// <param name="flyVelocity">�v���C���[�𐁂���΂��x���V�e�B</param>
	/// <returns>true = �������� , false = �����Ă��Ȃ�</returns>
	bool PlayerCapsuleHitCheck(Actor* player,DirectX::SimpleMath::Vector3* flyVelocity);

private:
	/// <summary>
	/// ��Q���̐���
	/// </summary>
	/// <param name="position">�o�����W</param>
	/// <param name="type">��Q�̃^�C�v</param>
	/// <param name="rot">�p�x</param>
	/// <returns>true = �������� , false = �������s</returns>
	bool CreateObstacle(const DirectX::SimpleMath::Vector3& position, Obstacle::ObstacleType type, DirectX::SimpleMath::Vector3 rot);

	/// <summary>
	/// ��Q���̃��f���쐬
	/// </summary>
	void CreateModel();
	
	/// <summary>
	/// �N�����v�֐�
	/// </summary>
	/// <param name="n">�ۂߏ������s���������l</param>
	/// <param name="min">�ۂߏ������s���ŏ��l</param>
	/// <param name="max">�ۂߏ������s���ő�l</param>
	/// <returns>�ۂߏ������s�������ʂƂȂ鐔�l</returns>
	float Clamp(float n, float min, float max);

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
	float ClosestPtSegmentSegment(DirectX::SimpleMath::Vector3 p1, DirectX::SimpleMath::Vector3 q1,
		DirectX::SimpleMath::Vector3 p2, DirectX::SimpleMath::Vector3 q2, float& s, float& t,
		DirectX::SimpleMath::Vector3& c1, DirectX::SimpleMath::Vector3& c2);



	/// <summary>
	/// �J�v�Z���ƃJ�v�Z���̏Փ˔���֐�
	/// </summary>
	/// <param name="capsule1">�����蔻�����肽���J�v�Z�� 1 ��</param>
	/// <param name="capsule2">�����蔻�����肽���J�v�Z�� 2 ��</param>
	/// <returns>true= ���������Afalse=�������ĂȂ�</returns>
	bool HitCheck_Capsule2Capsule(Capsule capsule1, Capsule capsule2);

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
	void PlayerCapuslePenetration(Actor* player, DirectX::SimpleMath::Vector3 cupseleToCupseVector, float playerCapsleRadius, float obstacleCupsleRadius);

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
	//�R�����X�e�[�g
	DirectX::CommonStates* m_commonState;
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
};