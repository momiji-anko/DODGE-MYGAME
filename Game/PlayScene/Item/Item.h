#pragma once
#include"../Actor/Actor.h"
#include"../ShadowMap.h"
#include"Game/PlayScene/Blink.h"

class Item :public Actor
{
public:
	//列挙体
	enum class ItemType
	{
		SHIELD_ITEM,
		
		NONE,
	};

protected:
	static const float ITEM_DELETE_TIME_S;

	ItemType m_itemType;
	bool m_hit;

	float m_deleteTime_s;


	float m_timer;

	ShadowMap* m_shadowMap;

	std::unique_ptr<Blink> m_blink;

public:
	//コンストラクタ
	Item();
	//デストラクタ
	virtual ~Item()override;

	// 初期化
	virtual void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
		bool active,
		float angle,
		IBehavior* behavia,
		DirectX::Model* model,
		DirectX::CommonStates* commonState)override;

	// 更新
	virtual void Update(const DX::StepTimer& timer)override;

	// 描画
	virtual void Draw(Camera* camera)override;

	// 終了処理
	virtual void Finalize()override;
	
	//アイテムの取得
	virtual ItemType GetItemType();
	//アイテムの設定
	virtual void SetItemType(ItemType item);

	void Reset();

	void SetGruondHit(bool hit) { m_hit = hit; }
	bool GetHit() { return m_hit; }

	void Set(ShadowMap* shadowMap) { m_shadowMap = shadowMap; }

	void Shadow(ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);


private:
	
	

};