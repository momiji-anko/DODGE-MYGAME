/*
* 2023/04/10
* TextureManager.cpp
* テクスチャを読み込み&管理する
* 麻生　楓
*/
#include"pch.h"
#include"TextureManager.h"
#include"DeviceResources.h"
#include"MemoryLeakDetector.h"

/// <summary>
/// テクスチャを読み込む
/// </summary>
/// <param name="fileName">テクスチャのファイルパス</param>
/// <returns>テクスチャ</returns>
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureManager::LoadTexture(const std::wstring& fileName)
{
	//テクスチャが既に読み込まれていない場合読み込む
	if (m_textures[fileName] == nullptr)
	{
		//DeviceResourcesからデバイスの取得
		ID3D11Device1* device = DX::DeviceResources::GetInstance()->GetD3DDevice();

		Microsoft::WRL::ComPtr<ID3D11Resource> resouce;

		//テクスチャの読み込み
		DirectX::CreateWICTextureFromFile(
			device,
			fileName.c_str(),
			resouce.ReleaseAndGetAddressOf(),
			m_textures[fileName].ReleaseAndGetAddressOf()
		);

		//テクスチャサイズを設定
		TextureSize(resouce, fileName);
		
	}

	return m_textures[fileName];
}

/// <summary>
/// テクスチャサイズの取得
/// </summary>
/// <param name="fileName">テクスチャのファイルパス</param>
/// <returns>テクスチャサイズ</returns>
DirectX::SimpleMath::Vector2 TextureManager::GetTextureSize(const std::wstring& fileName)
{
	//テクスチャが読み込まれた場合テクスチャサイズを返す
	if (m_textures[fileName] != nullptr)
	{
		return m_textureSize[fileName];
	}

	//テクスチャが読み込まれていない場合ゼロを返す
	return DirectX::SimpleMath::Vector2::Zero;
}

/// <summary>
/// テクスチャサイズの設定
/// </summary>
/// <param name="resouce">リソース</param>
/// <param name="fileName">テクスチャのファイルパス</param>
void TextureManager::TextureSize(const Microsoft::WRL::ComPtr<ID3D11Resource>& resouce, const std::wstring& fileName)
{
	//テクスチャ２D
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
	//リソースからテクスチャ２Dにキャスト
	resouce.As(&texture2d);
	
	D3D11_TEXTURE2D_DESC texDesc;
	//テクスチャ２DからDescを取得
	texture2d->GetDesc(&texDesc);

	//テクスチャサイズを設定
	m_textureSize[fileName] = DirectX::SimpleMath::Vector2(static_cast<float>(texDesc.Width), static_cast<float>(texDesc.Height));
}
