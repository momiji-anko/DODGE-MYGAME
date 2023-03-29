/*
* 2023/03/23
* Blink.h
* 麻生　楓
*/
#pragma once
#include"StepTimer.h"

/// <summary>
/// 点滅
/// </summary>
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

	//初期ブリンクタイム
	float m_startBlinkTime_s;
	//初期ブリンクスピード
	float m_startBlinkSpeed;

	int m_blinkCount;

	int m_startBlinkCount;

	int m_blinkedCount;
	float m_blinkTimeFast;
	bool m_isDraw;
public:
	/// <summary>
	/// 点滅するか設定
	/// </summary>
	/// <param name="flag">true = 点滅する　false = 点滅しない</param>
	void SetBlinkFlag(bool flag) { m_isBlink = flag; }
	
	/// <summary>
	/// 点滅状態取得
	/// </summary>
	/// <returns>true = 消えていない　false = 消えている</returns>
	bool IsBlink() { return m_isDraw; }

	/// <summary>
	/// 点滅開始
	/// </summary>
	void Start() { m_isBlink = true; }

	/// <summary>
	/// 点滅を止める
	/// </summary>
	void End() { m_isBlink = false; }

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
	/// <param name="blinkTime">点滅切り替え時間（秒指定）</param>
	/// <param name="blinkCount">点滅する回数</param>
	/// <param name="isGetiingFaster">点滅をだんだん早くするか</param>
	/// <param name="blinkSpeed">点滅する速さ</param>
	void Initialize(float blinkTime, int blinkCount = 10,  bool isGetiingFaster = false , float blinkSpeed = 0.5f);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	void Update(const DX::StepTimer& timer);

	/// <summary>
	/// リセット
	/// </summary>
	void ReSet();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

private:
	/// <summary>
	/// 点滅のついた瞬間の処理
	/// </summary>
	void BlinkedCheck();
};
