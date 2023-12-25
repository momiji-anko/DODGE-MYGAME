/*
* 2023/03/10
* FireShader.cpp
* ファイヤーシェーダー
* 麻生　楓
*/
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


//インプットレイアウト
const std::vector<D3D11_INPUT_ELEMENT_DESC> FireShader::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/// <summary>
/// コンストラクタ
/// </summary>
FireShader::FireShader()
	:
	m_scale(1.0f),
	m_CBuffer{},
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
	m_position{},
	m_velocity{},
	m_life{},
	m_startPosition{},
	m_startVelocity{},
	m_startLife{},
	m_wait{},
	m_rot{},
	m_angle{},
	m_offsetPos{}
{
}

/// <summary>
/// デストラクタ
/// </summary>
FireShader::~FireShader()
{
	Lost();
}

/// <summary>
/// シェーダーの作成
/// </summary>
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

	//コモンステータス生成
	m_states = std::make_unique<DirectX::CommonStates>(device);

	//バッファ作成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

	//ワールド行列初期化
	m_world = DirectX::SimpleMath::Matrix::Identity;

	//角度初期化
	m_angle = 0.0f;


}

/// <summary>
/// 描画設定
/// </summary>
/// <param name="camera">カメラの座標</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
void FireShader::SetRenderState(const DirectX::SimpleMath::Vector3& camera, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	m_camera = camera;
	m_view = view;
	m_proj = proj;
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="life">寿命</param>
/// <param name="pos">座標</param>
/// <param name="velocity">移動量</param>
void FireShader::Initialize(float life, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& velocity)
{
	//初期化
	m_startPosition = pos;
	m_startVelocity = velocity;
	m_startLife = life;


	//シード値
	int seed = 100;
	//動き始める時間を決める
	m_wait = ((rand() % seed) / (float)seed) * m_startLife;

	//リセット
	Reset();
}

/// <summary>
/// リセット
/// </summary>
void FireShader::Reset()
{
	m_position = m_startPosition;
	m_velocity = m_startVelocity;
	m_life = m_startLife;
	m_scale = 0.2f;
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">更新</param>
void FireShader::Update(DX::StepTimer timer)
{
	//処理に使う秒速計(1秒で1.0f)を取得する。
	float time = static_cast<float>(timer.GetElapsedSeconds());
	//だんだん減らす
	m_wait -= time;
	//０より高ければ処理しない
	if (m_wait > 0)
	{
		return;
	}

	//横に移動する量
	DirectX::SimpleMath::Vector3 vel = DirectX::SimpleMath::Vector3(0.002f, 0, 0);

	//中心に向かう
	if (m_position.x > 0)
	{
		m_velocity.x -= vel.x;
	}
	else if (m_position.x < 0)
	{
		m_velocity.x += vel.x;
	}

	//移動
	m_position += m_velocity;

	//だんだん小さくする
	m_scale = Lerp(1.0f, 0.0f, 1.0f - m_life / m_startLife);
	
	//角度を曲げる
	m_angle += 0.1f;
	
	//寿命を減らす
	m_life -= time;

	//寿命が０より低くなれば消える
	if (m_life < 0.0f)
	{
		Reset();
	}

}

/// <summary>
/// 描画
/// </summary>
void FireShader::Render()
{
	///ビルボード化
	m_world = DirectX::SimpleMath::Matrix::Identity;

	DirectX::SimpleMath::Matrix wrold = DirectX::SimpleMath::Matrix::Identity;
	//ビルボード
	wrold = DirectX::SimpleMath::Matrix::CreateBillboard(m_position + m_offsetPos, m_camera, DirectX::SimpleMath::Vector3::UnitY);

	//左座標系だから反転させる用行列
	DirectX::SimpleMath::Matrix revMat = DirectX::SimpleMath::Matrix::Identity;
	revMat._11 = -1.0f;
	revMat._33 = -1.0f;


	DirectX::SimpleMath::Matrix scaleMat, rotMat;
	//拡大率
	scaleMat = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
	//回転
	rotMat = DirectX::SimpleMath::Matrix::CreateRotationZ(m_angle);

	//ワールド行列計算
	m_world = scaleMat * rotMat * revMat * wrold;

	//シャドウ表示
	ShaderDraw();
}

/// <summary>
/// シェーダー描画
/// </summary>
void FireShader::ShaderDraw()
{
	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//コンテキスト取得
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

/// <summary>
/// 消去
/// </summary>
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




