/*
* 2023/06/12
* StageSelect.h
* ステージセレクト
* 麻生　楓
*/
#pragma once
#include"StepTimer.h"
#include<SimpleMath.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>
#include<Keyboard.h>
#include "../IScene.h"
#include "Game/GameMain.h"

#include"Libraries/ADX2/Adx2.h"
#include"../../Helpers/CueSheet_0.h"
#include"../../Helpers/DODGESound_acf.h"
#include"../PlayScene/Stage/Stage.h"
#include"Game/PlayScene/Stage/StageManager.h"

/// <summary>
/// ステージセレクト
/// </summary>
class StageSelect
{
public:

	/// <summary>
	/// 選択したステージ番号の取得
	/// </summary>
	/// <returns>選択したステージ番号</returns>
	int  GetSelectStageNum() { return m_selectStageNum; }

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stageManager">ステージマネージャーの生ポインタ</param>
	StageSelect(StageManager* stageManager);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~StageSelect();

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
	//ステージテキストテクスチャのファイルパス
	std::wstring m_stageTextTextureFilePath;
	//ステージ番号テクスチャのファイルパス
	std::wstring m_stageNumTextureFilePath;
	//ステージ矢印テクスチャのファイルパス
	std::wstring m_stageArrowTextureFilePath;

	//座標
	DirectX::SimpleMath::Vector2 m_texturePosition;
	
	//選択したステージの番号
	int m_selectStageNum;
	//ステージの最大数
	int m_stageMaxNum;

	//ステージマネージャー
	StageManager* m_stageManager;

	//ステージセレクトテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stageTextTexture;
	
	//テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stageNumTexture;
	
	//ステージセレクトテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stageArrowTexture;

};