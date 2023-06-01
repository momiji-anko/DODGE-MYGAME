/*
* 2023/04/14
* Item.h
* �A�C�e��
* �����@��
*/
#pragma once
#include"../Actor/Actor.h"
#include"../ShadowMap.h"
#include"Game/PlayScene/Blink.h"

/// <summary>
/// �A�C�e��
/// </summary>
class Item :public Actor
{
public:
	/// <summary>
	/// �A�C�e���^�C�v�𐮐��^�Ŏ擾
	/// </summary>
	/// <returns>�A�C�e���^�C�v</returns>
	int GetTypeInt()const override { return static_cast<int>(m_itemType); }
	/// <summary>
	/// �A�C�e���^�C�v�𐮐��^�Őݒ�
	/// </summary>
	/// <param name="item">�A�C�e���^�C�v</param>
	void SetTypeInt(int item) override { m_itemType = static_cast<ItemType>(item); }

	/// <summary>
	/// ���Ƃ̏Փ˃t���O
	/// </summary>
	/// <param name="hit">true = ���������A�@false = �����Ă��Ȃ�</param>
	void SetStageHit(bool hit) { m_stageHit = hit; }

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Item();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Item()override;

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="velocity">�ړ��x���V�e�B</param>
	/// <param name="position">���W</param>
	/// <param name="scale">�X�P�[��</param>
	/// <param name="active">�A�N�e�B�u</param>
	/// <param name="angle">�A���O��</param>
	/// <param name="behavia">�r�w�C�r�A�[�̐��|�C���^</param>
	/// <param name="model">���f���̐��|�C���^</param>
	/// <param name="commonState">�R�����X�e�[�g�̐��|�C���^</param>
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
	/// �X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	void Update(const DX::StepTimer& timer)override;

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="camera">�J�����̐��|�C���^</param>
	void Draw(Camera* camera)override;

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset()override;

public:
	//�A�C�e���^�C�v�̗񋓑�
	enum class ItemType
	{
		//���̃A�C�e��
		SHIELD_ITEM,

		NONE,
	};

private:
	//�A�C�e���̎��S���鎞��
	static const float ITEM_DELETE_TIME_S;
	//�A�C�e���^�C�v
	ItemType m_itemType;
	//�n�ʂɂ��Ă��邩
	bool m_stageHit;
	//���S�^�C��
	float m_deleteTime_s;
	//�_��
	std::unique_ptr<Blink> m_blink;
};