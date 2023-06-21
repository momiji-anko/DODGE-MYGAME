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



void FireEffectManager::Create()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

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
	
	int count = 20;
	
	for (int i = 0; i < count; i++) 
	{
		std::unique_ptr<FireShader> effect = std::make_unique<FireShader>();
		effect->Create();
		m_effectList.push_back(std::move(effect));
	}

}

void FireEffectManager::Initialize(float life, DirectX::SimpleMath::Vector3 pos)
{
	UNREFERENCED_PARAMETER(pos);

	srand((unsigned int)time(NULL));
	//life,pos,vel の値でm_effectを初期化する
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		int range = 200;
		DirectX::SimpleMath::Vector3 vel = DirectX::SimpleMath::Vector3(((rand() % (range * 2)) - range) / (float)range * 0.04f, ((rand() % (range * 2))) / (float)range * 0.01f, 0);
		while (vel.Length() < 0.001f)
		{
			vel = DirectX::SimpleMath::Vector3(((rand() % (range * 2)) - range) / (float)range * 0.04f, ((rand() % (range * 2))) / (float)range * 0.01f, 0);
		}
		//float a = (std::rand()%10) - 5.0f / 10.0f;

		(*ite)->Initialize(life, DirectX::SimpleMath::Vector3::Zero, vel);
	}

}


void FireEffectManager::Update(const DX::StepTimer& timer)
{
	//timerを渡してm_effectの更新処理を行う
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->Update(timer);
	}
}

void FireEffectManager::Render()
{
	
	//m_effectを描画する
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->Render();
	}
}

void FireEffectManager::SetRenderState(const DirectX::SimpleMath::Vector3& camera, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	//camera,view,proj,の値をm_effectに渡す
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->SetRenderState(camera,view,proj);
	}
}



void FireEffectManager::SetStartPosition(const DirectX::SimpleMath::Vector3& pos)
{
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
//		float a = (std::rand() % 10) - 5.0f ;
		(*ite)->SetStartPos(DirectX::SimpleMath::Vector3(pos.x , pos.y, pos.z));
	}

}

void FireEffectManager::SetOffsetPosition(const DirectX::SimpleMath::Vector3& pos)
{
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
//		float a = (std::rand() % 10) - 5.0f ;
		(*ite)->SetOffsetPosition(DirectX::SimpleMath::Vector3(pos.x , pos.y, pos.z));
	}

}
