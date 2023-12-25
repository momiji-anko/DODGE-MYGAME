/*
* *2023/03/10
* FireEffectManager.h
* ファイヤーシェーダー
* 麻生　楓
*/
#include "pch.h"
#include "DeviceResources.h"
#include "FireEffectManager.h"

#include <SimpleMath.h> 
#include <Effects.h> 
#include <PrimitiveBatch.h> 
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include <CommonStates.h>
#include<time.h>
#include<random>


/// <summary>
/// シェーダーの作成
/// </summary>
void FireEffectManager::Create()
{
	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//D3Dデバイス取得
	ID3D11Device1* device = pDR->GetD3DDevice();

	//バッチエフェクトの作成
	m_batchEffect = std::make_unique<DirectX::AlphaTestEffect>(device);
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);

	// 入力レイアウト生成
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf());
	
	//プリミティブバッチの作成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(pDR->GetD3DDeviceContext());
	
	//テクスチャのロード
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/shadow.png",
		nullptr,
		m_texture.GetAddressOf());
	
	//火の粉の数
	int fireNum = 20;
	
	//火の粉の数分生成
	for (int i = 0; i < fireNum; i++) 
	{
		//生成
		std::unique_ptr<FireShader> effect = std::make_unique<FireShader>();
		//エフェクト作成
		effect->Create();
		//エフェクトリストに追加
		m_effectList.push_back(std::move(effect));
	}

}

/// <summary>
/// 初期化
/// </summary>
/// <param name="life">寿命</param>
/// <param name="pos">座標</param>
/// <param name="velocity">移動量</param>
void FireEffectManager::Initialize(float life, DirectX::SimpleMath::Vector3 pos)
{
	UNREFERENCED_PARAMETER(pos);

	//シード値設定
	srand((unsigned int)time(NULL));
	//life,pos,vel の値でm_effectを初期化する
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		//振れ幅
		int range = 200;
		//移動量をランダムで出す
		DirectX::SimpleMath::Vector3 vel = DirectX::SimpleMath::Vector3(((rand() % (range * 2)) - range) / (float)range * 0.04f, ((rand() % (range * 2))) / (float)range * 0.01f, 0);
		//移動量が少ない場合やり直す
		while (vel.Length() < 0.001f)
		{
			vel = DirectX::SimpleMath::Vector3(((rand() % (range * 2)) - range) / (float)range * 0.04f, ((rand() % (range * 2))) / (float)range * 0.01f, 0);
		}
		
		//エフェクトを初期化する
		(*ite)->Initialize(life, DirectX::SimpleMath::Vector3::Zero, vel);
	}

}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">更新</param>
void FireEffectManager::Update(const DX::StepTimer& timer)
{
	//エフェクト更新
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->Update(timer);
	}
}

/// <summary>
/// 描画
/// </summary>
void FireEffectManager::Render()
{
	//エフェクトを描画する
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->Render();
	}
}

/// <summary>
/// 描画設定
/// </summary>
/// <param name="camera">カメラの座標</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
void FireEffectManager::SetRenderState(const DirectX::SimpleMath::Vector3& camera, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	//camera,view,proj,の値をエフェクトに渡す
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->SetRenderState(camera,view,proj);
	}
}

/// <summary>
/// 初期座標
/// </summary>
/// <param name="pos">座標</param>
void FireEffectManager::SetStartPosition(const DirectX::SimpleMath::Vector3& pos)
{
	//全てのエフェクトに初期座標を設定
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->SetStartPos(DirectX::SimpleMath::Vector3(pos.x , pos.y, pos.z));
	}

}

/// <summary>
/// オフセット座標設定
/// </summary>
/// <param name="pos">座標</param>
void FireEffectManager::SetOffsetPosition(const DirectX::SimpleMath::Vector3& pos)
{
	//全てのエフェクトにオフセット座標を設定
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->SetOffsetPosition(DirectX::SimpleMath::Vector3(pos.x , pos.y, pos.z));
	}

}
