#include"pch.h"
#include"ItemManager.h"
#include"../MyRandom.h"
#include"DeviceResources.h"
const int ItemManager::ITEM_MAX_NUM = 30;

//�R���X�g���N�^
ItemManager::ItemManager()
	:
	m_items{},
	m_spawners{},
	m_pModel{},
	m_commonState(nullptr),
	m_stageManeger(nullptr)
{
}

//�f�X�g���N�^
ItemManager::~ItemManager()
{

}

// ������
void ItemManager::Initialize(DirectX::CommonStates* commonState)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	m_commonState = commonState;
	m_items.resize(ITEM_MAX_NUM);

	for (std::unique_ptr<Actor>& item : m_items)
	{
		item = std::make_unique<Item>();
		
	}

	m_spawners[Item::ItemType::SHIELD_ITEM] = std::make_unique<ItemSpawner>();

	//	�G�t�F�N�g�t�@�N�g���̍쐬
	DirectX::EffectFactory* factory = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//	�e�N�X�`���̓ǂݍ��݃p�X�w��
	factory->SetDirectory(L"Resources/Models");

	//	�t�@�C�����w�肵�ă��f���f�[�^�ǂݍ���
	m_pModel[Item::ItemType::SHIELD_ITEM] = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/shield.cmo",
		*factory
	);
	delete factory;



	m_spawneTime_s=15.0f;
	m_spawneCoolTime=15.0f;
}

// �X�V
void ItemManager::Update(const DX::StepTimer& timer)
{
	float time = timer.GetElapsedSeconds();
	m_spawneTime_s -= time;
	if (m_spawneTime_s <= 0.0f)
	{
		
		m_spawneTime_s = m_spawneCoolTime;
		
		CreateItem(DirectX::SimpleMath::Vector3(MyRandom::GetFloatRange(-7.0f, 6.0f), 15.0f, MyRandom::GetFloatRange(-7.0f, 7.0f)), Item::ItemType::SHIELD_ITEM, 0.0f);
	
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

// �`��
void ItemManager::Draw(Camera* camera)
{

	for (std::unique_ptr<Actor>& item : m_items)
	{
		if (item->IsActive() == false)
			continue;

		item->Draw(camera);

	}
}

// �I������
void ItemManager::Finalize()
{
}

void ItemManager::StageHitCheck(AABBFor3D* stage)
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

bool ItemManager::PlayerHitItem(AABBFor3D* player)
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

Item::ItemType ItemManager::PlayerHitItemType(AABBFor3D* player)
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

void ItemManager::StageHIt(std::vector<std::unique_ptr<Stage>>& stage)
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

void ItemManager::SetShader(ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{

	for (std::unique_ptr<Actor>& item : m_items)
	{
		Item* i = dynamic_cast<Item*>(item.get());
		i->Set(shadowMap);
	}

}

void ItemManager::Shadow(ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	for (std::unique_ptr<Actor>& item : m_items)
	{
		if (item->IsActive() == false)
			continue;

		Item* i = dynamic_cast<Item*>(item.get());
		i->Shadow(shadowMap, view, projection);
	}
}

//�A�C�e���̐���
bool ItemManager::CreateItem(const DirectX::SimpleMath::Vector3& position, Item::ItemType type, float angle)
{
	return m_spawners[type]->Create(m_items, position, angle, nullptr, m_pModel[type].get(), m_commonState);
}
