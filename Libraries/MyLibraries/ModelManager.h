/*
* 2023/04/10
* ModelManager.h 
* モデルマネージャー
* 麻生　楓
*/
#pragma once 
#include"Singleton.h"
#include <unordered_map>
#include<Model.h>

/// <summary>
/// モデルマネージャー
/// </summary>
class ModelManager : public Singleton<ModelManager>
{


public:
	/// <summary>
	/// モデル読み込み
	/// </summary>
	/// <param name="fileName">モデルのファイルパス</param>
	/// <returns>モデルの生ポインタ</returns>
	DirectX::Model* LoadModel(const std::wstring& fileName);
private:
	//モデルの配列
	std::unordered_map<std::wstring, std::unique_ptr<DirectX::Model>> m_models;

};