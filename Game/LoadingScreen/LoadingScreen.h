/*
ロード画面
*/
#pragma once


#include <thread>
#include <atomic>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>

#include "../ILoadingScreen.h"

#include<SimpleMath.h>
#include<vector>

class LoadingScreen : public ILoadingScreen
{

public:

	static const float TEXTURE_MOVE_SPEED;
	static const float TEXTURE_POSITION_RESET;

	// コンストラクタ
	LoadingScreen();

	// デストラクタ
	~LoadingScreen();

	// 初期化
	void Initialize(IScene* targetScene) override;

	// 更新
	void Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

	// 読み込み中かどうかの取得
	bool IsLoading();

private:
	// 対象シーンのリソースの読み込み
	void LoadSceneResources(IScene* targetScene) override;

private:

	

	// リソース読み込み用
	std::thread m_loadingThread;
	std::atomic<bool> m_isLoading;

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	DirectX::SimpleMath::Vector2 m_textureStartPosition;
	std::vector<DirectX::SimpleMath::Vector2> m_texturePositions;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_blackTexture;

};