#pragma once
#include"StepTimer.h"

class TitleTextureMove
{
public:
	TitleTextureMove();
	~TitleTextureMove();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <returns>次のシーン</returns>
	void  Update(const DX::StepTimer& timer);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();


private:
	


};