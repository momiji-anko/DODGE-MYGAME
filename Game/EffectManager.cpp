#include "pch.h"
#include "DeviceResources.h"
#include "EffectManager.h"

#include <SimpleMath.h> 
#include <Effects.h> 
#include <PrimitiveBatch.h> 
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include <CommonStates.h>
#include<time.h>
#include<random>

using namespace DirectX;
using namespace DirectX::SimpleMath;

void EffectManager::Create()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	ID3D11Device1* device = pDR->GetD3DDevice();

	//バッチエフェクトの作成
	m_batchEffect = std::make_unique<AlphaTestEffect>(device);
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);

	// 入力レイアウト生成
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf());
	
	//プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pDR->GetD3DDeviceContext());
	
	//テクスチャのロード
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/shadow.png",
		nullptr,
		m_texture.GetAddressOf());
	
	int count = 100;
	
	for (int i = 0; i < count; i++) 
	{
		std::unique_ptr<ShaderSample> effect = std::make_unique<ShaderSample>();
		effect->Create();
		m_effectList.push_back(std::move(effect));
	}

}

void EffectManager::Initialize(float life,Vector3 pos)
{
	srand((unsigned int)time(NULL));
	//life,pos,vel の値でm_effectを初期化する
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		int range = 200;
		Vector3 vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.05f, ((rand() % (range * 2))) / (float)range * 0.01f, 0);
		while (vel.Length() < 0.01f)
		{
			vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.05f, ((rand() % (range * 2))) / (float)range * 0.01f, 0);
		}
		float a = (std::rand()%10) - 5.0f;

		(*ite)->Initialize(life, DirectX::SimpleMath::Vector3(pos.x + a / 10.0f, pos.y, pos.z), vel);
	}

}


void EffectManager::Update(const DX::StepTimer& timer)
{
	//timerを渡してm_effectの更新処理を行う
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->Update(timer);
	}
}

void EffectManager::Render()
{
	
	//m_effectを描画する
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->Render();
	}
}

void EffectManager::SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	//camera,view,proj,の値をm_effectに渡す
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->SetRenderState(camera,view,proj);
	}
}

void EffectManager::SetGravity(bool gravity)
{
	
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->SetGravity(gravity);
	}
}

void EffectManager::SetStartPosition(DirectX::SimpleMath::Vector3& pos)
{
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		float a = (std::rand() % 10) - 5.0f ;
		(*ite)->SetStartPos(DirectX::SimpleMath::Vector3(pos.x + a / 10.0f, pos.y, pos.z));
	}

}

void EffectManager::Reset()
{
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->Reset();
	}
}
