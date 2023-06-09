/*
* 2023/04/10
* TextureManager.h
* テクスチャを読み込み＆管理するクラス
* 麻生　楓
*/
#pragma once 
#include"Singleton.h"
#include<WICTextureLoader.h>
#include<unordered_map>
#include<SimpleMath.h>

/// <summary>
/// テクスチャマネージャー
/// </summary>
class TextureManager :public Singleton<TextureManager>
{

public:

	TextureManager();
	
	/// <summary>
	/// テクスチャを読み込む
	/// </summary>
	/// <param name="fileName">テクスチャのファイルパス</param>
	/// <returns>テクスチャ</returns>
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> LoadTexture(const std::wstring& fileName);

	/// <summary>
	/// テクスチャサイズの取得
	/// </summary>
	/// <param name="fileName">テクスチャのファイルパス</param>
	/// <returns>テクスチャサイズ</returns>
	DirectX::SimpleMath::Vector2 GetTextureSize(const std::wstring& fileName);

	/// <summary>
	/// テクスチャサイズの設定
	/// </summary>
	/// <param name="resouce">リソース</param>
	/// <param name="fileName">テクスチャのファイルパス</param>
	void TextureSize(const Microsoft::WRL::ComPtr<ID3D11Resource>& resouce, const std::wstring& fileName);

private:
	//テクスチャ
	std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;

	//テクスチャサイズ
	std::unordered_map<std::wstring, DirectX::SimpleMath::Vector2> m_textureSize;
};