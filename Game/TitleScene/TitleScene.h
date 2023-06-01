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


	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="start">開始の数値</param>
	/// <param name="end">終了の数値</param>
	/// <param name="t">現在の位置</param>
	/// <returns>現在の数値</returns>
	float Lerp(float start, float end, float t)
	{
		return start + (end - start) * t;
	}

	/// <summary>
	/// イージング
	/// </summary>
	float easeOutCubic(float x)
	{
		return static_cast <float>(1 - pow(1 - x, 3));
	}

private:
	/// <summary>
	/// タイトル文字の動き
	/// </summary>
	/// <param name="timer">タイマー</param>
	void TitleMove(const DX::StepTimer& timer);

	/// <summary>
	/// タイトルの状態の更新
	/// </summary>
	void TitleStateUpdate(const DX::StepTimer& timer);

	/// <summary>
	/// ステージセレクトの更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	void StageSelectUpdate(const DX::StepTimer& timer);

	/// <summary>
	/// モードセレクトの更新
	/// </summary>
	void ModeSelectUpdate();

	/// <summary>
	/// ステージ選択の画像を描画
	/// </summary>
	void DrawStageSelect();

	/// <summary>
	/// モード選択の画像を描画
	/// </summary>
	void DrawModeSelect();

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
	//タイトル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>              m_titileTexture;
	//PUSH_SPASE_KEYのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>              m_pushTexture;
	//CRIロゴテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>              m_CRIWARETexture;
	//モードセレクトテクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_modeSelectTextures;

	//PUSH_SPASE_KEYのアルファ値
	float m_alpha;
	//PUSH_SPASE_KEYのアルファ変化速度
	float m_alphaSpeed;

	//フェードインアウト
	std::unique_ptr<Fade> m_fadeInOut;

	//カメラの角度（ラジアン）
	float m_cameraRot;
	//タイトルの動きのタイム
	float m_titleTimer_s;
	//タイトルのルーチン
	int m_titleRoutine;
	//タイトルのポジション
	std::vector<DirectX::SimpleMath::Vector2> m_titlePosition;
	//タイトルの角度
	float m_titleRotetion;
	//タイトルのアルファ値
	float m_titleAlpha;
	//タイトルの状態
	TitleState m_titleSelect;

	//選択したプレイヤーモード
	GameMain::PlayerMode m_playerMode;

	//選択したステージの番号
	int m_stageNum;
	//ステージの画像
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_stageSelectTexture;
	//背景用ステージマネージャー
	std::unique_ptr<StageManager> m_stageManager;

	DirectX::SimpleMath::Vector2 m_stageTextureBasePosition;
	float m_stageTexturetime;

	DirectX::SimpleMath::Vector2 m_nextPosition;
	DirectX::SimpleMath::Vector2 m_prePosition;

	std::unique_ptr<StageSelect> m_stageSelect;
	
};