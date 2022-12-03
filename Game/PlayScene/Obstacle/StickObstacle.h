#pragma once 
#include"Obstacle.h"

class StickObstacle :public Obstacle
{

private:
	//�萔
	static const float MOVE_SPEED;

	//�ϐ�


public :
	//�R���X�g���N�^
	StickObstacle();
	//�f�X�g���N�^
	 ~StickObstacle()override;

	// ������
	 void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
		bool active,
		float angle,
		IBehavior* behavia,
		DirectX::Model* model,
		DirectX::CommonStates* commonState)override;

	// �X�V
	 void Update(const DX::StepTimer& timer)override;

	// �`��
	 void Draw(Camera* camera)override;

	// �I������
	 void Finalize()override;

	//���Z�b�g
	 void Reset()override;

};