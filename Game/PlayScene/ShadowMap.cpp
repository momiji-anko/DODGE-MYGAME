#include "pch.h"
#include "ShadowMap.h"
#include "VertexTypes.h"

#pragma comment(lib, "d3dcompiler.lib")


ShadowMap::ShadowMap()
	: m_Path{}
	, m_viewPort{}
{
}

// 初期化関数
void ShadowMap::Initialize(ID3D11Device* device, const wchar_t* path)
{
	SetDirectory(path);

	// 頂点シェーダーの読み込み
	DX::ThrowIfFailed(D3DReadFileToBlob(GetFullName(L"SM_VS.cso"), m_vsBlob.GetAddressOf()));
	DX::ThrowIfFailed(D3DReadFileToBlob(GetFullName(L"SM_VS_Depth.cso"), m_vsBlobDepth.GetAddressOf()));

	// ピクセルシェーダーの読み込み
	DX::ThrowIfFailed(D3DReadFileToBlob(GetFullName(L"SM_PS_Texture.cso"), m_psBlobTexture.GetAddressOf()));
	DX::ThrowIfFailed(D3DReadFileToBlob(GetFullName(L"SM_PS_NoneTex.cso"), m_psBlobNoneTex.GetAddressOf()));

	// 頂点シェーダーの作成
	device->CreateVertexShader(m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());
	device->CreateVertexShader(m_vsBlobDepth->GetBufferPointer(), m_vsBlobDepth->GetBufferSize(), nullptr, m_vertexShaderDepth.GetAddressOf());

	// ピクセルシェーダーの作成
	device->CreatePixelShader(m_psBlobTexture->GetBufferPointer(), m_psBlobTexture->GetBufferSize(), nullptr, m_pixelShaderTexture.GetAddressOf());
	device->CreatePixelShader(m_psBlobNoneTex->GetBufferPointer(), m_psBlobNoneTex->GetBufferSize(), nullptr, m_pixelShaderNoneTex.GetAddressOf());

	// 入力レイアウトの作成
	DX::ThrowIfFailed(
		device->CreateInputLayout(
			DirectX::VertexPositionNormalTangentColorTexture::InputElements, DirectX::VertexPositionNormalTangentColorTexture::InputElementCount,
			m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(),
			m_inputLayout.GetAddressOf()
		)
	);

	// シャドウ マップの作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = SM_RESOLUTION;   // 幅
	descDepth.Height = SM_RESOLUTION;  // 高さ
	descDepth.MipLevels = 1;       // ミップマップ レベル数
	descDepth.ArraySize = 1;       // 配列サイズ
	descDepth.Format = DXGI_FORMAT_R32_TYPELESS;  // フォーマット
	descDepth.SampleDesc.Count = 1;  // マルチサンプリングの設定
	descDepth.SampleDesc.Quality = 0;  // マルチサンプリングの品質
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // デフォルト使用法
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE; // 深度/ステンシル、シェーダ リソース ビューとして使用
	descDepth.CPUAccessFlags = 0;   // CPUからはアクセスしない
	descDepth.MiscFlags = 0;   // その他の設定なし
	DX::ThrowIfFailed(device->CreateTexture2D(&descDepth, nullptr, m_shadowMap.GetAddressOf()));

	// 深度/ステンシル ビューの作成
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;            // ビューのフォーマット
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	DX::ThrowIfFailed(device->CreateDepthStencilView(m_shadowMap.Get(), &descDSV, m_depthView.GetAddressOf()));

	// シェーダ リソース ビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R32_FLOAT; // フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;   // 最初のミップマップ レベル
	srDesc.Texture2D.MipLevels = static_cast<UINT>(-1);  // すべてのミップマップ レベル
	DX::ThrowIfFailed(device->CreateShaderResourceView(m_shadowMap.Get(), &srDesc, m_shaderView.GetAddressOf()));

	// ビューポートの設定
	m_viewPort[0].TopLeftX = 0.0f;		// ビューポート領域の左上X座標。
	m_viewPort[0].TopLeftY = 0.0f;		// ビューポート領域の左上Y座標。
	m_viewPort[0].Width = static_cast<FLOAT>(SM_RESOLUTION);	// ビューポート領域の幅
	m_viewPort[0].Height = static_cast<FLOAT>(SM_RESOLUTION);	// ビューポート領域の高さ
	m_viewPort[0].MinDepth = 0.0f;		// ビューポート領域の深度値の最小値
	m_viewPort[0].MaxDepth = 1.0f;		// ビューポート領域の深度値の最大値

	// 定数バッファの作成
	{
		D3D11_BUFFER_DESC desc = {};
		// バッファサイズは１６の倍数でないといけない
		size_t size = sizeof(ConstantBuffer);
		if (size % 16) size++;
		desc.ByteWidth = static_cast<UINT>(size * 16);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		DX::ThrowIfFailed(device->CreateBuffer(&desc, nullptr, m_constantBuffer.GetAddressOf()));
	}

	// シャドウマップ用サンプラーの作成
	D3D11_SAMPLER_DESC descSampler;
	descSampler.Filter = D3D11_FILTER_ANISOTROPIC;
	descSampler.MipLODBias = 0.0f;
	descSampler.MaxAnisotropy = 2;
	descSampler.ComparisonFunc = D3D11_COMPARISON_NEVER;
	descSampler.MinLOD = -FLT_MAX;
	descSampler.MaxLOD = FLT_MAX;
	descSampler.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	descSampler.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	descSampler.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	descSampler.BorderColor[0] = 1.0f;
	descSampler.BorderColor[1] = 1.0f;
	descSampler.BorderColor[2] = 1.0f;
	descSampler.BorderColor[3] = 1.0f;
	DX::ThrowIfFailed(device->CreateSamplerState(&descSampler, m_sampler.GetAddressOf()));

}

