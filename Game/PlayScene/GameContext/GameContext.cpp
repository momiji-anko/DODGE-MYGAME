/*
* 2023/04/28
* GameContext.cpp
* �Q�[���R���e�L�X�g
* ������
*/
#include"pch.h"
#include "GameContext.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameContext::GameContext()
	:
	m_commonState(nullptr),
	m_spriteBatcth(nullptr),
	m_keyBoardStateTracker(nullptr)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GameContext::~GameContext()
{
}

/// <summary>
/// ���Z�b�g
/// </summary>
void GameContext::Reset()
{
	m_commonState = nullptr;
	m_spriteBatcth = nullptr;
	m_keyBoardStateTracker = nullptr;
}
