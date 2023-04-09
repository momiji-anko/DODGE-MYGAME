/*
* 2023/04/09
* Actor.cpp 
* アクター
* 麻生　楓
*/
#include"pch.h"
#include"Actor.h"

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

