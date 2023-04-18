/*
* 2023/04/15
* ItemManager.h
* �A�C�e���̊Ǘ��N���X
* �����@��
*/
#pragma once
#include"Item.h"
#include<map>
#include"ItemSpawner.h"
#include<Model.h>
#include"Libraries/MyLibraries/Camera.h"

//�O���錾
class StageManager;

/// <summary>
/// �A�C�e���}�l�[�W���[
/// </summary>
class ItemManager
{
private:
	//�萔
	//�A�C�e���̍ő吔
	static const int ITEM_MAX_NUM;
	//�A�C�e���̃X�|�[���N�[���^�C��
	static const float ITEM_SPAWNE_COOL_TIME_S;

	//�ϐ�
	//�A�C�e���̃I�u�W�F�N�g�v�[��
	std::vector< std::unique_ptr<Actor>>                        m_items;
	//�A�C�e���̃X�|�i�[
	std::map<Item::ItemType, std::unique_ptr<ISpawner>>         m_spawners;
	//�A�C�e���̃��f������
	std::map < Item::ItemType, DirectX::Model*> m_pModel;
	//�R�����X�e�[�g
	DirectX::CommonStates*                                      m_commonState;
	//�A�C�e���X�|�[���^�C��
	float                                                       m_spawneTime_s;
	//�X�e�[�W�}�l�[�W���[
	StageManager*                                               m_stageManeger;

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ItemManager();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ItemManager();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="commonState">�R�����X�e�[�g�̐��|�C���^</param>
	void Initialize(DirectX::CommonStates* commonState);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	void Update(const DX::StepTimer& timer);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="camera">�J�����̐��|�C���^</param>
	void Draw(Camera* camera);

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	
	/// <summary>
	/// �A�C�e���ƃv���C���[���������Ă��邩����
	/// </summary>
	/// <param name="player">�v���C���[��AABB</param>
	/// <returns>NONE�@���@�������Ă��Ȃ��ANONE�ȊO�@���@�������Ă���i���������A�C�e���̎�ނ�Ԃ�</returns>
	Item::ItemType PlayerHitItemType(AABBFor3D* player);


	/// <summary>
	/// �e�̐���
	/// </summary>
	/// <param name="shadowMap">�V���h�E�}�b�v�̐��|�C���^</param>
	/// <param name="view">�r���[�s��</param>
	/// <param name="projection">�v���W�F�N�V�����s��</param>
	void Shadow(ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	/// <summary>
	/// �X�e�[�W�}�l�[�W���[�̐ݒ�
	/// </summary>
	/// <param name="stageManeger">�X�e�[�W�}�l�[�W���[�̐��|�C���^</param>
	void SetStageManeger(StageManager* stageManeger) { m_stageManeger = stageManeger; }

private:
	/// <summary>
	/// �A�C�e���̐���
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="type">�A�C�e���̃^�C�v</param>
	/// <param name="angle">�A���O��</param>
	/// <returns>true = ���������Afalse = �������s</returns>
	bool CreateItem(const DirectX::SimpleMath::Vector3& position ,Item::ItemType type,float angle);

	/// <summary>
	/// �A�C�e���ƃX�e�[�W���������Ă��邩����
	/// </summary>
	void ItemToStageCheckHit();
};