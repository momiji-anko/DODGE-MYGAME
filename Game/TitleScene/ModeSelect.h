/*
* 2023/06/12
* ModeSelect.h
* モードセレクト
* 麻生　楓
*/
#pragma once
#include "Game/GameMain.h"
#include<string>
#include<vector>

/// <summary>
/// モードセレクト
/// </summary>
class ModeSelect
{
public:
	/// <summary>
	/// プレイヤーモードの取得
	/// </summary>
	/// <returns>プレイヤーモード</returns>
	GameMain::PlayerMode GetPlayerMode() { return m_playerMode; }
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ModeSelect();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ModeSelect();


	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <returns>次の選択画面に行くか</returns>
	bool  Update(const DX::StepTimer& timer);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();
private:
	//モードファイルパス
	std::vector< std::wstring> m_modeFilePaths;
	
	//選択したプレイヤーモード
	GameMain::PlayerMode m_playerMode;

	//モードセレクトテクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_modeSelectTextures;

};