/*
* 2023/04/28
* GameContext.cpp
* ゲームコンテキスト
* 麻生楓
*/
#include"pch.h"
#include "GameContext.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameContext::GameContext()
	:
	m_commonState(nullptr),
	m_spriteBatcth(nullptr),
	m_keyBoardStateTracker(nullptr)
{
}

/// <summary>
/// デストラクタ
/// </summary>
GameContext::~GameContext()
{
}

/// <summary>
/// リセット
/// </summary>
void GameContext::Reset()
{
	m_commonState = nullptr;
	m_spriteBatcth = nullptr;
	m_keyBoardStateTracker = nullptr;
}
