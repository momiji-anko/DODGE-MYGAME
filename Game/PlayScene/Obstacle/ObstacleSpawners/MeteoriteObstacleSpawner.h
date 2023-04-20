/*
* 2023/03/21
* MeteoriteObstacleSpawner.h
* �����@��
*/
#pragma once 
#include"../../ISpawner.h"


/// <summary>
/// 覐΂̏�Q���𐶐�����X�|�i�[
/// </summary>
class MeteoriteObstacleSpawner :public ISpawner
{
public:
	
	// �R���X�g���N�^
	MeteoriteObstacleSpawner() = default;
	//�f�X�g���N�^
	~MeteoriteObstacleSpawner()override = default;

	/// <summary>
	/// 覐΂̏�Q���̐���
	/// </summary>
	/// <param name="actors">��Q���̔z��</param>
	/// <param name="position">��������ʒu</param>
	/// <param name="rotation">�p�x�i���W�A���j</param>
	/// <param name="behavior">覐΂̃r�w�C�r�A</param>
	/// <param name="model">覐΂̃��f��</param>
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