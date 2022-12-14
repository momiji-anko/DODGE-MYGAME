/*
タイトルシーン
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

class TitleScene : public IScene
{
private:

	enum class TitleState
	{
		FADEIN,
		TITLE,
		STAGESELECT,
		MODESELECT,
		FADEOUT,
	};
	ADX2* m_pAdx2;
	int m_musicID;	
	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardStateTracker;

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>						m_titileTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>						m_pushTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>						m_blackTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>						m_CRIWARETexture;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>	        m_modeSelectTextures;
	int   m_modeSelectNum;

	float m_alpha;
	float m_alphaVel;

	float m_fade;
	bool  m_flag;
	bool  m_flagFadeIn;

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

	int m_stageSelectInt;
	GameMain::PlayerMode m_playerMode;

	int m_stageNum;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_stageSelectTexture;

public:

	// コンストラクタ
	TitleScene();

	// デストラクタ
	~TitleScene();

	// 初期化
	void Initialize() override;

	// 更新
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

	// リソースの読み込み
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