/*
* 2023/04/11
* ISpawner.h
* �X�|�i�[�̃C���^�[�t�F�[�X�N���X
* �����@��
*/
#pragma once
#include<vector>
#include<SimpleMath.h>
#include<Model.h>
#include<CommonStates.h>
class Actor;
class IBehavior;

/// <summary>
/// �X�|�i�[�̃C���^�[�t�F�[�X�N���X
/// </summary>
class ISpawner
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ISpawner() = default;
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ISpawner() = default;
	/// <summary>
	/// �I�u�W�F�N�g�̐���
	/// </summary>
	/// <param name="actors">�A�N�^�[</param>
	/// <param name="position">���W</param>
	/// <param name="rotation">�p�x</param>
	/// <param name="behavior">�r�w�C�r�A�[</param>
	/// <param name="model">���f��</param>
	/// <param name="commonState">�R�����X�e�[�g</param>
	/// <returns>true=��������, false=�������s</returns>
	virtual bool Create( std::vector< std::unique_ptr<Actor>>& actors,
		 const DirectX::SimpleMath::Vector3& position,
		  const DirectX::SimpleMath::Vector3& rotation,
		  IBehavior* behavior,
		DirectX::Model* model
	) = 0;
};