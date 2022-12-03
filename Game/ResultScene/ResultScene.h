/*
タイトルシーン
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
#include"../PlayScene/Obstacle/ObstacleManeger.h"
#include"../PlayScene/Item/ItemManeger.h"

class ResultScene : public IScene
{
private:
	enum class RANK : int
	{
		D,
		C,
		B,
		A,
		S,
		NONE,
	};


	enum class ResultState
	{
		TIME,
		RANK,
		SCENET,
	};

	ADX2* m_pAdx2;
	int m_musicID;

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_numTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_blackTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pushTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_rankTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_rankBackTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_aliveTimeTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_resultTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_rankStringTexture;
	
	float m_fade;
	bool m_flag;
	bool m_flagFadeIn;

	float m_alpha;
	float m_alphaVel;


	float m_time;
	float m_resultTime_s;

	RANK m_rank;

	int m_resultRotetion;

	DirectX::SimpleMath::Vector2 m_rankPos;

	std::unique_ptr<StageManeger> m_stageManeger;
	std::unique_ptr<Player> m_player;
	std::unique_ptr<ObstacleManeger> m_obstacleManeger;
	std::unique_ptr<ItemManeger> m_itemManeger;

	float m_cameraRot;

	ResultState m_scene;
public:

	// コンストラクタ
	ResultScene();

	// デストラクタ
	~ResultScene();

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

	void SetTime(float time) { m_time = time; }

	void SetStageManeger(std::unique_ptr<StageManeger>& stage) { m_stageManeger = std::move(stage);}
	void SetPlayer(std::unique_ptr<Player>& player) { m_player = std::move(player); }
	void SetObstacleManeger(std::unique_ptr<ObstacleManeger>& obstacleManeger) { m_obstacleManeger = std::move(obstacleManeger); }

	void SetItemManeger(std::unique_ptr<ItemManeger>& itemManeger) { m_itemManeger = std::move(itemManeger); }
private: 
	void RenderPlayStage(Camera* camera);
};