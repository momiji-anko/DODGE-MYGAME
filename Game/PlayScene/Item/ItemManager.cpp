/*
* 2023/04/15
* ItemManager.cpp
* �A�C�e���̊Ǘ��N���X
* �����@��
*/
#include"pch.h"
#include"ItemManager.h"
#include"../MyRandom.h"
#include"DeviceResources.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"../Stage/StageManager.h"
#include"Game/PlayScene/GameContext/GameContext.h"

//�A�C�e���̍ő吔
const int ItemManager::ITEM_MAX_NUM = 30;

//�A�C�e���̃X�|�[���N�[���^�C��
const float ItemManager::ITEM_SPAWNE_COOL_TIME_S = 15.0f;

//�A�C�e���̃X�|�[���͈�
const float ItemManager::ITEM_SPAWNE_EREA = 6.0f;
//�A�C�e���̃X�|�[�����鍂��
const float ItemManager::ITEM_SPAWNE_HEIGHT = 15.0f;



/// <summary>
/// �R���X�g���N�^
/// </summary>
ItemManager::ItemManager()
	:
	m_items{},
	m_spawners{},
	m_pModel{},
	m_stageManeger(nullptr),
	m_spawneTime_s(0)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ItemManager::~ItemManager()
{

}

/// <summary>
/// ������
/// </summary>
void ItemManager::Initialize()
{

	
	//�A�C�e���̔z����A�C�e���̍ő吔�Ń��T�C�Y����
	m_items.resize(ITEM_MAX_NUM);

	//�A�C�e���̐���
	for (std::unique_ptr<Actor>& item : m_items)
	{
		item = std::make_unique<Item>();
	}

	//�A�C�e���̃X�|�i�[�쐬
	m_spawners[Item::ItemType::SHIELD_ITEM] = std::make_unique<ItemSpawner>();

	//�A�C�e���̃��f������
	m_pModel[Item::ItemType::SHIELD_ITEM] = ModelManager::GetInstance().LoadModel(L"Resources/Models/shield.cmo");


	//�X�|�[���^�C��������
	m_spawneTime_s = ITEM_SPAWNE_COOL_TIME_S;
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void ItemManager::Update(const DX::StepTimer& timer)
{
	//�o�ߎ���
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	//�v���C���[���S���Ă���΍X�V���Ȃ�
	if (GameContext::GetInstance().GetIsPlayerDeath())
		return;

	//�X�|�[���^�C�����o�ߎ��Ԃň���
	m_spawneTime_s -= elapsedTime;

	//�X�|�[���^�C�����O�ɂȂ�����A�C�e���𐶐�����
	if (m_spawneTime_s <= 0.0f)
	{
		//�X�|�[���^�C��������������
		m_spawneTime_s = ITEM_SPAWNE_COOL_TIME_S;

		DirectX::SimpleMath::Vector3 createPosition = DirectX::SimpleMath::Vector3(MyRandom::GetFloatRange(-ITEM_SPAWNE_EREA, ITEM_SPAWNE_EREA), ITEM_SPAWNE_HEIGHT, MyRandom::GetFloatRange(-ITEM_SPAWNE_EREA, ITEM_SPAWNE_EREA));

		//�A�C�e���𐶐�����
		CreateItem(createPosition, Item::ItemType::SHIELD_ITEM, DirectX::SimpleMath::Vector3(0.0f, DirectX::XM_PI, 0.0f));
	
	}

	//�A�C�e���̍X�V
	for (std::unique_ptr<Actor>&item : m_items)
	{
		//�A�N�e�B�u��ԂłȂ���΍X�V���Ȃ�
		if (item->IsActive() == false) 
		{
			continue; 
		}

		//�X�V
		item->Update(timer);
		
	}

	// �A�C�e���ƃX�e�[�W���������Ă��邩����
	ItemToStageCheckHit();
}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̐��|�C���^</param>
void ItemManager::Draw(Camera* camera)
{
	//�A�C�e���̕`��
	for (std::unique_ptr<Actor>&item : m_items)
	{
		//�A�N�e�B�u��ԂłȂ���Ε`�悵�Ȃ�
		if (item->IsActive() == false)
		{
			continue;
		}

		//�`��
		item->Draw(camera);

	}
}

/// <summary>
/// �I������
/// </summary>
void ItemManager::Finalize()
{
}



/// <summary>
/// �A�C�e���ƃv���C���[���������Ă��邩����
/// </summary>
/// <param name="player">�v���C���[��AABB</param>
/// <returns>NONE�@���@�������Ă��Ȃ��ANONE�ȊO�@���@�������Ă���i���������A�C�e���̎�ނ�Ԃ�</returns>
Item::ItemType ItemManager::PlayerHitItemType(AABBFor3D* player)
{
	//�v���C���[�ƃA�C�e�������蔻��
	for (std::unique_ptr<Actor>&item : m_items)
	{
		//�A�C�e�����A�N�e�B�u��ԂłȂ���Δ��肵�Ȃ�
		if (item->IsActive() == false)
		{
			continue;
		}

		//�v���C���[�ƃA�C�e�����������Ă���Γ������Ă���A�C�e���^�C�v��Ԃ�
		if (item->GetAABB()->HitCheck(player))
		{
			//���������A�C�e���^�C�v�̎擾
			Item::ItemType hitItemType = static_cast<Item::ItemType>(item->GetTypeInt());
			//�A�C�e���̃��Z�b�g�����A�A�C�e��������
			item->Reset();
			//�������Ă���A�C�e���^�C�v��Ԃ�
			return hitItemType;

			
		}	
	}

	//�������Ă��Ȃ����NONE��Ԃ�
	return Item::ItemType::NONE;
}


/// <summary>
/// �e�̐���
/// </summary>
/// <param name="shadowMap">�V���h�E�}�b�v�̐��|�C���^</param>
/// <param name="view">�r���[�s��</param>
/// <param name="projection">�v���W�F�N�V�����s��</param>
void ItemManager::Shadow(ShadowMap* shadowMap, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	//�e�̐�������
	for (std::unique_ptr<Actor>&item : m_items)
	{
		//�A�N�e�B�u��ԂłȂ���ΐ������Ȃ�
		if (item->IsActive() == false)
		{
			continue;
		}

		//�e����
		item->CreateShadow(shadowMap, view, projection);
		
	}
}

/// <summary>
/// �A�C�e���̐���
/// </summary>
/// <param name="position">���W</param>
/// <param name="type">�A�C�e���̃^�C�v</param>
/// <param name="rot">�A���O��</param>
/// <returns>true = ���������Afalse = �������s</returns>
bool ItemManager::CreateItem(const DirectX::SimpleMath::Vector3& position, Item::ItemType type, const DirectX::SimpleMath::Vector3& rot)
{

	//�A�C�e���̐���
	return m_spawners[type]->Create(m_items, position, rot, nullptr, m_pModel[type]);
}

/// <summary>
/// �A�C�e���ƃX�e�[�W���������Ă��邩����
/// </summary>
void ItemManager::ItemToStageCheckHit()
{
	//�X�e�[�W�ƃA�C�e�����������Ă��邩������s��
	for (std::unique_ptr<Actor>&item : m_items)
	{
		//�A�N�e�B�u��ԂłȂ���Δ��肵�Ȃ�
		if (item->IsActive() == false)
		{
			continue;
		}

		//Item�ɃL���X�g
		Item* i = dynamic_cast<Item*>(item.get());
		//NULL�`�F�b�N
		if (i != nullptr)
		{
			//�A�C�e���ƃX�e�[�W���������Ă��邩����
			bool isHit = m_stageManeger->StageToActorHitCheck(item.get());
			//���ʂ��A�C�e���ɐݒ肷��
			i->SetStageHit(isHit);

		}

	}
}

