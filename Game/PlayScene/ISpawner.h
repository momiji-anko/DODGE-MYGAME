#pragma once
class Actor;
class IBehavior;
#include<vector>
#include<SimpleMath.h>
#include<Model.h>
#include<CommonStates.h>
class ISpawner
{
public:
	ISpawner() = default;
	virtual ~ISpawner() = default;
	virtual bool Create( std::vector< std::unique_ptr<Actor>>& actors,
		 const DirectX::SimpleMath::Vector3& position,
		  const float angle,
		  IBehavior* behavior,
		DirectX::Model* model,
		DirectX::CommonStates* commonState
	) = 0;
};