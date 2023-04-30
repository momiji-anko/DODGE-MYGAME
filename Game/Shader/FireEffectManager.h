//-----------------------------------
//	エフェクト管理クラス	
//	EffectManager.h
//-----------------------------------
#pragma once

#include "StepTimer.h"

#include <SimpleMath.h>
#include <list>
#include <WICTextureLoader.h>

#include"FireShader.h"

class FireEffectManager
{
public:

	void SetRenderState(const DirectX::SimpleMath::Vector3& camera, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);


	void SetStartPosition(const DirectX::SimpleMath::Vector3& pos);
	void SetOffsetPosition(const DirectX::SimpleMath::Vector3& pos);



public:

	void Create();

	//Create()で全部処理は長いので分割
	void Initialize(float life, DirectX::SimpleMath::Vector3 pos);

	void Update(const DX::StepTimer& timer);

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