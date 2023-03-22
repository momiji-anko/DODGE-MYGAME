#pragma once

#include<map>
#include<vector>
#include"Obstacle.h"
#include<Model.h>
#include"../ShadowMap.h"

#include"../../Shader/FireEffectManager.h"
#include"Game/PlayScene/Stage/StageManager.h"

//�r�w�C�r�A�[�C���N���[�h
#include"ObstacelBhaviors/BirdBehavior.h"
#include"ObstacelBhaviors/MeanderingeBehavior.h"
#include"ObstacelBhaviors/MeteoriteBehavior.h"
#include"ObstacelBhaviors/NormalBehavior.h"
#include"ObstacelBhaviors/ReverseRotateStickBehavior.h"
#include"ObstacelBhaviors/RotateStickBehavior.h"
#include"ObstacelBhaviors/StickBehavior.h"

//�X�|�i�[�C���N���[�h
#include"ObstacleSpawners/BirdObstacleSpawner.h"
#include"ObstacleSpawners/MeanderingObstacleSpawner.h"
#include"ObstacleSpawners/MeteoriteObstacleSpawner.h"
#include"ObstacleSpawners/NormalObstacleSpawner.h"
#include"ObstacleSpawners/ReverseRotateStickObstacleSpawner.h"
#include"ObstacleSpawners/RotateStickObstacleSpawner.h"
#include"ObstacleSpawners/StickObstacleSpawner.h"


class Player;

class ObstacleManeger
{
public:
	static const int OBSTACLE_MAX_NUM;
	static const float SPANE_COOL_TIME_S;
	static const int EFFECT_MAX_NUM;
private:
	std::vector< std::unique_ptr<Actor>>                               m_obstacles;
	std::map< Obstacle::ObstacleType, std::unique_ptr<ISpawner>>       m_spawners;
	std::map< Obstacle::ObstacleType, std::unique_ptr<DirectX::Model>> m_models;
	std::map< Obstacle::ObstacleType, std::unique_ptr<IBehavior>>      m_behavior;
	DirectX::CommonStates*                                             m_commonState;
	
	std::map< int, DirectX::SimpleMath::Vector3>                       m_normalSpawnePosition;
	std::map< int, DirectX::SimpleMath::Vector3>                       m_stickSpawnePosition;
	std::map< int, DirectX::SimpleMath::Vector3>                       m_birdSpawnPosition;

	DirectX::SimpleMath::Vector3                                       m_playerPosition;

	std::vector<std::unique_ptr<FireEffectManager>>                       m_effectlist;

	DirectX::SimpleMath::Vector3                                       m_hitvel;
	DirectX::SimpleMath::Vector3                                       m_capsuleHitC1;
	DirectX::SimpleMath::Vector3                                       m_capsuleHitC2;

	float                                                              m_time_s;
	float                                                              m_spawneTime_s;
	float                                                              m_spawneCoolTime;


public:
	//�R���X�g���N�^
	ObstacleManeger();
	//�f�X�g���N�^
	~ObstacleManeger();

	// ������
	void Initialize(DirectX::CommonStates* commonState, StageManeger::StageSelect stage );

	// �X�V
	void Update(const DX::StepTimer& timer);

	// �`��
	void Draw(Camera* camera);

	// �I������
	void Finalize();

	void CreateSpawner();
	void CreateBehavior();

	void SetPlayerPosition(DirectX::SimpleMath::Vector3 position) { m_playerPosition = position; }

	bool PlayerHitCheck(AABBFor3D* playerAABB);

	void ObstacleShadow( ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	bool PlayerCapsuleHitCheck(Capsule* playerCapsule);
	bool PlayerCapsuleHitCheck(Player* player);

private:
	//�A�C�e���̐���
	bool CreateObstacle(const DirectX::SimpleMath::Vector3& position, Obstacle::ObstacleType type, float angle);

	// �N�����v�֐�
// n�F�ۂߏ������s���������l
// min�F�ۂߏ������s���ŏ��l
// max�F�ۂߏ������s���ő�l
// �߂�l
// �ۂߏ������s�������ʂƂȂ鐔�l
	float Clamp(float n, float min, float max)
	{
		return std::min(std::max(min, n), max);
	}


	// �Q�̐����̍ŒZ�����̕�����Ԃ��֐�
	// p1: ���� 1 �̎n�_ q1: ���� 1 �̏I�_
	// p2: ���� 2 �̎n�_ q2: ���� 2 �̏I�_
	// s: �����P��̍ŒZ�ʒu��\���W�� t: ���� 2 ��̍ŒZ�ʒu��\���W��
	// c1: �����P��̍ŒZ�����̈ʒu c2: ���� 2 ��̍ŒZ�����̈ʒu
	// �Ԃ�l: �Q�̐����̍ŒZ�����̕���
	float ClosestPtSegmentSegment(DirectX::SimpleMath::Vector3 p1, DirectX::SimpleMath::Vector3 q1,
		DirectX::SimpleMath::Vector3 p2, DirectX::SimpleMath::Vector3 q2, float& s, float& t,
		DirectX::SimpleMath::Vector3& c1, DirectX::SimpleMath::Vector3& c2)
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


	// �J�v�Z���ƃJ�v�Z���̏Փ˔���֐�
	// capsule1�F�����蔻�����肽���J�v�Z�� 1 ��
	// capsule2�F�����蔻�����肽���J�v�Z�� 2 ��
	bool HitCheck_Capsule2Capsule(Capsule capsule1, Capsule capsule2)
	{
		float s, t;
		DirectX::SimpleMath::Vector3 c1, c2;
		// �J�v�Z���̒��S�̐����Ԃ̋����̕������v�Z
		// s, t, c1, c2 �͖߂�l
		float dist2 = ClosestPtSegmentSegment(capsule1.a, capsule1.b, capsule2.a, capsule2.b, s, t, c1, c2);
		float radius = capsule1.r + capsule2.r;
		return dist2 <= radius * radius;
	}



};