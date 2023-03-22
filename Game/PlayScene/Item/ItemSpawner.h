#pragma once 
#include"../ISpawner.h"

class ItemSpawner :public ISpawner
{
public:
	ItemSpawner() = default;
	 ~ItemSpawner()override = default;
	/// <summary>
	/// ���̃A�C�e���̃X�|�i�[
	/// </summary>
	/// <param name="actors">�A�C�e���̔z��</param>
	/// <param name="position">�A�C�e���𐶐�������W</param>
	/// <param name="angle">�p�x�i���W�A���j</param>
	/// <param name="behavior">NULL��OK</param>
	/// <param name="model">�A�C�e���̃��f��</param>
	/// <param name="commonState">�R�����X�e�[�g</param>
	/// <returns>true�����������@false���������s</returns>
	bool Create( std::vector< std::unique_ptr<Actor>>& actors,
		const DirectX::SimpleMath::Vector3 & position,
		const float angle,
		IBehavior * behavior,
		DirectX::Model* model,
		DirectX::CommonStates* commonState
	) override;
};

