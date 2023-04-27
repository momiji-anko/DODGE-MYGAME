/*
* 2023/04/11
* IBehavior.h
* �r�w�C�r�A�[�̃C���^�[�t�F�[�X�N���X
* �����@��
*/
#pragma once
#include"StepTimer.h"
#include<vector>

//�O���錾
class Actor;

/// <summary>
/// �r�w�C�r�A�[�̃C���^�[�t�F�[�X
/// </summary>
class IBehavior
{
public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual~IBehavior() = default;

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <param name="actor">�A�N�^�[</param>
	virtual void Execute(const DX::StepTimer& timer, Actor* actor) = 0;

};