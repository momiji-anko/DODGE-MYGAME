#pragma once
#include"StepTimer.h"
#include<vector>
#include<SimpleMath.h>

class TitleText
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleText();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleText();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	void  Update(const DX::StepTimer& timer);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

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
	static const float MOVED_TEXTURE_ALPHA;
	//アルファの最小値
	static const float MOVEING_TEXTURE_ALPHA;


	//タイトルのテクスチャのファイルパス
	std::wstring m_titleTextureFilePath;
	//タイトルのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_titileTexture;
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

};