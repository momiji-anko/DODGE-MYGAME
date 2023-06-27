/*
* 2023/04/14
* Item.cpp
* �A�C�e��
* �����@��
*/
#include"pch.h"
#include"Item.h"
#include"DeviceResources.h"
#include"Game/PlayScene/GameContext/GameContext.h"

const float Item::ITEM_DELETE_TIME_S = 20.0f;

/// <summary>
/// �R���X�g���N�^
/// </summary>
Item::Item()
	:
	Actor(),
	m_itemType(Item::ItemType::NONE),
	m_stageHit(false),
	m_deleteTime_s(0),
	m_blink{}
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Item:: ~Item()
{

}

/// <summary>
/// ������
/// </summary>
/// <param name="velocity">�ړ��x���V�e�B</param>
/// <param name="position">���W</param>
/// <param name="scale">�X�P�[��</param>
/// <param name="rotation">�p�x</param>
/// <param name="active">�A�N�e�B�u</param>
/// <param name="behavia">�r�w�C�r�A�[�̐��|�C���^</param>
/// <param name="model">���f���̐��|�C���^</param>
/// <param name="commonState">�R�����X�e�[�g�̐��|�C���^</param>
void Item::Initialize(const DirectX::SimpleMath::Vector3& velocity,const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, bool active,IBehavior* behavia,DirectX::Model* model)
 {

	
	//�p�����[�^�̐ݒ�
	//�ړ����x
	SetVelocity(velocity);
	
	//���W
	SetPosition(position);
	
	//�X�P�[��
	SetScale(scale);
	 
	//�A�N�e�B�u
	SetActive(active);
		
	//�r�w�C�r�A�[
	SetBehavior(behavia);

	//���f��
	SetModel(model);
	
	
	
	//�p�x�ݒ�
	SetRotation(rotation);

	
	//���S�^�C��
	m_deleteTime_s = ITEM_DELETE_TIME_S;
	//�n�ʂɓ������ĂȂ���Ԃɂ���
	m_stageHit = false;

	//�_�ł̐ݒ�
	m_blink = std::make_unique<Blink>();
	//������
	m_blink->Initialize(0.1, 30);

 }

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void Item::Update(const DX::StepTimer& timer)
{
	//�o�ߎ���
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	//���S�^�C�����o�ߎ��Ԃň���
	m_deleteTime_s -= elapsedTime;

	DirectX::SimpleMath::Vector3 position = GetPosition();

	//�����蔻��Đݒ�
	DirectX::SimpleMath::Vector3 length = { 0.5f,0.5f,0.5f };

	GetAABB()->SetData(position - length, position + length);

	//�_�ł̍X�V
	m_blink->Update(timer);

	//���S�^�C�����T�b�ɂȂ�����_�ł��J�n����
	if (m_deleteTime_s < ITEM_DELETE_TIME_S / 4.0f)
	{
		//�_�ł���
		m_blink->Start();
	}

	//�n�ʂɓ����Ă��Ȃ���Η�������
	if(!m_stageHit)
	{
		//�ړ�
		SetPosition(position + (GetVelocity() * elapsedTime));
	}

	
	//���S�^�C�����O�ɂȂ������A�N�e�B�u��Ԃɂ���
	if (m_deleteTime_s <= 0)
	{
		//��A�N�e�B�u��Ԃɂ���
		SetActive(false);
	}

}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̐��|�C���^</param>
void Item::Draw(Camera* camera)
 {

	 //�f�o�C�X���\�[�X�擾
	 DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	 //�f�o�C�X���\�[�X����f�o�C�X�R���e�L�X�g�擾
	 ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	 CalculateWorld();

	 //�_�ŏ�Ԃ��擾���A�����Ă����ԂłȂ���Ε\������
	 if (m_blink->IsBlink())
	 {
		 //���f���\��
		GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorld(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
	 }

 }

/// <summary>
/// �I������
/// </summary>
void Item::Finalize()
 {

 }

/// <summary>
/// ���Z�b�g
/// </summary>
void Item::Reset()
 {
	SetActive(false);
 }

