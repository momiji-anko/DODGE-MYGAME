#include "pch.h"
#include "Fade.h"
#include"BinaryFile.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include<Mouse.h>
#include<Keyboard.h>
#include"DeviceResources.h"



const std::vector<D3D11_INPUT_ELEMENT_DESC> Fade::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

Fade::Fade():
	m_CBuffer{},
	m_timer{},
	m_inputLayout{},
	m_batch{},
	m_states{},
	m_texture{},
	m_vertexShader{},
	m_pixelShader{},
	m_geometryShader{},
	m_centerPosition{},
	m_billboardTranslation{},
	m_view{},
	m_proj{},
	m_model{},
	m_capture{},
	m_rtv{},
	m_srv{},
	m_fadeTime_s{},
	m_fadeState{}
{
	
	
}

Fade::~Fade()
{
	Lost();
}
//-----------------------------------------------------------------------------------
//	Create()
//	シェーダ等を生成
//	IN	:	デバイス			DX::DeviceResources* deviceResources
//	RE	:	void
//-----------------------------------------------------------------------------------
void Fade::Create()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	ID3D11Device1* device = pDR->GetD3DDevice();

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/FadeVS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/FadePS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/FadeGS.cso");

	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf()
	);
	

	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}

	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	const wchar_t* name = L"Resources\\Textures\\089.png";
	DirectX::CreateWICTextureFromFile(pDR->GetD3DDevice(), name, nullptr, m_texture.GetAddressOf());



	// プリミティブバッチの作成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	m_states = std::make_unique<DirectX::CommonStates>(device);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage		=	D3D11_USAGE_DEFAULT;
	bd.ByteWidth	=	sizeof(ConstBuffer);
	bd.BindFlags	=	D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;	
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);


	/*std::unique_ptr<EffectFactory> fxFactory = std::make_unique<EffectFactory>(device);
	fxFactory->SetDirectory(L"Resources/Models");

	m_model = Model::CreateFromCMO(device, L"Resources/Models/cup.cmo", *fxFactory);*/

}
//-----------------------------------------------------------------------------------
//	SetRenderState()
//	カメラ、ビュー、射影の反映
//	IN	:	カメラ		DirectX::SimpleMath::Vector3 camera
//			ビュー行列	DirectX::SimpleMath::Matrix view
//			射影行列	DirectX::SimpleMath::Matrix proj
//	RE	:	void
//-----------------------------------------------------------------------------------
void Fade::SetRenderState(const DirectX::SimpleMath::Vector3& camera, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	m_view = view;
	m_proj = proj;

	m_billboardTranslation = DirectX::SimpleMath::Matrix::CreateBillboard(m_centerPosition, camera, DirectX::SimpleMath::Vector3::UnitY);

	//ビルボードの計算で裏返るので補正
	//Y軸で180度回転する行列
	DirectX::SimpleMath::Matrix rev = DirectX::SimpleMath::Matrix::Identity;
	rev._11 = -1;
	rev._33 = -1;

	//補正行列を先にかけて他に影響がないようにする
	m_billboardTranslation = rev * m_billboardTranslation;
}

void Fade::Initialize(const DirectX::SimpleMath::Vector3& pos, float fade)
{
	m_centerPosition = pos;
	m_fadeTime_s = fade;
	m_fadeState = State::FADE_NONE;
}

