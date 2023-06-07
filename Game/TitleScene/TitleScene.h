/*
�^�C�g���V�[��
*/
#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>
#include<Keyboard.h>
#include "../IScene.h"
#include "Game/GameMain.h"

#include"Libraries/AXD2/Adx2.h"
#include"../../Helpers/CueSheet_0.h"
#include"../../Helpers/DODGESound_acf.h"
#include"../PlayScene/Stage/Stage.h"
#include"Game/Shader/Fade.h"
#include"Game/PlayScene/Stage/StageManager.h"
#include"StageSelect.h"
#include"ModeSelect.h"
#include"TitleText.h"

class TitleScene : public IScene
{
public:

	/// <summary>
	/// �v���C���[���[�h�擾
	/// </summary>
	/// <returns>�I�������v���C���[���[�h</returns>
	GameMain::PlayerMode  GetPlayerMode()const { return m_playerMode; }

	/// <summary>
	/// �X�e�[�W�̔ԍ��擾
	/// </summary>
	/// <returns>�I�������X�e�[�W�̔ԍ�</returns>
	int GetStageNum() const{ return m_stageNum; }

	/// <summary>
	/// �X�e�[�W�ԍ��̐ݒ�
	/// </summary>
	/// <param name="stageNum">�X�e�[�W�ԍ�</param>
	void SetStageNum(int stageNum)override { UNREFERENCED_PARAMETER(stageNum); }

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	TitleScene();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	~TitleScene();

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
	/// ���[�\�X�̓ǂݍ���
	/// </summary>
	void LoadResources() override;


private:

	/// <summary>
	/// �^�C�g���̏�Ԃ̍X�V
	/// </summary>
	void TitleStateUpdate(const DX::StepTimer& timer);

	/// <summary>
	/// �^�C�g���̏�Ԃ̕`��
	/// </summary>
	void TitleStateDraw();

private:
	//�^�C�g���̈ړ�����
	static const float MOVE_TIME;
	//�^�C�g���̈ړ��p�x
	static const float MOVE_RORATION;
	//�^�C�g���̏����p�x
	static const float RORATION_ZERO;
	//�^�C�g���̉�]���[�`���̃��Z�b�g
	static const int ROTATION_RESET_ROUTINE;
	//�J�����̉�]���x
	static const float CAMERA_ROT_SPEED;
	//�A���t�@�̍ő�l
	static const float ALPHA_MAX_NUM;
	//�A���t�@�̍ŏ��l
	static const float ALPHA_MIN_NUM;

	//�^�C�g���̏��
	enum class TitleState
	{
		//�t�F�[�h�C��
		FADEIN,
		//�^�C�g��
		TITLE,
		//�X�e�[�W�Z���N�g
		STAGESELECT,
		//���[�h�Z���N�g
		MODESELECT,
		//�t�F�[�h�A�E�g
		FADEOUT,
	};

	//���֘A
	ADX2* m_pAdx2;
	int m_musicID;

	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	//�L�[�{�[�h�X�e�[�g�g���b�J�[
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardStateTracker;

	// �e�N�X�`��
	//PUSH_SPASE_KEY�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pushTexture;
	//CRI���S�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_CRIWARETexture;

	//PUSH_SPASE_KEY�̃A���t�@�l
	float m_alpha;
	//PUSH_SPASE_KEY�̃A���t�@�ω����x
	float m_alphaSpeed;

	//�t�F�[�h�C���A�E�g
	std::unique_ptr<Fade> m_fadeInOut;

	//�J�����̊p�x�i���W�A���j
	float m_cameraRot;

	//�^�C�g���̏��
	TitleState m_titleSelect;

	//�I�������v���C���[���[�h
	GameMain::PlayerMode m_playerMode;

	//�I�������X�e�[�W�̔ԍ�
	int m_stageNum;
	//�w�i�p�X�e�[�W�}�l�[�W���[
	std::unique_ptr<StageManager> m_stageManager;

	//�X�e�[�W�Z���N�g
	std::unique_ptr<StageSelect> m_stageSelect;
	//���[�h�Z���N�g
	std::unique_ptr<ModeSelect> m_modeSelect;
	//�^�C�g������
	std::unique_ptr<TitleText> m_titleText;
	
};