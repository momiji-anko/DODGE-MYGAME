//-----------------------------------
//	エフェクト管理クラス	
//	FireEffectManager.h
//-----------------------------------
#pragma once

#include "StepTimer.h"

#include <SimpleMath.h>
#include <list>
#include <WICTextureLoader.h>

#include"FireShader.h"

/// <summary>
/// ファイヤーエフェクト管理クラス
/// </summary>
class FireEffectManager
{
public:
	/// <summary>
	/// 描画設定
	/// </summary>
	/// <param name="camera">カメラの座標</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="proj">プロジェクション行列</param>
	void SetRenderState(const DirectX::SimpleMath::Vector3& camera, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

	/// <summary>
	/// 初期座標
	/// </summary>
	/// <param name="pos">座標</param>
	void SetStartPosition(const DirectX::SimpleMath::Vector3& pos);

	/// <summary>
	/// オフセット座標設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetOffsetPosition(const DirectX::SimpleMath::Vector3& pos);

public:
	/// <summary>
	/// シェーダーの作成
	/// </summary>
	void Create();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="life">寿命</param>
	/// <param name="pos">座標</param>
	/// <param name="velocity">移動量</param>
	void Initialize(float life, DirectX::SimpleMath::Vector3 pos);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">更新</param>
	void Update(const DX::StepTimer& timer);
	/// <summary>
	/// 描画
	/// </summary>
	void Render();

private:

	//バッチエフェクト
	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect;

	//プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//エフェクトのポインタ
	std::list<std::unique_ptr<FireShader>> m_effectList;
	//テクスチャデータ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;
};