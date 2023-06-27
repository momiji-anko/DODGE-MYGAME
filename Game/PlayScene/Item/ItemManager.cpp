/*
* 2023/04/15
* ItemManager.cpp
* アイテムの管理クラス
* 麻生　楓
*/
#include"pch.h"
#include"ItemManager.h"
#include"../MyRandom.h"
#include"DeviceResources.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"../Stage/StageManager.h"
#include"Game/PlayScene/GameContext/GameContext.h"

//アイテムの最大数
const int ItemManager::ITEM_MAX_NUM = 30;

//アイテムのスポーンクールタイム
const float ItemManager::ITEM_SPAWNE_COOL_TIME_S = 15.0f;

//アイテムのスポーン範囲
const float ItemManager::ITEM_SPAWNE_EREA = 6.0f;
//アイテムのスポーンする高さ
const float ItemManager::ITEM_SPAWNE_HEIGHT = 15.0f;



/// <summary>
/// コンストラクタ
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
/// デストラクタ
/// </summary>
ItemManager::~ItemManager()
{

}

/// <summary>
/// 初期化
/// </summary>
void ItemManager::Initialize()
{

	
	//アイテムの配列をアイテムの最大数でリサイズする
	m_items.resize(ITEM_MAX_NUM);

	//アイテムの生成
	for (std::unique_ptr<Actor>& item : m_items)
	{
		item = std::make_unique<Item>();
	}

	//アイテムのスポナー作成
	m_spawners[Item::ItemType::SHIELD_ITEM] = std::make_unique<ItemSpawner>();

	//アイテムのモデル生成
	m_pModel[Item::ItemType::SHIELD_ITEM] = ModelManager::GetInstance().LoadModel(L"Resources/Models/shield.cmo");


	//スポーンタイム初期化
	m_spawneTime_s = ITEM_SPAWNE_COOL_TIME_S;
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void ItemManager::Update(const DX::StepTimer& timer)
{
	//経過時間
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	//プレイヤー死亡していれば更新しない
	if (GameContext::GetInstance().GetIsPlayerDeath())
		return;

	//スポーンタイムを経過時間で引く
	m_spawneTime_s -= elapsedTime;

	//スポーンタイムが０になったらアイテムを生成する
	if (m_spawneTime_s <= 0.0f)
	{
		//スポーンタイムを初期化する
		m_spawneTime_s = ITEM_SPAWNE_COOL_TIME_S;

		DirectX::SimpleMath::Vector3 createPosition = DirectX::SimpleMath::Vector3(MyRandom::GetFloatRange(-ITEM_SPAWNE_EREA, ITEM_SPAWNE_EREA), ITEM_SPAWNE_HEIGHT, MyRandom::GetFloatRange(-ITEM_SPAWNE_EREA, ITEM_SPAWNE_EREA));

		//アイテムを生成する
		CreateItem(createPosition, Item::ItemType::SHIELD_ITEM, DirectX::SimpleMath::Vector3(0.0f, DirectX::XM_PI, 0.0f));
	
	}

	//アイテムの更新
	for (std::unique_ptr<Actor>&item : m_items)
	{
		//アクティブ状態でなければ更新しない
		if (item->IsActive() == false) 
		{
			continue; 
		}

		//更新
		item->Update(timer);
		
	}

	// アイテムとステージが当たっているか判定
	ItemToStageCheckHit();
}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラの生ポインタ</param>
void ItemManager::Draw(Camera* camera)
{
	//アイテムの描画
	for (std::unique_ptr<Actor>&item : m_items)
	{
		//アクティブ状態でなければ描画しない
		if (item->IsActive() == false)
		{
			continue;
		}

		//描画
		item->Draw(camera);

	}
}

/// <summary>
/// 終了処理
/// </summary>
void ItemManager::Finalize()
{
}



/// <summary>
/// アイテムとプレイヤーが当たっているか判定
/// </summary>
/// <param name="player">プレイヤーのAABB</param>
/// <returns>NONE　＝　当たっていない、NONE以外　＝　当たっている（当たったアイテムの種類を返す</returns>
Item::ItemType ItemManager::PlayerHitItemType(AABBFor3D* player)
{
	//プレイヤーとアイテム当たり判定
	for (std::unique_ptr<Actor>&item : m_items)
	{
		//アイテムがアクティブ状態でなければ判定しない
		if (item->IsActive() == false)
		{
			continue;
		}

		//プレイヤーとアイテムが当たっていれば当たっているアイテムタイプを返す
		if (item->GetAABB()->HitCheck(player))
		{
			//当たったアイテムタイプの取得
			Item::ItemType hitItemType = static_cast<Item::ItemType>(item->GetTypeInt());
			//アイテムのリセットをし、アイテムを消す
			item->Reset();
			//当たっているアイテムタイプを返す
			return hitItemType;

			
		}	
	}

	//当たっていなければNONEを返す
	return Item::ItemType::NONE;
}


/// <summary>
/// 影の生成
/// </summary>
/// <param name="shadowMap">シャドウマップの生ポインタ</param>
/// <param name="view">ビュー行列</param>
/// <param name="projection">プロジェクション行列</param>
void ItemManager::Shadow(ShadowMap* shadowMap, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	//影の生成する
	for (std::unique_ptr<Actor>&item : m_items)
	{
		//アクティブ状態でなければ生成しない
		if (item->IsActive() == false)
		{
			continue;
		}

		//影生成
		item->CreateShadow(shadowMap, view, projection);
		
	}
}

/// <summary>
/// アイテムの生成
/// </summary>
/// <param name="position">座標</param>
/// <param name="type">アイテムのタイプ</param>
/// <param name="rot">アングル</param>
/// <returns>true = 生成成功、false = 生成失敗</returns>
bool ItemManager::CreateItem(const DirectX::SimpleMath::Vector3& position, Item::ItemType type, const DirectX::SimpleMath::Vector3& rot)
{

	//アイテムの生成
	return m_spawners[type]->Create(m_items, position, rot, nullptr, m_pModel[type]);
}

/// <summary>
/// アイテムとステージが当たっているか判定
/// </summary>
void ItemManager::ItemToStageCheckHit()
{
	//ステージとアイテムが当たっているか判定を行う
	for (std::unique_ptr<Actor>&item : m_items)
	{
		//アクティブ状態でなければ判定しない
		if (item->IsActive() == false)
		{
			continue;
		}

		//Itemにキャスト
		Item* i = dynamic_cast<Item*>(item.get());
		//NULLチェック
		if (i != nullptr)
		{
			//アイテムとステージが当たっているか判定
			bool isHit = m_stageManeger->StageToActorHitCheck(item.get());
			//結果をアイテムに設定する
			i->SetStageHit(isHit);

		}

	}
}

