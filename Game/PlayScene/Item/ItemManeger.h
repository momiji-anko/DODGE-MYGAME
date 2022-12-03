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
	//�萔
	static const int ITEM_MAX_NUM;

	//�ϐ�
	std::vector< std::unique_ptr<Actor>>                               m_items;
	std::map<Item::ItemType, std::unique_ptr<ISpawner>>                m_spawners;
	std::map < Item::ItemType, std::unique_ptr<DirectX::Model>>        m_pModel;
	DirectX::CommonStates*                                             m_commonState;


	float                                                              m_spawneTime_s;
	float                                                              m_spawneCoolTime;

	StageManeger* m_stageManeger;

public:
	//�R���X�g���N�^
	ItemManeger();
	//�f�X�g���N�^
	~ItemManeger();

	// ������
	void Initialize(DirectX::CommonStates* commonState);

	// �X�V
	void Update(const DX::StepTimer& timer);

	// �`��
	void Draw(Camera* camera);

	// �I������
	void Finalize();

	void StageHitCheck(AABBFor3D* stage);

	bool PlayerHitItem(AABBFor3D* player);
	Item::ItemType PlayerHitItemType(AABBFor3D* player);


	void StageHIt(std::vector<std::unique_ptr<Stage>>& stage);

	
	void SetShader(ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	void Shadow(ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	void SetStageManeger(StageManeger* stageManeger) { m_stageManeger = stageManeger; }

private:
	//�A�C�e���̐���
	bool CreateItem(const DirectX::SimpleMath::Vector3& position ,Item::ItemType type,float angle);
};