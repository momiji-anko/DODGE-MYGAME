/*
* 2023/03/23
* Blink.cpp
* 麻生　楓
* 点滅する
*/
#include"pch.h"
#include"Blink.h"
#include"DeviceResources.h"


/// <summary>
/// コンストラクタ
/// </summary>
Blink::Blink()
	:
	m_blinkTime_s(0),
	m_blinkSpeed(0),
	m_isBlink(false),
	m_isGettingFaster(false),
	m_startBlinkSpeed(0),
	m_startBlinkTime_s(0),
	m_blinkCount(0),
	m_startBlinkCount(0),
	m_blinkTimeFast(0),
	m_blinkedCount(0),
	m_isDraw(false)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Blink::~Blink()
{

}

/// <summary>
/// 初期化
/// </summary>
/// <param name="blinkTime">点滅する時間（秒指定）</param>
/// <param name="blinkCount">点滅する回数</param>
/// <param name="isGetiingFaster">点滅をだんだん早くするか</param>
/// <param name="blinkSpeed">点滅する速さ</param>
void Blink::Initialize(float blinkTime, int blinkCount, bool isGetiingFaster, float blinkSpeed )
{
	//パラメータ初期化
	
	m_isGettingFaster = isGetiingFaster;
	
	//初期は点滅していない状態にする
	m_isBlink = false;
	
	//ブリンクタイム
	m_startBlinkTime_s = blinkTime;
	m_blinkTime_s = m_startBlinkTime_s;
	m_blinkTimeFast = m_startBlinkTime_s;
	
	//ブリンクスピード
	m_startBlinkSpeed = blinkSpeed;
	m_blinkSpeed = m_startBlinkSpeed;
	
	//ブリンク回数
	m_startBlinkCount = blinkCount;
	m_blinkCount = m_startBlinkCount;

	//表示
	m_blinkedCount = 0;
	m_isDraw = true;

}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void Blink::Update(const DX::StepTimer& timer)
{
	//経過時間
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	//m_isBlinkがtrue & 点滅回数が０より高ければ点滅する
	if (m_isBlink && m_blinkCount > 0)
	{
		//ブリンクタイムを経過時間で引く
		m_blinkTime_s -= elapsedTime;

		//ブリンクタイムが０以下になったら点滅状態の変更
		if (m_blinkTime_s <= 0)
		{
			//点滅状態の変更
			m_isDraw = !m_isDraw;

			//点滅してついた瞬間の処理
			BlinkedCheck();
			
		}
	}
	else
	{
		//点滅していないのであればReSetをかけておく
		ReSet();
	}

}

/// <summary>
/// リセット
/// </summary>
void Blink::ReSet()
{
	//パラメータ初期化

	//初期は点滅していない状態にする
	m_isBlink = false;
	
	//ブリンクタイム
	m_blinkTime_s = m_startBlinkTime_s;
	m_blinkTimeFast = m_startBlinkTime_s;

	//ブリンクスピード
	m_blinkSpeed = m_startBlinkSpeed;
	
	//ブリンク回数
	m_blinkCount = m_startBlinkCount;

	//表示
	m_blinkedCount = 0;
	m_isDraw = true;
}

/// <summary>
/// リセット
/// </summary>
void Blink::Finalize()
{
}

/// <summary>
/// 点滅のついた瞬間の処理
/// </summary>
void Blink::BlinkedCheck()
{
	//ついている状態か
	if (m_isDraw)
	{
		//点滅回数を減らす
		m_blinkCount--;
		//点滅した回数を増やす
		m_blinkedCount++;

		//だんだん早くするか
		if (m_isGettingFaster)
		{
			//タイムを減らす
			m_blinkTimeFast -= m_blinkSpeed;

			//ブリンクタイムをだんだん早くする
			m_blinkTime_s = m_blinkTimeFast;

		}
		else
		{
			//ブリンクタイムを初期化
			m_blinkTime_s = m_startBlinkTime_s;
		}
	}
}