//-----------------------------------------------------------------------------------
//	Update()
//	更新処理
//	IN	:	タイマー		DX::StepTimer timer
//	RE	:	void
//-----------------------------------------------------------------------------------
void Fade::Update(DX::StepTimer timer)
{
	m_timer = timer;
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	switch (m_fadeState)
	{
	case Fade::State::FADE_IN:
		m_fadeTime_s -= elapsedTime;

		if (m_fadeTime_s <= 0)
		{
			m_fadeState = State::FADE_NONE;
		}

		break;
	case Fade::State::FADE_OUT:
		m_fadeTime_s += elapsedTime;

		if (m_fadeTime_s >= 1)
		{
			m_fadeState = State::FADE_NONE;
		}
		break;
	case Fade::State::FADE_NONE:
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------------
//	Render()
//	描画処理(メイン以外の外部要因）
//	IN	:	void
//	RE	:	void
//-----------------------------------------------------------------------------------
void Fade::Render()
{
	//回転するモデルを表示
	static float rot = 0.0f;
	rot += 0.1f;
	//m_model->Draw(pDR->GetD3DDeviceContext(),*m_states, Matrix::CreateRotationZ(rot), m_view, m_proj);

	//全画面エフェクト
	DirectX::SimpleMath::Matrix  mat = DirectX::SimpleMath::Matrix::Identity;
	Draw(mat, mat, mat);

	//板ポリゴンエフェクト
	//Draw(m_billboardTranslation, m_view, m_proj);
}
//-----------------------------------------------------------------------------------
//	Draw()
//	メイン描画処理
//	IN	:	void
//	RE	:	void
//-----------------------------------------------------------------------------------
void Fade::Draw(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// 頂点情報(板ポリゴンの頂点の座標情報）
	DirectX::VertexPositionColorTexture vertex[1] =
	{
		DirectX::VertexPositionColorTexture(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),DirectX::SimpleMath::Vector4::One, DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
	};

	//シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = world.Transpose();
	//Time		x:経過時間(トータル秒)	y:1Fの経過時間(秒）	z:反復（サインカーブ） w:未使用（暫定で１）
	
	//cbuff.Time = Vector4((float)m_timer.GetTotalSeconds(), (float)m_timer.GetElapsedSeconds(), Mouse::Get().GetState().x / 800.0f, Mouse::Get().GetState().y / 600.0f);
	
	cbuff.Time = DirectX::SimpleMath::Vector4((float)m_timer.GetTotalSeconds(), m_fadeTime_s, DirectX::Mouse::Get().GetState().x / 800.0f, DirectX::Mouse::Get().GetState().y / 600.0f);
	
	
	//受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);

	//画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//半透明描画指定
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	// カリングは反時計回り
	context->RSSetState(m_states->CullCounterClockwise());

	//シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);

	//ピクセルシェーダにテクスチャを登録する。
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());


	//インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	// 板ポリゴンを描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();

	//シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);

}

void Fade::Lost()
{
	m_CBuffer.Reset();
	m_inputLayout.Reset();
	m_batch.reset();
	m_states.reset();
	m_texture.Reset();
	m_vertexShader.Reset();
	m_pixelShader.Reset();
}

//モデル等をテクスチャ(m_srv)に描画する処理
void Fade::RenderModel()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	D3D11_TEXTURE2D_DESC texDesc;
	pDR->GetRenderTarget()->GetDesc(&texDesc);
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	pDR->GetD3DDevice()->CreateTexture2D(&texDesc, NULL, m_capture.ReleaseAndGetAddressOf());

	// レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	// レンダーターゲットビューの生成
	pDR->GetD3DDevice()->CreateRenderTargetView(m_capture.Get(), &rtvDesc, &m_rtv);

	//シェーダリソースビューの設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	//レンダーターゲットビュー,深度ビューを取得（後で元に戻すため）
	ID3D11RenderTargetView* defaultRTV = pDR->GetRenderTargetView();
	ID3D11DepthStencilView* pDSV = pDR->GetDepthStencilView();

	//背景色の設定（アルファを０にするとオブジェクトのみ表示）
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 0.0f };

	//レンダーターゲットビューをセットし、初期化する
	context->OMSetRenderTargets(1, m_rtv.GetAddressOf(), pDSV);
	context->ClearRenderTargetView(m_rtv.Get(), clearColor);
	context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//----------------------------------------------------------------------------
	//とりあえずの動きのために回転
	static float rot = 0.0f;
	rot += 0.1f;

	//モデルを描画
	m_model->Draw(pDR->GetD3DDeviceContext(), *m_states, DirectX::SimpleMath::Matrix::CreateRotationZ(rot), m_view, m_proj);

	

	//------------------------------------------------------------------
	//設定をもとに戻す
	clearColor[0] = 0.3f;
	clearColor[1] = 0.3f;
	context->OMSetRenderTargets(1, &defaultRTV, pDSV);
	context->ClearRenderTargetView(defaultRTV, clearColor);
	context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

