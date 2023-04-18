/*
* 2023/04/09
* Actor.cpp 
* アクター
* 麻生　楓
*/
#include"pch.h"
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
	

}


DirectX::SimpleMath::Matrix Actor::CalculationWorld()
{
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

