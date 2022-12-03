#pragma once 
#include"../ISpawner.h"

class ItemSpawner :public ISpawner
{
public:
	ItemSpawner() = default;
	 ~ItemSpawner()override = default;
	bool Create( std::vector< std::unique_ptr<Actor>>& actors,
		const DirectX::SimpleMath::Vector3 & position,
		const float angle,
		IBehavior * behavior,
		DirectX::Model* model,
		DirectX::CommonStates* commonState
	) override;
};

class SlipItemSpawner :public ISpawner
{
public:
	SlipItemSpawner() = default;
	~SlipItemSpawner()override = default;
	bool Create(std::vector< std::unique_ptr<Actor>>& actors,
		const DirectX::SimpleMath::Vector3& position,
		const float angle,
		IBehavior* behavior,
		DirectX::Model* model,
		DirectX::CommonStates* commonState
	) override;
};