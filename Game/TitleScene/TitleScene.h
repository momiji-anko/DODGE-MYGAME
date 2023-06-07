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
#include"StageSelect.h"
#include"ModeSelect.h"
#include"TitleText.h"

class TitleScene : public IScene
{
public:

	/// <summary>
	/// プレイヤーモード取得
	/// </summary>
	/// <returns>選択したプレイヤーモード</returns>
	GameMain::PlayerMode  GetPlayerMode()const { return m_playerMode; }

	/// <summary>
	/// ステージの番号取得
	/// </summary>
	/// <returns>選択したステージの番号</returns>
	int GetStageNum() const{ return m_stageNum; }

	/// <summary>
	/// ステージ番号の設定
	/// </summary>
	/// <param name="stageNum">ステージ番号</param>
	void SetStageNum(int stageNum)override { UNREFERENCED_PARAMETER(stageNum); }

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	~TitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <returns>次のシーン</returns>
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// リーソスの読み込み
	/// </summary>
	void LoadResources() override;


private:

	/// <summary>
	/// タイトルの状態の更新
	/// </summary>
	void TitleStateUpdate(const DX::StepTimer& timer);

	/// <summary>
	/// タイトルの状態の描画
	/// </summary>
	void TitleStateDraw();

private:
	//タイトルの移動時間
	static const float MOVE_TIME;
	//タイトルの移動角度
	static const float MOVE_RORATION;
	//タイトルの初期角度
	static const float RORATION_ZERO;
	//タイトルの回転ルーチンのリセット
	static const int ROTATION_RESET_ROUTINE;
	//カメラの回転速度
	static const float CAMERA_ROT_SPEED;
	//アルファの最大値
	static const float ALPHA_MAX_NUM;
	//アルファの最小値
	static const float ALPHA_MIN_NUM;

	//タイトルの状態
	enum class TitleState
	{
		//フェードイン
		FADEIN,
		//タイトル
		TITLE,
		//ステージセレクト
		STAGESELECT,
		//モードセレクト
		MODESELECT,
		//フェードアウト
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
	//PUSH_SPASE_KEYのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pushTexture;
	//CRIロゴテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_CRIWARETexture;

	//PUSH_SPASE_KEYのアルファ値
	float m_alpha;
	//PUSH_SPASE_KEYのアルファ変化速度
	float m_alphaSpeed;

	//フェードインアウト
	std::unique_ptr<Fade> m_fadeInOut;

	//カメラの角度（ラジアン）
	float m_cameraRot;

	//タイトルの状態
	TitleState m_titleSelect;

	//選択したプレイヤーモード
	GameMain::PlayerMode m_playerMode;

	//選択したステージの番号
	int m_stageNum;
	//背景用ステージマネージャー
	std::unique_ptr<StageManager> m_stageManager;

	//ステージセレクト
	std::unique_ptr<StageSelect> m_stageSelect;
	//モードセレクト
	std::unique_ptr<ModeSelect> m_modeSelect;
	//タイトル文字
	std::unique_ptr<TitleText> m_titleText;
	
};