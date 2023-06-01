/*
* AABBFor3D.cpp
* 
* 
* 
* 
* 
*/
#include"pch.h"

#include"AABBFor3D.h"
#include"DeviceResources.h"


//コンストラクタ
AABBFor3D::AABBFor3D():
	m_center{},
	m_r{}
{

}

//デストラクタ
AABBFor3D::~AABBFor3D()
{

}

//初期化
void AABBFor3D::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//コモンステート::D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);
	
	//ベーシックエフェクトの作成
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);

	//入力レイアウトを作成するための準備
	const void* shaderByteCode;
	size_t byteCodeLength;

	//頂点情報を決定するために必要な情報
	m_basicEffect->SetLightingEnabled(false);	//ライトOFF
	m_basicEffect->SetTextureEnabled(false);	//テクスチャOFF
	m_basicEffect->SetVertexColorEnabled(true);	//頂点カラーON

	//使用する頂点シェーダー情報の取得
	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	//入力レイアウトの作成
	device->CreateInputLayout(
		DirectX::VertexPositionColor::InputElements,
		DirectX::VertexPositionColor::InputElementCount,
		shaderByteCode,
		byteCodeLength,
		m_inputLayout.ReleaseAndGetAddressOf()
	);
	
	//プリミティブバッチの作成
	m_primitiveBach = 
		std::make_unique< DirectX::PrimitiveBatch < DirectX::VertexPositionColor>>(context);

	m_center = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	m_r = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
}

/*
	AABB当たり判定のボックスをデバック表示する
   
   引数
	DirectX::SimpleMath::Matrix world
	親のワールドを指定する
	通常は「DirectX::SimpleMath::Matrix::Identity」を指定する
	DirectX::SimpleMath::Matrix view
	カメラで算出されたviewを指定する
	DirectX::SimpleMath::Matrix projection
	射影行列を設定したprojectionを設定する
	DirectX::XMFLOAT4 color
	描画をする時の色を指定する

	戻り値　void
*/
void AABBFor3D::Draw(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection, const DirectX::XMFLOAT4& color)
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//デバイスコンテキストの取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//エフェクトに各種変換行列の設定ー＞設定がまずいと見た目が正しくならない
	m_basicEffect->SetWorld(world);				//ワールド行列を設定
	m_basicEffect->SetView(view);				//ビュー行列を設定
	m_basicEffect->SetProjection(projection);	//射影行列の設定
	m_basicEffect->Apply(context);				

	//入力アセンブラステージに入力レイアウトを設定
	context->IASetInputLayout(m_inputLayout.Get());

	//ラインでボックスを描画する
	m_primitiveBach->Begin();
	DrawBox(color);
	m_primitiveBach->End();
}

/*
	AABBの当たり判定用のボックスの情報を更新する

	引数
	DirectX::SimpleMath::Vector3 minPos
	一番小さいX座標、一番小さいY座標、一番小さいZ座標
	DirectX::SimpleMath::Vector3 maxPos
	一番大きいX座標、一番大きいXY座標、一番大きいZ座標

	戻り値 void
*/
void AABBFor3D::SetData(const DirectX::SimpleMath::Vector3& minPos, const DirectX::SimpleMath::Vector3& maxPos)
{
	//ボックスの中心点を計算する
	m_center.x = (minPos.x + maxPos.x) / 2.0f;
	m_center.y = (minPos.y + maxPos.y) / 2.0f;
	m_center.z = (minPos.z + maxPos.z) / 2.0f;

	//各辺と中心点との距離を計算する
	m_r.x = maxPos.x - m_center.x;
	m_r.y = maxPos.y - m_center.y;
	m_r.z = maxPos.z - m_center.z;
}

