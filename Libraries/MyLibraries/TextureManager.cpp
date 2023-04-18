/*
* 2023/04/10
* TextureManager.cpp
* テクスチャを読み込み&管理する
* 麻生　楓
*/
#include"pch.h"
#include"TextureManager.h"
#include"DeviceResources.h"

/// <summary>
/// テクスチャを読み込む
/// </summary>
/// <param name="fileName">テクスチャのファイルパス</param>
/// <returns>テクスチャ</returns>
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureManager::LoadTexture(std::wstring fileName)
{
	if (m_textures[fileName] == nullptr)
	{
		//DeviceResourcesからデバイスの取得
		ID3D11Device1* device = DX::DeviceResources::GetInstance()->GetD3DDevice();

		//テクスチャの読み込み
		DirectX::CreateWICTextureFromFile(
			device,
			fileName.c_str(),
			nullptr,
			m_textures[fileName].ReleaseAndGetAddressOf()
		);
	}

	return m_textures[fileName].Get();
}
