#include "pch.h"
#include "FireShader.h"
#include "BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include"Game/PlayScene/MyRandom.h"



const std::vector<D3D11_INPUT_ELEMENT_DESC> FireShader::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

FireShader::FireShader()
	:
	m_scale(1.0f),
	m_CBuffer{},
	m_timer{},
	m_inputLayout{},
	m_batch{},
	m_states{},
	m_texture{},
	m_vertexShader{},
	m_pixelShader{},
	m_geometryShader{},
	m_world{},
	m_view{},
	m_proj{},
	m_camera{},
	m_gravity{},
	m_position{},
	m_velocity{},
	m_life{},
	m_startPosition{},
	m_startVelocity{},
	m_startLife{},
	m_wait{},
	m_positionList{},
	m_rot{},
	m_angle{},
	m_color{},
	m_rutin{},
	m_colorTime{},
	m_time{},
	m_offsetPos{}


{
}

FireShader::~FireShader()
{
	Lost();
}
//-----------------------------------------------------------------------------------
//	Create()
//	シェーダ等を生成
//	IN	:	デバイス			DX::DeviceResources* deviceResources
//	RE	:	void
//-----------------------------------------------------------------------------------
void FireShader::Create()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	auto device = pDR->GetD3DDevice();

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");

	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}

	const wchar_t* name = L"Resources\\Textures\\6.bmp";
	DirectX::CreateWICTextureFromFile(pDR->GetD3DDevice(), name, nullptr, m_texture.GetAddressOf());


	// プリミティブバッチの作成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	m_states = std::make_unique<DirectX::CommonStates>(device);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

	m_world = DirectX::SimpleMath::Matrix::Identity;


	m_angle = 0.0f;

	int count = 8;

	for (int i = 0; i < count; i++)
	{
		m_positionList.push_back(DirectX::SimpleMath::Vector3(1.0f, 0.0f, 1.0f));
	}

}
//-----------------------------------------------------------------------------------
//	SetRenderState()
//	カメラ、ビュー、射影の反映
//	IN	:	カメラ		DirectX::SimpleMath::Vector3 camera
//			ビュー行列	DirectX::SimpleMath::Matrix view
//			射影行列	DirectX::SimpleMath::Matrix proj
//	RE	:	void
//-----------------------------------------------------------------------------------
void FireShader::SetRenderState(const DirectX::SimpleMath::Vector3& camera, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	m_camera = camera;
	m_view = view;
	m_proj = proj;
}

void FireShader::Initialize(float life, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& velocity)
{
	m_startPosition = pos;
	m_startVelocity = velocity;
	m_startLife = life;



	int seed = 100;
	m_wait = ((rand() % seed) / (float)seed) * m_startLife;

	Reset();
	m_rutin = 0;
	if (m_velocity.x > 0)
	{

		m_gravity = DirectX::SimpleMath::Vector3(0, -0.001f, 0);
	}
	else if (m_velocity.x < 0)
	{

		m_gravity = DirectX::SimpleMath::Vector3(0, 0.001f, 0);
	}



}

void FireShader::Reset()
{
	m_position = m_startPosition;
	m_velocity = m_startVelocity;
	m_life = m_startLife;
	m_scale = 0.2f;
	m_rutin = 0;
	m_time = 0.0f;
}
//-----------------------------------------------------------------------------------
//	Update()
//	更新処理
//	IN	:	タイマー		DX::StepTimer timer
//	RE	:	void
//-----------------------------------------------------------------------------------
void FireShader::Update(DX::StepTimer timer)
{
	//処理に使う秒速計(1秒で1.0f)を取得する。
	float time = static_cast<float>(timer.GetElapsedSeconds());

	m_wait -= time;

	if (m_wait > 0)
	{
		return;
	}

	DirectX::SimpleMath::Vector3 vel = DirectX::SimpleMath::Vector3(0.002f, 0, 0);



	if (m_position.x > 0)
	{
		m_velocity.x -= vel.x;
	}
	else if (m_position.x < 0)
	{
		m_velocity.x += vel.x;
	}




	m_position += m_velocity;

	m_scale = Lerp(1.0f, 0.0f, 1.0f - m_life / m_startLife);
	m_angle += 0.1f;

	m_life -= time;

	if (m_life < 0.0f)
	{
		Reset();
	}


}

//-----------------------------------------------------------------------------------
//	Render()
//	描画処理(メイン以外の外部要因）
//	IN	:	void
//	RE	:	void
//-----------------------------------------------------------------------------------
void FireShader::Render()
{
	////ビルボード化
	m_world = DirectX::SimpleMath::Matrix::Identity;

	DirectX::SimpleMath::Matrix wrold = DirectX::SimpleMath::Matrix::Identity;
	wrold = DirectX::SimpleMath::Matrix::CreateBillboard(m_position + m_offsetPos, m_camera, DirectX::SimpleMath::Vector3::UnitY);

	DirectX::SimpleMath::Matrix revMat = DirectX::SimpleMath::Matrix::Identity;
	revMat._11 = -1.0f;
	revMat._33 = -1.0f;

	DirectX::SimpleMath::Matrix scaleMat, rotMat;
	scaleMat = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
	rotMat = DirectX::SimpleMath::Matrix::CreateRotationZ(m_angle);

	m_world = scaleMat * rotMat * revMat * wrold;

	ShaderDraw();
}
//-----------------------------------------------------------------------------------
//	ShaderDraw()
//	メイン描画処理
//	IN	:	void
//	RE	:	void
//-----------------------------------------------------------------------------------
void FireShader::ShaderDraw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	auto context = pDR->GetD3DDeviceContext();

	// 頂点情報(板ポリゴンの４頂点の座標情報）
	DirectX::VertexPositionColorTexture vertex[] =
	{
		DirectX::VertexPositionColorTexture(DirectX::SimpleMath::Vector3::Zero,DirectX::SimpleMath::Vector4::One, DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
		DirectX::VertexPositionColorTexture(DirectX::SimpleMath::Vector3(0.0f,0.3f,0.0f),DirectX::SimpleMath::Vector4::One, DirectX::SimpleMath::Vector2(0.0f, 0.0f))
	};


	//シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.matView = m_view.Transpose();
	cbuff.matProj = m_proj.Transpose();
	cbuff.matWorld = m_world.Transpose();
	cbuff.Diffuse = DirectX::SimpleMath::Vector4(1.0, 0.3, 0.1, 1);
	/*Vector3 pos = vx[0].position;
	cbuff.Diffuse = Vector4(pos.x, pos.y, pos.z, 1);*/

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
	//ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	ID3D11BlendState* blendstate = m_states->Additive();

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);

	// カリングは左周り
	context->RSSetState(m_states->CullNone());

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
	//m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();

	//シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);

}

void FireShader::Draw()
{
}

void FireShader::Lost()
{
	m_CBuffer.Reset();
	m_inputLayout.Reset();
	m_batch.reset();
	m_states.reset();
	m_texture.Reset();
	m_vertexShader.Reset();
	m_pixelShader.Reset();
}