/*
	当たり判定を行う

	引数
	AABBFor3D* other
	当たり判定の相手となるAABBオブジェクト

	戻り値 bool 
	当たっている true
	当っていない false
*/
bool AABBFor3D::HitCheck(AABBFor3D* other)
{
	//中心点同士の距離を測る
	DirectX::SimpleMath::Vector3 centerLength = other->m_center - this->m_center;
	DirectX::SimpleMath::Vector3 lineLength = DirectX::SimpleMath::Vector3(
														this->m_r.x + other->m_r.x,
														this->m_r.y + other->m_r.y,
														this->m_r.z + other->m_r.z);

	//中心点同士の距離と距離合計の和を比べる
	if (fabsf(centerLength.x) <= lineLength.x &&
		fabsf(centerLength.y) <= lineLength.y &&
		fabsf(centerLength.z) <= lineLength.z)
	{
		//中心点同士の距離のほうが小さければ、当てっている
		return true;
	}

	//当てっていない
	return false;
}

/*
	内部用の、ボックスを描画する関数
	引数
	DirectX::XMFLOAT4
	ボックスを描画するための色

	戻り値　void
*/
void AABBFor3D::DrawBox(const DirectX::XMFLOAT4& color)
{
	DirectX::VertexPositionColor lineList[24];
	DirectX::SimpleMath::Vector3 minPos;
	DirectX::SimpleMath::Vector3 maxPos;
	DirectX::SimpleMath::Vector3 tmp;

	//
	for (int i = 0; i < 24; i++)
	{
		lineList[i].color = color;
	}

	//最小座標と最大座標を計算する
	minPos.x = m_center.x - m_r.x;
	minPos.y = m_center.y - m_r.y;
	minPos.z = m_center.z - m_r.z;

	maxPos.x = m_center.x + m_r.x;
	maxPos.y = m_center.y + m_r.y;
	maxPos.z = m_center.z + m_r.z;

	lineList[0].position = minPos;
	lineList[1].position = minPos + DirectX::SimpleMath::Vector3(m_r.x * 2.0f, 0.0f, 0.0f);

	lineList[2].position = minPos;
	lineList[3].position = minPos + DirectX::SimpleMath::Vector3( 0.0f, m_r.y * 2.0f, 0.0f);

	lineList[4].position = minPos;
	lineList[5].position = minPos + DirectX::SimpleMath::Vector3( 0.0f, 0.0f, m_r.z * 2.0f);

	
	lineList[6].position = maxPos;
	lineList[7].position = maxPos - DirectX::SimpleMath::Vector3(m_r.x * 2.0f, 0.0f, 0.0f);

	lineList[8].position = maxPos;
	lineList[9].position = maxPos - DirectX::SimpleMath::Vector3(0.0f, m_r.y * 2.0f, 0.0f);

	lineList[10].position = maxPos;
	lineList[11].position = maxPos - DirectX::SimpleMath::Vector3( 0.0f, 0.0f, m_r.z * 2.0f);

	
	tmp.x = minPos.x;
	tmp.y = maxPos.y;
	tmp.z = maxPos.z;
	lineList[12].position = tmp;
	lineList[13].position = tmp - DirectX::SimpleMath::Vector3( 0.0f, 0.0f, m_r.z * 2.0f);

	lineList[14].position = tmp;
	lineList[15].position = tmp - DirectX::SimpleMath::Vector3( 0.0f, m_r.y * 2.0f, 0.0f);

	
	tmp.x = maxPos.x;
	tmp.y = maxPos.y;
	tmp.z = minPos.z;
	lineList[16].position = tmp;
	lineList[17].position = tmp - DirectX::SimpleMath::Vector3(m_r.x * 2.0f, 0.0f, 0.0f);

	lineList[18].position = tmp;
	lineList[19].position = tmp - DirectX::SimpleMath::Vector3(0.0f, m_r.y * 2.0f, 0.0f);

	//Yが最小値、ZXが最大値の点から2本
	tmp.x = maxPos.x;
	tmp.y = minPos.y;
	tmp.z = maxPos.z;
	lineList[20].position = tmp;
	lineList[21].position = tmp - DirectX::SimpleMath::Vector3(m_r.x * 2.0f, 0.0f, 0.0f);

	lineList[22].position = tmp;
	lineList[23].position = tmp - DirectX::SimpleMath::Vector3(0.0f, 0.0f, m_r.z * 2.0f);

	//ラインリストを渡して描画してもらう
	m_primitiveBach->Draw(D3D_PRIMITIVE_TOPOLOGY_LINELIST, lineList, 24);

}
