/*
* 2023/04/14
* Item.h
* アイテム
* 麻生　楓
*/
#pragma once
#include"../Actor/Actor.h"
#include"../ShadowMap.h"
#include"Game/PlayScene/Blink.h"

/// <summary>
/// アイテム
/// </summary>
class Item :public Actor
{
public:
	/// <summary>
	/// アイテムタイプを整数型で取得
	/// </summary>
	/// <returns>アイテムタイプ</returns>
	int GetTypeInt()const override { return static_cast<int>(m_itemType); }
	/// <summary>
	/// アイテムタイプを整数型で設定
	/// </summary>
	/// <param name="item">アイテムタイプ</param>
	void SetTypeInt(int item) override { m_itemType = static_cast<ItemType>(item); }

	/// <summary>
	/// 床との衝突フラグ
	/// </summary>
	/// <param name="hit">true = 当たった、　false = 当っていない</param>
	void SetStageHit(bool hit) { m_stageHit = hit; }

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Item();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Item()override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="velocity">移動ベロシティ</param>
	/// <param name="position">座標</param>
	/// <param name="scale">スケール</param>
	/// <param name="active">アクティブ</param>
	/// <param name="angle">アングル</param>
	/// <param name="behavia">ビヘイビアーの生ポインタ</param>
	/// <param name="model">モデルの生ポインタ</param>
	/// <param name="commonState">コモンステートの生ポインタ</param>
	void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& scale, 
		const DirectX::SimpleMath::Vector3& rotation, 
		bool active,
		IBehavior* behavia,
		DirectX::Model* model
	)override;

	// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	void Update(const DX::StepTimer& timer)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラの生ポインタ</param>
	void Draw(Camera* camera)override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// リセット
	/// </summary>
	void Reset()override;

public:
	//アイテムタイプの列挙体
	enum class ItemType
	{
		//盾のアイテム
		SHIELD_ITEM,

		NONE,
	};

private:
	//アイテムの死亡する時間
	static const float ITEM_DELETE_TIME_S;
	//アイテムタイプ
	ItemType m_itemType;
	//地面についているか
	bool m_stageHit;
	//死亡タイム
	float m_deleteTime_s;
	//点滅
	std::unique_ptr<Blink> m_blink;
};