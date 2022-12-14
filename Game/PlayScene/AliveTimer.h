#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>
#include"../../Libraries/MyLibraries/Camera.h"
#include"StepTimer.h"
#include"Libraries/MyLibraries/Singleton.h"
#include<vector>

class AliveTimer : public Singleton< AliveTimer>
{

public:
	enum class TimeType
	{
		COUNTDOWN,
		NORMAL,
	};
private:
	float m_timer_s;
	//プレイヤーのタイム
	std::vector<float> m_playersAliveTime_s;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_numTexture;

	DirectX::CommonStates* m_commonState;

public:
	AliveTimer();
	~AliveTimer();

	// 初期化
	void Initialize(DirectX::CommonStates* commonState, int playerNum);

	// 更新
	void Update(const DX::StepTimer& timer);

	// 描画
	void Draw();

	// 終了処理
	void Finalize();

	float GetTimer() { return m_timer_s; }

	std::vector<float> GetPlayerAliveTime() { return m_playersAliveTime_s; }

};