/*
* 2023/03/21
* ReverseRotateStickObstacleSpawner.h
* �����@��
*/
#pragma once 
#include"../../ISpawner.h"

/// <summary>
/// �����v���ɉ�]����_�̏�Q���̃X�|�i�[
/// </summary>
class ReverseRotateStickObstacleSpawner :public ISpawner
{
public:
	//�R���X�g���N�^
	ReverseRotateStickObstacleSpawner() = default;
	//�f�X�g���N�^
	~ReverseRotateStickObstacleSpawner()override = default;
	/// <summary>
	/// ��]����_�̏�Q���̐���
	/// </summary>
	/// <param name="actors">��Q���̔z��</param>
	/// <param name="position">��������ʒu</param>
	/// <param name="rotation">�p�x�i���W�A���j</param>
	/// <param name="behavior">��]����_�̃r�w�C�r�A</param>
	/// <param name="model">��]����_�̃��f��</param>
	/// <param name="commonState">�R�����X�e�[�g</param>
	/// <returns>true=���������@false=�������s</returns>
	bool Create(std::vector< std::unique_ptr<Actor>>& actors,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& rotation,
		IBehavior* behavior,
		DirectX::Model* model,
		DirectX::CommonStates* commonState
	) override;
};
