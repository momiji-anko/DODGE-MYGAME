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

//アイテムの最大数
const int ItemManager::ITEM_MAX_NUM = 30;
//アイテムのスポーンクールタイム
const float ItemManager::ITEM_SPAWNE_COOL_TIME_S = 15.0f;

/// <summary>
/// コンストラクタ
/// </summary>
ItemManager::ItemManager()
	:
	m_items{},
	m_spawners{},
	m_pModel{},
	m_commonState(nullptr),
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
/// <param name="commonState">コモンステートの生ポインタ</param>
void ItemManager::Initialize(DirectX::CommonStates* commonState)
{
	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスリソースからデバイスコンテキスト取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//コモンステート
	m_commonState = commonState;
	
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
	float elapsedTime = timer.GetElapsedSeconds();
	//スポーンタイムを経過時間で引く
	m_spawneTime_s -= elapsedTime;

	//スポーンタイムが０になったらアイテムを生成する
	if (m_spawneTime_s <= 0.0f)
	{
		//スポーンタイムを初期化する
		m_spawneTime_s = ITEM_SPAWNE_COOL_TIME_S;
		//アイテムを生成する
		CreateItem(DirectX::SimpleMath::Vector3(MyRandom::GetFloatRange(-7.0f, 6.0f), 15.0f, MyRandom::GetFloatRange(-7.0f, 7.0f)), Item::ItemType::SHIELD_ITEM, 0.0f);
	
	}

	//アイテムの更新
	for (std::unique_ptr<Actor>& item : m_items)
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
	for (std::unique_ptr<Actor>& item : m_items)
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
	for (std::unique_ptr<Actor>& item : m_items)
	{
		//アイテムがアクティブ状態でなければ判定しない
		if (item->IsActive() == false)
		{
			continue;
		}

		//プレイヤーとアイテムが当たっていれば当たっているアイテムタイプを返す
		if (item->GetAABB()->HitCheck(player))
		{
			//ActorからItemにダイナミックキャスト
			Item* i = dynamic_cast<Item*>(item.get());
			//NULLチェック
			if (i != nullptr)
			{
				//当たったアイテムタイプの取得
				Item::ItemType hitItemType = i->GetItemType();
				//アイテムのリセットをし、アイテムを消す
				i->Reset();
				//当たっているアイテムタイプを返す
				return hitItemType;

			}
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
void ItemManager::Shadow(ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	//影の生成する
	for (std::unique_ptr<Actor>& item : m_items)
	{
		//アクティブ状態でなければ生成しない
		if (item->IsActive() == false)
		{
			continue;
		}

		//ActorからItemにダイナミックキャスト
		Item* i = dynamic_cast<Item*>(item.get());
		//NULLチェック
		if (i != nullptr)
		{
			//影の生成
			i->Shadow(shadowMap, view, projection);
		}

		
	}
}

/// <summary>
/// アイテムの生成
/// </summary>
/// <param name="position">座標</param>
/// <param name="type">アイテムのタイプ</param>
/// <param name="angle">アングル</param>
/// <returns>true = 生成成功、false = 生成失敗</returns>
bool ItemManager::CreateItem(const DirectX::SimpleMath::Vector3& position, Item::ItemType type, float angle)
{
	//アイテムの生成
	return m_spawners[type]->Create(m_items, position, angle, nullptr, m_pModel[type], m_commonState);
}

/// <summary>
/// アイテムとステージが当たっているか判定
/// </summary>
void ItemManager::ItemToStageCheckHit()
{
	//ステージとアイテムが当たっているか判定を行う
	for (std::unique_ptr<Actor>& item : m_items)
	{
		//アクティブ状態でなければ判定しない
		if (item->IsActive() == false)
		{
			continue;
		}

		//ActorからItemにダイナミックキャスト
		Item* i = dynamic_cast<Item*>(item.get());
		//NULLチェック
		if (i != nullptr)
		{
			//アイテムとステージが当たっているか判定
			bool isHit = m_stageManeger->ItemHitCheck(item.get());
			//結果をアイテムに設定する
			i->SetStageHit(isHit);
		}

	}
}

