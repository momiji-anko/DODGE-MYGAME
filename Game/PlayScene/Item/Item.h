#pragma once
#include"../Actor/Actor.h"
#include"../ShadowMap.h"
class Item :public Actor
{
public:
	//�񋓑�
	enum class ItemType
	{
		INVINCIBLE_ITEM,
		SLIP_ITEM,
		NONE,
	};

protected:
	ItemType m_itemType;
	bool m_hit;

	float m_deleteTime_s;


	float m_timer;

	int m_hoge;
	ShadowMap* m_shadowMap;


public:
	//�R���X�g���N�^
	Item();
	//�f�X�g���N�^
	virtual ~Item()override;

	// ������
	virtual void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
		bool active,
		float angle,
		IBehavior* behavia,
		DirectX::Model* model,
		DirectX::CommonStates* commonState)override;

	// �X�V
	virtual void Update(const DX::StepTimer& timer)override;

	// �`��
	virtual void Draw(Camera* camera)override;

	// �I������
	virtual void Finalize()override;
	
	//�A�C�e���̎擾
	virtual ItemType GetItemType();
	//�A�C�e���̐ݒ�
	virtual void SetItemType(ItemType item);

	void Reset();

	void SetGruondHit(bool hit) { m_hit = hit; }
	bool GetHit() { return m_hit; }

	void Set(ShadowMap* shadowMap) { m_shadowMap = shadowMap; }

	void Shadow(ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);


private:
	
	void InvincibleItemUpdete(const DX::StepTimer& timer);

	void SlipItemUpdete(const DX::StepTimer& timer);

};