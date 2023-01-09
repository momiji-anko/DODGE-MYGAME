#include"pch.h"
#include"ItemManeger.h"
#include"../MyRandom.h"
#include"DeviceResources.h"
const int ItemManeger::ITEM_MAX_NUM = 30;

//コンストラクタ
ItemManeger::ItemManeger()
	:
	m_items{},
	m_spawners{},
	m_pModel{},
	m_commonState(nullptr),
	m_stageManeger(nullptr)
{
}

//デストラクタ
ItemManeger::~ItemManeger()
{

}

// 初期化
void ItemManeger::Initialize(DirectX::CommonStates* commonState)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	m_commonState = commonState;
	m_items.resize(ITEM_MAX_NUM);

	for (std::unique_ptr<Actor>& item : m_items)
	{
		item = std::make_unique<Item>();
		
	}

	m_spawners[Item::ItemType::INVINCIBLE_ITEM] = std::make_unique<ItemSpawner>();
	m_spawners[Item::ItemType::SLIP_ITEM] = std::make_unique<SlipItemSpawner>();

	//	エフェクトファクトリの作成
	DirectX::EffectFactory* factory = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//	テクスチャの読み込みパス指定
	factory->SetDirectory(L"Resources/Models");

	//	ファイルを指定してモデルデータ読み込み
	m_pModel[Item::ItemType::INVINCIBLE_ITEM] = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/shield.cmo",
		*factory
	);
	//	ファイルを指定してモデルデータ読み込み
	m_pModel[Item::ItemType::SLIP_ITEM] = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/star.cmo",
		*factory
	);

	delete factory;
	//CreateItem(DirectX::SimpleMath::Vector3(MyRandom::GetFloatRange(-7.0f, 6.0f), 10.0f, MyRandom::GetFloatRange(-7.0f, 7.0f)), Item::ItemType::INVINCIBLE_ITEM, 0.0f);

	m_spawneTime_s=15.0f;
	m_spawneCoolTime=15.0f;
}

// 更新
void ItemManeger::Update(const DX::StepTimer& timer)
{
	float time = timer.GetElapsedSeconds();
	m_spawneTime_s -= time;
	if (m_spawneTime_s <= 0.0f)
	{
		
		m_spawneTime_s = m_spawneCoolTime;
		
		CreateItem(DirectX::SimpleMath::Vector3(MyRandom::GetFloatRange(-7.0f, 6.0f), 15.0f, MyRandom::GetFloatRange(-7.0f, 7.0f)), Item::ItemType::INVINCIBLE_ITEM, 0.0f);
	
	}


	for (std::unique_ptr<Actor>& item : m_items)
	{
		if (item->IsActive() == false)
			continue;

		item->Update(timer);
		
	}


	for (std::unique_ptr<Actor>& item : m_items)
	{
		if (item->IsActive() == false)
			continue;
		
		Item* i = dynamic_cast<Item*>(item.get());

		bool isHit = m_stageManeger->ItemHitCheck(item.get());

		i->SetGruondHit(isHit);
		
	}

}

// 描画
void ItemManeger::Draw(Camera* camera)
{

	for (std::unique_ptr<Actor>& item : m_items)
	{
		if (item->IsActive() == false)
			continue;

		item->Draw(camera);

	}
}

// 終了処理
void ItemManeger::Finalize()
{
}

void ItemManeger::StageHitCheck(AABBFor3D* stage)
{

	for (std::unique_ptr<Actor>& item : m_items)
	{
		Item* i = dynamic_cast<Item*>(item.get());
		if (item->IsActive() == false)
			continue;


		if (i!=nullptr)
		{
			if (i->GetAABB()->HitCheck(stage))
			{
				i->SetGruondHit(true);
				break;
			}
			else
			{
				i->SetGruondHit(false);
			}
		}

	}
}

bool ItemManeger::PlayerHitItem(AABBFor3D* player)
{

	for (std::unique_ptr<Actor>& item : m_items)
	{
		if (item->IsActive() == false)
			continue;

		if (item->GetAABB()->HitCheck(player))
		{
			Item* i = dynamic_cast<Item*>(item.get());
			if (i != nullptr)
			{
				i->Reset();

			}
			return true;
		}

	}

	return false;
}

Item::ItemType ItemManeger::PlayerHitItemType(AABBFor3D* player)
{
	for (std::unique_ptr<Actor>& item : m_items)
	{
		if (item->IsActive() == false)
			continue;

		if (item->GetAABB()->HitCheck(player))
		{
			Item* i = dynamic_cast<Item*>(item.get());
			if (i != nullptr)
			{
				i->Reset();

				return i->GetItemType();

			}
		}	

	}

	return Item::ItemType::NONE;
}

void ItemManeger::StageHIt(std::vector<std::unique_ptr<Stage>>& stage)
{
	

	for (std::unique_ptr<Actor>& item : m_items)
	{
		Item* i = dynamic_cast<Item*>(item.get());
		if (item->IsActive() == false)
			continue;

		for (std::unique_ptr<Stage >& s : stage)
		{
			if (i != nullptr)
			{
				if (i->GetAABB()->HitCheck(s->GetAABB()))
				{
					i->SetGruondHit(true);
					break;
				}
				else
				{
					i->SetGruondHit(false);
				}
			}
		}
	}




}

void ItemManeger::SetShader(ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{

	for (std::unique_ptr<Actor>& item : m_items)
	{
		Item* i = dynamic_cast<Item*>(item.get());
		i->Set(shadowMap);
	}

}

void ItemManeger::Shadow(ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	for (std::unique_ptr<Actor>& item : m_items)
	{
		if (item->IsActive() == false)
			continue;

		Item* i = dynamic_cast<Item*>(item.get());
		i->Shadow(shadowMap, view, projection);
	}
}

//アイテムの生成
bool ItemManeger::CreateItem(const DirectX::SimpleMath::Vector3& position, Item::ItemType type, float angle)
{
	return m_spawners[type]->Create(m_items, position, angle, nullptr, m_pModel[type].get(), m_commonState);
}
