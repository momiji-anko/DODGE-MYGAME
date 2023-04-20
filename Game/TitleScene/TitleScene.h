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
#include"Game/Shader/Fade.h"
#include"Game/PlayScene/Stage/StageManager.h"

//タイトルシーン
class TitleScene : public IScene
{
private:
	//タイトルの状態
	enum class TitleState
	{
		FADEIN,
		TITLE,
		STAGESELECT,
		MODESELECT,
		FADEOUT,
	};
	//音関連
	ADX2* m_pAdx2;
	int m_musicID;	

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	//キーボードステートトラッカー
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardStateTracker;

	// テクスチャ
	//タイトル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>              m_titileTexture;
	//PUSH_SPASE_KEYのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>              m_pushTexture;
	//フェードインアウト用テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>              m_blackTexture;
	//CRIロゴテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>              m_CRIWARETexture;
	//モードセレクトテクスチャ
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
	//カメラの角度（ラジアン）
	float m_cameraRot;
	//タイトルの動きのタイム
	float m_titleTimer_s;
	//タイトルのルーチン
	int m_titleRoutine;
	//タイトルのポジション
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