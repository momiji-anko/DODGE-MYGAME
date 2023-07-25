/*
リザルトシーン
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
/// リザルトシーン
/// </summary>
class ResultScene : public IScene
{
public:
	//ランク
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
	/// ステージ番号の設定
	/// </summary>
	/// <param name="stageNum">ステージ番号</param>
	void SetStageNum(int stageNum)override { m_stageNum = static_cast<StageManager::StageSelect>(stageNum); }
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ResultScene(GameMain* parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ResultScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	void Update(const DX::StepTimer& timer) override;

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
	/// ランクを決める
	/// </summary>
	/// <param name="aliveTime_s">アライブタイム</param>
	/// <returns>ランク</returns>
	ResultScene::RANK RankDecide(float aliveTime_s);


private: 
	/// <summary>
	/// 背景のステージを描画
	/// </summary>
	/// <param name="camera">カメラの生ポインタ</param>
	void RenderStage(Camera* camera);


	/// <summary>
	/// アライブタイムの表示
	/// </summary>
	void DrawAliveTime();

private:
	

	//カメラの回転速度
	static const float CAMERA_ROT_SPEED;
	//アルファの最大値
	static const float ALPHA_MAX_NUM;
	//アルファの最小値
	static const float ALPHA_MIN_NUM;


	//音関連
	ADX2* m_pAdx2;
	int m_musicID;


	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	//フェード
	std::unique_ptr<Fade> m_fadeInOut;

	// テクスチャ
	//数字
	ID3D11ShaderResourceView* m_numTexture;
	//PUSH_SPASE_KEY
	ID3D11ShaderResourceView* m_pushTexture;
	//ランク
	ID3D11ShaderResourceView* m_rankTexture;
	//アライブタイム文字画像
	ID3D11ShaderResourceView* m_aliveTimeTexture;
	//リザルト文字
	ID3D11ShaderResourceView* m_resultTexture;
	//ランク文字
	ID3D11ShaderResourceView* m_rankStringTexture;

	//PUSH_SPASE_KEYのアルファ値
	float m_alpha;
	//PUSH_SPASE_KEYのアルファ変化速度
	float m_alphaSpeed;

	//生きていた時間
	float m_aliveTime;
	//ランク
	RANK m_rank;

	//ステージマネージャー
	std::unique_ptr<StageManager> m_stageManeger;

	//カメラの角度
	float m_cameraRot;

	//ステージ番号
	StageManager::StageSelect m_stageNum;

	GameMain* m_parent;

};