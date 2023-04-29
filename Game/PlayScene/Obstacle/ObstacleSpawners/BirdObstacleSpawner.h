/*
* 2023/03/21
* BirdObstacleSpawner.h
* �����@��
*/
#pragma once 
#include"../../ISpawner.h"

/// <summary>
/// ���̏�Q���̃X�|�i�[
/// </summary>
class BirdObstacleSpawner :public ISpawner
{
public:
	//�R���X�g���N�^
	BirdObstacleSpawner() = default;
	//�f�X�g���N�^
	~BirdObstacleSpawner()override = default;
	/// <summary>
	/// ���̏�Q���̐���
	/// </summary>
	/// <param name="actors">��Q���̔z��</param>
	/// <param name="position">��������ʒu</param>
	/// <param name="rotation">�p�x�i���W�A���j</param>
	/// <param name="behavior">���̃r�w�C�r�A</param>
	/// <param name="model">���̃��f��</param>
	/// <returns>true=���������@false=�������s</returns>
	bool Create(std::vector< std::unique_ptr<Actor>>& actors,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& rotation,
		IBehavior* behavior,
		DirectX::Model* model
	) override;
};