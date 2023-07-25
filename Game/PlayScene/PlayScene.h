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
class GameMain;

/// <summary>
/// �v���C�V�[��
/// </summary>
class PlayScene : public IScene
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PlayScene(GameMain* parent);

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
	/// ���\�[�X�ǂݍ���
	/// </summary>
	void LoadResources() override;


	/// <summary>
	/// �X�e�[�W�ԍ�
	/// </summary>
	/// <param name="stageNum">�X�e�[�W�ԍ�</param>
	void SetStageNum(int stageNum)override { m_stageNum = static_cast<StageManager::StageSelect>(stageNum); }

private:

	/// <summary>
	/// �J�E���g�_�E���̕\��
	/// </summary>
	void DrawCountDown();

	/// <summary>
	/// �e����
	/// </summary>
	void CreateShadow();
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
	std::unique_ptr<Camera>                          m_pCamera;

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
	//�v���C���[�}�l�[�W���[
	std::unique_ptr<PlayerManager> m_playerManager;
	//�v���C���[���[�h
	GameMain::PlayerMode m_playerMode;
	//�X�e�[�W
	StageManager::StageSelect m_stageNum;

	//�v���C���[�̑�����@�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_playerMoveKey;
	bool m_isTabKey;

	GameMain* m_parent;
};