#pragma once
#include"Item.h"
#include<map>
#include"ItemSpawner.h"
#include<Model.h>
#include"Libraries/MyLibraries/Camera.h"
#include"../Stage/StageManager.h"
class ItemManeger
{
private:
	//定数
	static const int ITEM_MAX_NUM;

	//変数
	std::vector< std::unique_ptr<Actor>>                               m_items;
	std::map<Item::ItemType, std::unique_ptr<ISpawner>>                m_spawners;
	std::map < Item::ItemType, std::unique_ptr<DirectX::Model>>        m_pModel;
	DirectX::CommonStates*                                             m_commonState;


	float                                                              m_spawneTime_s;
	float                                                              m_spawneCoolTime;

	StageManeger* m_stageManeger;

public:
	//コンストラクタ
	ItemManeger();
	//デストラクタ
	~ItemManeger();

	// 初期化
	void Initialize(DirectX::CommonStates* commonState);

	// 更新
	void Update(const DX::StepTimer& timer);

	// 描画
	void Draw(Camera* camera);

	// 終了処理
	void Finalize();

	void StageHitCheck(AABBFor3D* stage);

	bool PlayerHitItem(AABBFor3D* player);
	Item::ItemType PlayerHitItemType(AABBFor3D* player);


	void StageHIt(std::vector<std::unique_ptr<Stage>>& stage);

	
	void SetShader(ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	void Shadow(ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	void SetStageManeger(StageManeger* stageManeger) { m_stageManeger = stageManeger; }

private:
	//アイテムの生成
	bool CreateItem(const DirectX::SimpleMath::Vector3& position ,Item::ItemType type,float angle);
};