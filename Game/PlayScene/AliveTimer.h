/*
* 2023/04/26
* AliveTimer.h
* アライブタイム
* 麻生　楓
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

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AliveTimer();
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~AliveTimer();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer"></param>
	void Update(const DX::StepTimer& timer);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// タイマーの取得
	/// </summary>
	/// <returns>タイマー</returns>
	float GetTimer() { return m_timer_s; }


	void SetTimerStop(bool isStop) { m_isTimerStop = isStop; }

private:
	//タイマー
	float m_timer_s;

	// スプライトバッチ
	DirectX::SpriteBatch* m_spriteBatch;

	//数字テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_numTexture;

	bool m_isTimerStop;

};