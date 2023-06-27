/*
* 2023/04/10
* ModelManager.cpp
* モデルを読み込んで管理する
* 麻生　楓
*/
#include"pch.h"
#include"ModelManager.h"
#include"DeviceResources.h"
#include<Effects.h>

/// <summary>
/// モデル読み込み
/// </summary>
/// <param name="fileName">モデルのファイルパス</param>
/// <returns>モデルの生ポインタ</returns>
DirectX::Model* ModelManager::LoadModel(const std::wstring& fileName)
{
	//nullptrであればモデルを作る
	if (m_models[fileName] == nullptr)
	{
		//DeviceResourcesからデバイスの取得
		ID3D11Device1* device = DX::DeviceResources::GetInstance()->GetD3DDevice();

		//エフェクトファクトリの作成
		std::unique_ptr<DirectX::EffectFactory> effectFactry = std::make_unique<DirectX::EffectFactory>(device);

		//	テクスチャの読み込みパス指定
		effectFactry->SetDirectory(L"Resources/Models");

		//	ファイルを指定してモデルデータ読み込み＆読み込んだモデルを返す
		m_models[fileName] = DirectX::Model::CreateFromCMO(
			device,
			fileName.c_str(),
			*effectFactry
		);
	}

	//モデルを生ポインタで返す
	return m_models[fileName].get();
}

void ModelManager::Reset()
{
	for (auto& model : m_models)
	{
		model.second.reset();
	}

}
