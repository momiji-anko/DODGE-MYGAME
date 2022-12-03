#pragma once 
#include"Obstacle.h"

class NormalObstacle :public Obstacle
{

private:
	//定数
	static const float MOVE_SPEED;

	//変数


public:
	//コンストラクタ
	NormalObstacle();
	//デストラクタ
	~NormalObstacle()override;

	// 初期化
	void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
		bool active,
		float angle,
		IBehavior* behavia,
		DirectX::Model* model,
		DirectX::CommonStates* commonState)override;

	// 更新
	void Update(const DX::StepTimer& timer)override;

	// 描画
	void Draw(Camera* camera)override;

	// 終了処理
	void Finalize()override;

	//リセット
	void Reset()override;

};