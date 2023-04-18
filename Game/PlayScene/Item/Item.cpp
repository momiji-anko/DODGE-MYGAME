/*
* 2023/04/14
* Item.cpp
* �A�C�e��
* �����@��
*/
#include"pch.h"
#include"Item.h"
#include"DeviceResources.h"

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
/// <param name="active">�A�N�e�B�u</param>
/// <param name="angle">�A���O��</param>
/// <param name="behavia">�r�w�C�r�A�[�̐��|�C���^</param>
/// <param name="model">���f���̐��|�C���^</param>
/// <param name="commonState">�R�����X�e�[�g�̐��|�C���^</param>
void Item::Initialize(const DirectX::SimpleMath::Vector3& velocity,const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale,bool active,float angle,IBehavior* behavia,DirectX::Model* model,DirectX::CommonStates* commonState)
 {
	//�f�o�C�X���\�[�X�擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//�f�o�C�X���\�[�X����f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	
	//�p�����[�^�̐ݒ�
	//�ړ����x
	SetVelocity(velocity);
	
	//���W
	SetPosition(position);
	
	//�X�P�[��
	SetScale(scale);
	 
	//�A�N�e�B�u
	
	SetActive(active);
	//�A���O��
	SetAngle(angle);
	
	//�r�w�C�r�A�[
	SetBehavior(behavia);
	//���f��
	SetModel(model);
	
	//�R�����X�e�[�g
	SetCommonState(commonState);
	
	
	//�p�x�ݒ�
	DirectX::SimpleMath::Vector3 rot = GetRotation().ToEuler();
	rot.y = DirectX::XM_PI;
	SetRotation(DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rot));

	
	//���S�^�C��
	m_deleteTime_s = ITEM_DELETE_TIME_S;
	//�n�ʂɓ������ĂȂ���Ԃɂ���
	m_stageHit = false;

	//�_�ł̐ݒ�
	m_blink = std::make_unique<Blink>();
	//������
	m_blink->Initialize(0.3, 30, true, 0.003f);

 }

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void Item::Update(const DX::StepTimer& timer)
{
	//�o�ߎ���
	float elapsedTime = timer.GetElapsedSeconds();
	//���S�^�C�����o�ߎ��Ԃň���
	m_deleteTime_s -= elapsedTime;

	DirectX::SimpleMath::Vector3 position = GetPosition();

	//�����蔻��Đݒ�
	DirectX::SimpleMath::Vector3 length = { 0.5f,0.5f,0.5f };

	GetAABB()->SetData(position + length, position + length);

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

	 CalculationWorld();

	 //�_�ŏ�Ԃ��擾���A�����Ă����ԂłȂ���Ε\������
	 if (m_blink->IsBlink())
	 {
		 //���f���\��
		GetModel()->Draw(context, *GetCommonState(), GetWorld(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
	 }

 }

/// <summary>
/// �I������
/// </summary>
void Item::Finalize()
 {

 }

/// <summary>
/// �A�C�e���̃^�C�v�̎擾
/// </summary>
/// <returns>�^�C�v</returns>
Item::ItemType Item::GetItemType()
 {
	 return m_itemType;
 }

/// <summary>
/// �A�C�e���^�C�v�̐ݒ�
/// </summary>
/// <param name="item">�A�C�e���^�C�v</param>
void Item::SetItemType(ItemType item)
 {
	 m_itemType = item;
 }

/// <summary>
/// ���Z�b�g
/// </summary>
void Item::Reset()
 {
	SetActive(false);
 }

/// <summary>
/// �A�C�e���̉e����
/// </summary>
/// <param name="shadowMap">�V���h�E�}�b�v</param>
/// <param name="view">�r���[�s��</param>
/// <param name="projection">�v���W�F�N�V�����s��</param>
void Item::Shadow(ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
 {

	//�f�o�C�X���\�[�X�擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//�f�o�C�X���\�[�X����f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	 //�e�쐬
	 GetModel()->Draw(context, *GetCommonState(), GetWorld(), view, projection, false, [&]()
		 {
			 shadowMap->DrawShadowMap(context);
		 }
	 );

 }

