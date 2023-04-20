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

//�^�C�g���V�[��
class TitleScene : public IScene
{
private:
	//�^�C�g���̏��
	enum class TitleState
	{
		FADEIN,
		TITLE,
		STAGESELECT,
		MODESELECT,
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
	//�^�C�g��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>              m_titileTexture;
	//PUSH_SPASE_KEY�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>              m_pushTexture;
	//�t�F�[�h�C���A�E�g�p�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>              m_blackTexture;
	//CRI���S�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>              m_CRIWARETexture;
	//���[�h�Z���N�g�e�N�X�`��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_modeSelectTextures;
	int   m_modeSelectNum;

	float m_alpha;
	float m_alphaVel;

	float m_fade;
	bool  m_flag;
	bool  m_flagFadeIn;

	std::unique_ptr<DirectX::Model> m_stageModel;

	std::unique_ptr<Fade> m_fadeInOut;

	std::vector<std::unique_ptr<Stage>> m_stageobj;
	std::vector<DirectX::SimpleMath::Vector3> m_stagePosition;
	//�J�����̊p�x�i���W�A���j
	float m_cameraRot;
	//�^�C�g���̓����̃^�C��
	float m_titleTimer_s;
	//�^�C�g���̃��[�`��
	int m_titleRoutine;
	//�^�C�g���̃|�W�V����
	std::vector<DirectX::SimpleMath::Vector2> m_titlePosition;
	float m_titleRotetion;
	float m_titleAlpha;
	RECT m_size;

	TitleState m_titleSelect;

	int m_stageSelectInt;
	GameMain::PlayerMode m_playerMode;

	int m_stageNum;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_stageSelectTexture;
	std::unique_ptr<StageManager> m_stageManager;
public:

	// �R���X�g���N�^
	TitleScene();

	// �f�X�g���N�^
	~TitleScene();

	// ������
	void Initialize() override;

	// �X�V
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

	// ���\�[�X�̓ǂݍ���
	void LoadResources() override;

	float Lerp(float start, float end, float t)
	{
		return start + (end - start) * t;
	}

	float easeOutCubic(float x) 
	{
		return 1 - pow(1 - x, 3);
	}

	GameMain::PlayerMode  GetPlayerMode() { return m_playerMode; }

	int GetStageNum() { return m_stageNum; }

};