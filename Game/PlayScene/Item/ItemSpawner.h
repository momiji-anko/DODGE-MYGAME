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
	/// <param name="rotation">�p�x�i���W�A���j</param>
	/// <param name="behavior">NULL��OK</param>
	/// <param name="model">�A�C�e���̃��f��</param>
	/// <returns>true�����������@false���������s</returns>
	bool Create( std::vector< std::unique_ptr<Actor>>& actors,
		const DirectX::SimpleMath::Vector3 & position,
		const DirectX::SimpleMath::Vector3& rotation,
		IBehavior * behavior,
		DirectX::Model* model
	) override;
};

