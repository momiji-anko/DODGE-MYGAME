/*
* 2023/03/21
* MeanderingObstacleSpawner.h
* �����@��
*/
#pragma once 
#include"../../ISpawner.h"

/// <summary>
/// �֍s���鉊�̏�Q���X�|�i�[
/// </summary>
class MeanderingObstacleSpawner :public ISpawner
{
public:
	MeanderingObstacleSpawner() = default;
	~MeanderingObstacleSpawner()override = default;

	/// <summary>
	/// �֍s���鉊�̏�Q���̐���
	/// </summary>
	/// <param name="actors">��Q���̔z��</param>
	/// <param name="position">��������ʒu</param>
	/// <param name="rotation">�p�x�i���W�A���j</param>
	/// <param name="behavior">�֍s���鉊�̃r�w�C�r�A</param>
	/// <param name="model">���̃��f���iNULL��OK�j</param>
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