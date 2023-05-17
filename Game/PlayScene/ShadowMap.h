#pragma once

#include <d3dcompiler.h>
#include "SimpleMath.h"

class ShadowMap
{

private:

	// パス付きの名前に変換する関数
	wchar_t* GetFullName(const wchar_t* name)
	{
		static wchar_t fullName[MAX_PATH];

		wcscpy_s(fullName, m_Path);
		wcscat_s(fullName, name);

		return fullName;
	}

public:

	// コンストラクタ
	ShadowMap();

	// 初期化関数
	void Initialize(ID3D11Device* device, const wchar_t* path = nullptr);

	// シャドウマップ作成開始
	void Begin(ID3D11DeviceContext* context);

	// シャドウマップ作成終了
	void End(ID3D11DeviceContext* context, const DirectX::SimpleMath::Matrix& lightViewProj);

	// シャドウマップ作成時に呼び出すカスタムステート
	void DrawShadowMap(ID3D11DeviceContext* context);

	// 影付きのモデルを描画したい時に呼び出すカスタムステート
	// テクスチャなしのモデルを表示したい場合は第２引数をfalseにする事
	void DrawShadow(ID3D11DeviceContext* context, bool texture = true);

	// シャドウマップ用テクスチャリソースを取得する関数
	ID3D11ShaderResourceView* GetShadowMapTexture()
	{
		return m_shaderView.Get();
	}
	
	void SetDirectory(const wchar_t* path);

private:

	// シャドウマップの解像度
	static const int SM_RESOLUTION = 1024;

	// csoの場所
	wchar_t m_Path[MAX_PATH];

	// 頂点シェーダーのデータ
	Microsoft::WRL::ComPtr<ID3DBlob> m_vsBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> m_vsBlobDepth;

	// ピクセルシェーダーのデータ
	Microsoft::WRL::ComPtr<ID3DBlob> m_psBlobTexture;
	Microsoft::WRL::ComPtr<ID3DBlob> m_psBlobNoneTex;

	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShaderDepth;

	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShaderTexture;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShaderNoneTex;

	// シャドウマップ用テクスチャ
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_shadowMap;

	// 深度バッファビュー
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthView;

	// シェーダーリソースビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderView;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// シャドウマップ作成時のビューポート
	D3D11_VIEWPORT m_viewPort[1];

	// 定数バッファの構造体の定義
	struct ConstantBuffer
	{
		DirectX::XMMATRIX LightViewProj;
	};

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	// サンプラー
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;

};

