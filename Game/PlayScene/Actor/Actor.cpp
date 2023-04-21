/*
* 2023/04/09
* Actor.cpp 
* アクター
* 麻生　楓
*/
#include"pch.h"
#include"DeviceResources.h"
#include"Actor.h"

/// <summary>
/// コンストラクタ
/// </summary>
Actor::Actor():
	m_position(DirectX::SimpleMath::Vector3::Zero),
	m_velocity(DirectX::SimpleMath::Vector3::Zero),
	m_rotation(DirectX::SimpleMath::Vector3::Zero),
	m_active(false),
	m_angle(0.0f),
	m_behavia(nullptr),
	m_pModel(nullptr),
	m_commonState(nullptr),
	m_AABBObject(nullptr)
{
	CreateAABB();
	CreateCapsule();

}


DirectX::SimpleMath::Matrix Actor::CalculationWorld()
{
	//ワールド行列に単位行列を入れる
	m_world = DirectX::SimpleMath::Matrix::Identity;
	//移動
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	//回転
	DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_rotation);
	//拡縮
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
	//ワールド行列の計算
	m_world *= scale * rot * trans;

	return m_world;
}

void Actor::CreateShadow(ShadowMap* shadow, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスコンテキスト取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//モデルがあれば影を生成する
	if (GetModel() != nullptr)
	{
		//ワールド行列を計算する
		CalculationWorld();

		//影生成
		GetModel()->Draw(context, *GetCommonState(), GetWorld(), view, projection, false, [&]()
			{
				shadow->DrawShadowMap(context);
			}
		);
	}

}

void Actor::CreateAABB()
{
	//AABB当たり判定作成
	m_AABBObject = std::make_unique<AABBFor3D>();
	//AABB初期化
	m_AABBObject->Initialize();

	DirectX::SimpleMath::Vector3 length = { 0.5f,0.5f,0.5f };

	//AABBの当たり判定を設定
	m_AABBObject->SetData(m_position + length, m_position - length);


}

void Actor::CreateCapsule()
{
	//カプセルの作成
	m_capsule = std::make_unique<Capsule>();
	//半径設定
	m_capsule->r = 0.5f;
}

