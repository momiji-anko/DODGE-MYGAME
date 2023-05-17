/*
�v���C�V�[��
*/
#pragma once
#include<Keyboard.h>
#include <CommonStates.h>
#include <Model.h>
#include <GeometricPrimitive.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

#include "../IScene.h"
#include "Game/GameMain.h"
#include"Player/Player.h"
#include"Player/PlayerManager.h"
#include"Actor/Actor.h"
#include"Obstacle/ObstacleManager.h"
#include"Stage/Stage.h"
#include"Item/ItemManager.h"
#include"ShadowMap.h"
#include"Stage/StageManager.h"
#include"AliveTimer.h"

#include"Libraries/AXD2/Adx2.h"
#include"../../Helpers/CueSheet_0.h"
#include"../../Helpers/DODGESound_acf.h"
#include"Objects.h"
#include"Game/Shader/Fade.h"

class GridFloor;
class Camera;

/// <summary>
/// �v���C�V�[��
/// </summary>
class PlayScene : public IScene
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PlayScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PlayScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <returns>���̃V�[��</returns>
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// ���\�[�X�ǂݍ���
	/// </summary>
	void LoadResources() override;


	/// <summary>
	/// �v���C���[�̃��[�h�i��l����l���j
	/// </summary>
	/// <param name="mode">Player1=��l�APlayer2=��l�A</param>
	void SetPlayerMode(GameMain::PlayerMode mode) { m_playerMode = mode; }

	/// <summary>
	/// �X�e�[�W�ԍ�
	/// </summary>
	/// <param name="stageNum">�X�e�[�W�ԍ�</param>
	void SetStageNum(int stageNum)override { m_stageNum = static_cast<StageManager::StageSelect>(stageNum); }

private:

	/// <summary>
	/// �v���C���[�̍쐬
	/// </summary>
	void CreatePlayer();

	/// <summary>
	/// �S�ăv���C���[������ł��邩�m�F
	/// </summary>
	/// <returns>true=�S�Ď���ł���Afalse=�S�Ď���ł��Ȃ�</returns>
	bool AllPlayerIsDead();

	/// <summary>
	/// �J�E���g�_�E���̕\��
	/// </summary>
	void DrawCountDown();

	/// <summary>
	/// �e����
	/// </summary>
	void CreateShadow();

	/// <summary>
/// �N�����v�֐�
/// </summary>
/// <param name="n">�ۂߏ������s���������l</param>
/// <param name="min">�ۂߏ������s���ŏ��l</param>
/// <param name="max">�ۂߏ������s���ő�l</param>
/// <returns>�ۂߏ������s�������ʂƂȂ鐔�l</returns>
	float Clamp(float n, float min, float max)
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
	float ClosestPtSegmentSegment(const DirectX::SimpleMath::Vector3& p1, const DirectX::SimpleMath::Vector3& q1, const DirectX::SimpleMath::Vector3& p2, const DirectX::SimpleMath::Vector3& q2, float& s, float& t, DirectX::SimpleMath::Vector3& c1, DirectX::SimpleMath::Vector3& c2)
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

		// ��ԋ߂���_�̋����̓���Ԃ�
		return (c1 - c2).Dot(c1 - c2);
	}

	/// <summary>
	/// �J�v�Z���ƃJ�v�Z���̏Փ˔���֐�
	/// </summary>
	/// <param name="capsule1">�����蔻�����肽���J�v�Z�� 1 ��</param>
	/// <param name="capsule2">�����蔻�����肽���J�v�Z�� 2 ��</param>
	/// <returns>true= ���������Afalse=�������ĂȂ�</returns>
	bool HitCheck_Capsule2Capsule(Capsule capsule1, Capsule capsule2)
	{
		float s, t;
		DirectX::SimpleMath::Vector3 c1, c2;
		// �J�v�Z���̒��S�̐����Ԃ̋����̕������v�Z
		// s, t, c1, c2 �͖߂�l
		float dist2 = ClosestPtSegmentSegment(capsule1.a, capsule1.b, capsule2.a, capsule2.b, s, t, c1, c2);
		float radius = capsule1.r + capsule2.r;
		if (dist2 <= radius * radius)
		{
			PlayerCapuslePenetration(m_players[0].get(), c2 - c1, capsule1.r, capsule2.r);
			PlayerCapuslePenetration(m_players[1].get(), c1 - c2, capsule1.r, capsule1.r);
		}

		return dist2 <= radius * radius;
	}

	void PlayerCapuslePenetration(Actor* player, const DirectX::SimpleMath::Vector3& cupseleToCupseVector, float playerCapsleRadius, float obstacleCupsleRadius)
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

private:
	//�J�E���g�_�E���^�C��
	static const float COUNT_DOWN_TIME_S;

	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	//�V��
	DirectX::Model* m_pModel;

	//��Q���}�l�[�W���[
	std::unique_ptr<ObstacleManager>                 m_obstacleManager;
	//�A�C�e���}�l�[�W���[
	std::unique_ptr<ItemManager>                     m_itemManager;
	//�X�e�[�W�}�l�[�W���[
	std::unique_ptr<StageManager>                    m_stageManager;

	//�J����
	std::unique_ptr<Camera> m_pCamera;

	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch>            m_spriteBatch;
	//�J�E���g�_�E���e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_countDownTexture;
	//�J�E���g�_�E��
	float                                            m_countDownTime;

	//�L�[�{�[�h�X�e�[�g�g���b�J�[
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardStateTracker;

	//�V���h�E�}�b�v
	std::unique_ptr<ShadowMap>                       m_shadowMap;


	//���֘A
	ADX2* m_pAdx2;
	//BGM��ID
	int  m_musicID;

	//�t�F�[�h
	std::unique_ptr<Fade> m_fadeInOut;

	//�A���C�u�^�C��
	AliveTimer* m_aliveTime;
	//2P�p�ɔz��Ńv���C���[���`
	std::vector<std::unique_ptr<Player>> m_players;
	//�v���C���[�}�l�[�W���[
	std::unique_ptr<PlayerManager> m_playerManager;
	//�v���C���[���[�h
	GameMain::PlayerMode m_playerMode;
	//�X�e�[�W
	StageManager::StageSelect m_stageNum;

	//�v���C���[�̑�����@�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_playerMoveKey;
	bool m_isTabKey;

};