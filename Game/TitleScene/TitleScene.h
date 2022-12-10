/*
�^�C�g���V�[��
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
#include"../PlayScene/Stage/Stage.h"

class TitleScene : public IScene
{
private:
	enum class PlayerMode
	{
		Player1 = 1,
		Player2 = 2
	};
	enum class TitleState
	{
		FADEIN,
		TITLE,
		STAGESELECT,
		FADEOUT,
	};
	ADX2* m_pAdx2;
	int m_musicID;	
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_titileTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pushTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_blackTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mCRIWARETexture;

	float m_alpha;
	float m_alphaVel;

	float m_fade;
	bool m_flag;
	bool m_flagFadeIn;

	std::unique_ptr<DirectX::Model> m_stageModel;

	std::vector< std::unique_ptr<Stage> > m_stageobj;
	std::vector<DirectX::SimpleMath::Vector3> m_stagePosition;

	float m_cameraRot;

	float m_titleTimer_s;
	int m_titleRoutine;
	std::vector<DirectX::SimpleMath::Vector2> m_titlePosition;
	float m_titleRotetion;
	float m_titleAlpha;
	RECT m_size;

	TitleState m_titleSelect;
	StageSelect m_stageSelect;

	int m_stageSelectInt;

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

	StageSelect GetStageSelect() { return m_stageSelect; }
};