/*
* 2023/03/23
* Blink.h
* 麻生　楓
*/
#pragma once
#include"StepTimer.h"

class Blink
{
private:
	//ブリンクタイム
	float m_blinkTime_s;
	//点滅する速さ
	float m_blinkSpeed;

	//点滅速度をだんだん早くするか
	bool m_isGettingFaster;

	//点滅するかどうか
	bool m_isBlink;

public:
	/// <summary>
	/// 点滅するか設定
	/// </summary>
	/// <param name="flag">true = 点滅する　false = 点滅しない</param>
	void SetBlinkFlag(bool flag) { m_isBlink = flag; }
	
	/// <summary>
	/// 点滅状態取得
	/// </summary>
	/// <returns>true = 点滅している　false = 点滅していない</returns>
	bool IsBlink() { return m_isBlink; }

	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Blink();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Blink();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="blinkTime">点滅する時間（秒指定）</param>
	/// <param name="blinkSpeed">点滅する速さ</param>
	/// <param name="isGetiingFaster">点滅をだんだん早くするか</param>
	void Initialize(float blinkTime ,float blinkSpeed = 0.5f ,bool isGetiingFaster = false);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer"></param>
	void Update(const DX::StepTimer& timer);

	/// <summary>
	/// リセット
	/// </summary>
	void ReSet();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();


};