void ShadowMap::Begin(ID3D11DeviceContext* context)
{
	// デバイスコンテキストのクリア
	context->ClearState();

	// シャドウマップをクリア
	context->ClearDepthStencilView(m_depthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	//----- 描画対象をシャドウマップテクスチャへ変更 -----//

	// 描画ターゲット
	ID3D11RenderTargetView* pRender[1] = { nullptr };
	context->OMSetRenderTargets(1, pRender, m_depthView.Get());

	// ビューポート
	context->RSSetViewports(1, m_viewPort);
}

void ShadowMap::End(ID3D11DeviceContext* context, const DirectX::SimpleMath::Matrix& lightViewProj)
{
	//----------------------------------//
	// 定数バッファを設定               //
	//----------------------------------//
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		DX::ThrowIfFailed(
			context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)
		);

		ConstantBuffer buffer;
		buffer.LightViewProj = XMMatrixTranspose(lightViewProj);

		*static_cast<ConstantBuffer*>(mappedResource.pData) = buffer;
		context->Unmap(m_constantBuffer.Get(), 0);
	}
	//----------------------------------//
	// 定数バッファを設定（終）         //
	//----------------------------------//
}

void ShadowMap::DrawShadowMap(ID3D11DeviceContext* context)
{
	// 入力レイアウトの設定
	context->IASetInputLayout(m_inputLayout.Get());

	// 頂点シェーダー
	context->VSSetShader(m_vertexShaderDepth.Get(), nullptr, 0);

	// ピクセルシェーダー（指定しない）
	context->PSSetShader(nullptr, nullptr, 0);
}

void ShadowMap::DrawShadow(ID3D11DeviceContext* context, bool texture)
{
	// 入力レイアウトの設定
	context->IASetInputLayout(m_inputLayout.Get());

	// 頂点シェーダー
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->VSSetConstantBuffers(1, 1, m_constantBuffer.GetAddressOf());

	// ピクセルシェーダー
	if (texture)
	{
		context->PSSetShader(m_pixelShaderTexture.Get(), nullptr, 0);
	}
	else
	{
		context->PSSetShader(m_pixelShaderNoneTex.Get(), nullptr, 0);
	}
	context->PSSetShaderResources(1, 1, m_shaderView.GetAddressOf());
	context->PSSetSamplers(1, 1, m_sampler.GetAddressOf());
}

void ShadowMap::SetDirectory(const wchar_t* path)
{
	if (path && *path != 0)
	{
		wcscpy_s(m_Path, path);
		size_t len = wcsnlen(m_Path, MAX_PATH);
		if (len > 0 && len < (MAX_PATH - 1))
		{
			// Ensure it has a trailing slash
			if (m_Path[len - 1] != L'\\')
			{
				m_Path[len] = L'\\';
				m_Path[len + 1] = 0;
			}
		}
	}
	else
		*m_Path = 0;
}
