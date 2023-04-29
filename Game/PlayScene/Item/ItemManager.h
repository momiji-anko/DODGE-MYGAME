/*
* 2023/04/15
* ItemManager.h
* アイテムの管理クラス
* 麻生　楓
*/
#pragma once
#include"Item.h"
#include<map>
#include"ItemSpawner.h"
#include<Model.h>
#include"Libraries/MyLibraries/Camera.h"

//前方宣言
class StageManager;

/// <summary>
/// アイテムマネージャー
/// </summary>
class ItemManager
{
public:

	/// <summary>
	/// 影の生成
	/// </summary>
	/// <param name="shadowMap">シャドウマップの生ポインタ</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="projection">プロジェクション行列</param>
	void Shadow(ShadowMap* shadowMap, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);

	/// <summary>
	/// ステージマネージャーの設定
	/// </summary>
	/// <param name="stageManeger">ステージマネージャーの生ポインタ</param>
	void SetStageManeger(StageManager* stageManeger) { m_stageManeger = stageManeger; }


public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ItemManager();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ItemManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	void Update(const DX::StepTimer& timer);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラの生ポインタ</param>
	void Draw(Camera* camera);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	
	/// <summary>
	/// アイテムとプレイヤーが当たっているか判定
	/// </summary>
	/// <param name="player">プレイヤーのAABB</param>
	/// <returns>NONE　＝　当たっていない、NONE以外　＝　当たっている（当たったアイテムの種類を返す</returns>
	Item::ItemType PlayerHitItemType(AABBFor3D* player);


private:
	/// <summary>
	/// アイテムの生成
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="type">アイテムのタイプ</param>
	/// <param name="rot">アングル</param>
	/// <returns>true = 生成成功、false = 生成失敗</returns>
	bool CreateItem(const DirectX::SimpleMath::Vector3& position ,Item::ItemType type, const DirectX::SimpleMath::Vector3& rot);

	/// <summary>
	/// アイテムとステージが当たっているか判定
	/// </summary>
	void ItemToStageCheckHit();

private:
	//定数
	//アイテムの最大数
	static const int ITEM_MAX_NUM;
	//アイテムのスポーンクールタイム
	static const float ITEM_SPAWNE_COOL_TIME_S;

	//アイテムのスポーン範囲
	static const float ITEM_SPAWNE_EREA;
	//アイテムのスポーンする高さ
	static const float ITEM_SPAWNE_HEIGHT;

	//変数
	//アイテムのオブジェクトプール
	std::vector< std::unique_ptr<Actor>>                m_items;
	//アイテムのスポナー
	std::map<Item::ItemType, std::unique_ptr<ISpawner>> m_spawners;
	//アイテムのモデル生成
	std::map < Item::ItemType, DirectX::Model*>         m_pModel;
	
	//アイテムスポーンタイム
	float  m_spawneTime_s;
	
	//ステージマネージャー
	StageManager* m_stageManeger;

};