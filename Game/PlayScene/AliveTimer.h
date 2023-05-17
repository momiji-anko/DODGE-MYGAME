/*
* 2023/04/26
* AliveTimer.h
* 
*/
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
	AliveTimer();
	~AliveTimer();

	// 初期化
	void Initialize();

	// 更新
	void Update(const DX::StepTimer& timer);

	// 描画
	void Draw();

	// 終了処理
	void Finalize();

	float GetTimer() { return m_timer_s; }

private:
	float m_timer_s;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_numTexture;

};