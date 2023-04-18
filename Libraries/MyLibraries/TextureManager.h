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

/// <summary>
/// テクスチャマネージャー
/// </summary>
class TextureManager :public Singleton<TextureManager>
{
private:
	//テクスチャ
	std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;
public:
	/// <summary>
	/// テクスチャを読み込む
	/// </summary>
	/// <param name="fileName">テクスチャのファイルパス</param>
	/// <returns>テクスチャ</returns>
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> LoadTexture(std::wstring fileName);
};