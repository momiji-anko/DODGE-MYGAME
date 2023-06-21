/*
* 2023/05/16
* Fade.h
* フェード
* 麻生　楓
*/
#pragma once

#include "StepTimer.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <Model.h>
#include <vector>

/// <summary>
/// フェード
/// </summary>
class Fade
{
public:

	//インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	/// <summary>
	/// コンストバッファー
	/// </summary>
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Time;
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Fade();
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Fade();

	/// <summary>
	/// シェーダー作成
	/// </summary>
	void Create();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="fade">フェードアルファ</param>
	void Initialize(const DirectX::SimpleMath::Vector3& pos,float fade = 1.0f);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	void Update(DX::StepTimer timer);

	/// <summary>
	/// 描画
	/// </summary>
	void Render();

	/// <summary>
	/// 削除
	/// </summary>
	void Lost();

	/// <summary>
	/// 描画設定
	/// </summary>
	/// <param name="camera">カメラの座標</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="proj">プロジェクション行列</param>
	void SetRenderState(const DirectX::SimpleMath::Vector3& camera, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

	/// <summary>
	/// メイン描画処理
	/// </summary>
	/// <param name="world">ワールド行列</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="proj">プロジェクション行列</param>
	void Draw(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

	/// <summary>
	/// フェードイン
	/// </summary>
	void FadeIn()  { m_fadeState = State::FADE_IN; }
	/// <summary>
	/// フェードアウト
	/// </summary>
	void FadeOut() { m_fadeState = State::FADE_OUT; }

	/// <summary>
	/// フェードアウトしているかどうか
	/// </summary>
	/// <returns>true=フェードアウトしている、false=フェードアウトしていない</returns>
	bool ISClose() { return m_fadeTime_s >= 1; }
	/// <summary>
	/// フェードインしているかどうか
	/// </summary>
	/// <returns>true=フェードインしている、false=フェードインしていない</returns>
	bool ISOpen()  { return m_fadeTime_s <= 0; }

private:
	//フェードアウト終了値
	static const float FADEOUT_END_NUM;
	//フェードイン終了値
	static const float FADEIN_END_NUM;

	//シェーダーに渡すバッファー
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	//タイマー
	DX::StepTimer                           m_timer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;

	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;


	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

	//カメラの座標
	DirectX::SimpleMath::Vector3 m_centerPosition;
	//ビルボード用行列
	DirectX::SimpleMath::Matrix m_billboardTranslation;
	//ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	//プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;


	//フェードの状態
	enum class State
	{
		FADE_IN,
		FADE_OUT,
		FADE_NONE,
	};
	//フェードタイム
	float m_fadeTime_s;
	//フェードの状態
	State m_fadeState;
};