#pragma once
#include"Stage.h"
#include"Game/GameMain.h"
#include <CommonStates.h>
#include<SimpleMath.h>
#include<Model.h>
#include <string>


class StageManeger
{
public:

private:

	std::unique_ptr<DirectX::Model>           m_stageModel;

	std::vector<DirectX::SimpleMath::Vector3> m_stagePositions;

	std::vector<std::unique_ptr<Stage>>       m_stage;

	DirectX::CommonStates*                    m_commonState;
	
	StageSelect m_stageSelect;

	std::vector<std::string> m_stageData;

	float m_time_s;
	std::vector<bool> m_flag;
public:

	StageManeger();
	~StageManeger();

	// 初期化
	void Initialize(DirectX::CommonStates* commonState, StageSelect stage = StageSelect::Stage1);

	// 更新
	void Update(const DX::StepTimer& timer);

	// 描画
	void Draw(Camera* camera);

	// 終了処理
	void Finalize();

	bool PlayerStageAABBHitCheck(Actor* player);

	bool ItemHitCheck(AABBFor3D* itemAABB);

	bool LoadGraphDataByJSON(const std::wstring& fileName);

	void ParseJSON();


	void SetShadow(ShadowMap* shadow);

};