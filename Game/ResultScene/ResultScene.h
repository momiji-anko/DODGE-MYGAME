/*
���U���g�V�[��
*/
#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>

#include "../IScene.h"
#include "Game/GameMain.h"


#include"Libraries/AXD2/Adx2.h"
#include"../../Helpers/CueSheet_0.h"
#include"../../Helpers/DODGESound_acf.h"

#include"../PlayScene/Stage/StageManager.h"
#include"../PlayScene/Player/Player.h"
#include"../PlayScene/Obstacle/ObstacleManager.h"
#include"../PlayScene/Item/ItemManager.h"

#include"Game/Shader/Fade.h"

/// <summary>
/// ���U���g�V�[��
/// </summary>
class ResultScene : public IScene
{
public:
	//�����N
	enum class RANK : int
	{
		D,
		C,
		B,
		A,
		S,
		NONE,
	};

	/// <summary>
	/// �X�e�[�W�ԍ��̐ݒ�
	/// </summary>
	/// <param name="stageNum">�X�e�[�W�ԍ�</param>
	void SetStageNum(int stageNum)override { m_stageNum = static_cast<StageManager::StageSelect>(stageNum); }
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ResultScene(GameMain* parent);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ResultScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	void Update(const DX::StepTimer& timer) override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// ���[�\�X�̓ǂݍ���
	/// </summary>
	void LoadResources() override;

	/// <summary>
	/// �����N�����߂�
	/// </summary>
	/// <param name="aliveTime_s">�A���C�u�^�C��</param>
	/// <returns>�����N</returns>
	ResultScene::RANK RankDecide(float aliveTime_s);


private: 
	/// <summary>
	/// �w�i�̃X�e�[�W��`��
	/// </summary>
	/// <param name="camera">�J�����̐��|�C���^</param>
	void RenderStage(Camera* camera);


	/// <summary>
	/// �A���C�u�^�C���̕\��
	/// </summary>
	void DrawAliveTime();

private:
	

	//�J�����̉�]���x
	static const float CAMERA_ROT_SPEED;
	//�A���t�@�̍ő�l
	static const float ALPHA_MAX_NUM;
	//�A���t�@�̍ŏ��l
	static const float ALPHA_MIN_NUM;


	//���֘A
	ADX2* m_pAdx2;
	int m_musicID;


	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	//�t�F�[�h
	std::unique_ptr<Fade> m_fadeInOut;

	// �e�N�X�`��
	//����
	ID3D11ShaderResourceView* m_numTexture;
	//PUSH_SPASE_KEY
	ID3D11ShaderResourceView* m_pushTexture;
	//�����N
	ID3D11ShaderResourceView* m_rankTexture;
	//�A���C�u�^�C�������摜
	ID3D11ShaderResourceView* m_aliveTimeTexture;
	//���U���g����
	ID3D11ShaderResourceView* m_resultTexture;
	//�����N����
	ID3D11ShaderResourceView* m_rankStringTexture;

	//PUSH_SPASE_KEY�̃A���t�@�l
	float m_alpha;
	//PUSH_SPASE_KEY�̃A���t�@�ω����x
	float m_alphaSpeed;

	//�����Ă�������
	float m_aliveTime;
	//�����N
	RANK m_rank;

	//�X�e�[�W�}�l�[�W���[
	std::unique_ptr<StageManager> m_stageManeger;

	//�J�����̊p�x
	float m_cameraRot;

	//�X�e�[�W�ԍ�
	StageManager::StageSelect m_stageNum;

	GameMain* m_parent;

};