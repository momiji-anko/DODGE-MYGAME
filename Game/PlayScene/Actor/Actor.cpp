/*
* 2023/04/09
* Actor.cpp 
* アクター
* 麻生　楓
*/
#include"pch.h"
#include"Game/PlayScene/Item/Item.h"
#include"DeviceResources.h"
#include"Actor.h"
#include"Game/PlayScene/GameContext/GameContext.h"

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
	m_AABBObject(nullptr)
{
	CreateAABB();
	CreateCapsule();

}

/// <summary>
/// ワールド行列の計算
/// </summary>
/// <returns>ワールド行列</returns>
DirectX::SimpleMath::Matrix Actor::CalculateWorld()
{
	//ワールド行列に単位行列を入れる
	m_world = DirectX::SimpleMath::Matrix::Identity;
	//移動
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	//回転
	DirectX::SimpleMath::Matrix rotation = DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_rotation);
	//拡縮
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
	//ワールド行列の計算
	m_world *= scale * rotation * trans;

	return m_world;
}

/// <summary>
/// 影生成
/// </summary>
/// <param name="shadow">シャドウマップの生ポインタ</param>
/// <param name="view">ビュー行列</param>
/// <param name="projection">プロジェクション行列</param>
void Actor::CreateShadow(ShadowMap* shadow, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスコンテキスト取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//モデルがあれば影を生成する
	if (GetModel() != nullptr)
	{
		//ワールド行列を計算する
		CalculateWorld();

		//影生成
		GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorld(), view, projection, false, [&]()
			{
				shadow->DrawShadowMap(context);
			}
		);
	}

}


/// <summary>
/// AABBの作成
/// </summary>
void Actor::CreateAABB()
{
	//AABB当たり判定作成
	m_AABBObject = std::make_unique<AABBFor3D>();
	//AABB初期化
	m_AABBObject->Initialize();

	DirectX::SimpleMath::Vector3 length = { 0.5f,0.5f,0.5f };

	//AABBの当たり判定を設定
	m_AABBObject->SetData(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);


}

/// <summary>
/// カプセルの生成
/// </summary>
void Actor::CreateCapsule()
{
	//カプセルの作成
	m_capsule = std::make_unique<Capsule>();
	//半径設定
	m_capsule->r = 1.0f;
}

