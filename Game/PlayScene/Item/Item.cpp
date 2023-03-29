#include"pch.h"
#include"Item.h"
#include"DeviceResources.h"

const float Item::ITEM_DELETE_TIME_S = 20.0f;

//コンストラクタ
Item::Item()
	:
	Actor(),
	m_itemType(Item::ItemType::NONE)
{

}

//デストラクタ
Item:: ~Item()
{

}

// 初期化
 void Item::Initialize(const DirectX::SimpleMath::Vector3& velocity,const DirectX::SimpleMath::Vector3& position,bool active,float angle,IBehavior* behavia,DirectX::Model* model,DirectX::CommonStates* commonState)
 {

	 DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	 ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	 m_velocity = velocity;
	 m_position = position;

	 m_active = active;
	 m_angle = angle;

	 m_behavia = behavia;

	 m_pModel = model;
	 m_commonState = commonState;
	 m_AABBObject = std::make_unique<AABBFor3D>();
	 m_AABBObject->Initialize();
	 m_AABBObject->SetData(DirectX::SimpleMath::Vector3(m_position.x + 0.5f, m_position.y + 0.5f, m_position.z + 0.5f), DirectX::SimpleMath::Vector3(m_position.x - 0.5f, m_position.y - 0.5f, m_position.z - 0.5f));
	 m_deleteTime_s = ITEM_DELETE_TIME_S;
	 m_hit = false;

	 m_rotation.y = 180.0f;

	 m_blink = std::make_unique<Blink>();
	 m_blink->Initialize(0.3, 30, true, 0.003f);

 }

// 更新
void Item::Update(const DX::StepTimer& timer)
{

	float time = timer.GetElapsedSeconds();

	m_deleteTime_s -= time;

	m_AABBObject->SetData(DirectX::SimpleMath::Vector3(m_position.x - 0.5f, m_position.y , m_position.z - 0.5f), DirectX::SimpleMath::Vector3(m_position.x + 0.5f, m_position.y + 1.0f, m_position.z + 0.5f));

	m_blink->Update(timer);

	if (m_deleteTime_s < 5.0f)
	{
		m_blink->Start();
	}

	if(!m_hit)
	{
		m_position += m_velocity * time;
	}
	else
	{

	}

	if (m_deleteTime_s <= 0)
	{
		m_active = false;
	}

}

// 描画
 void Item::Draw(Camera* camera)
 {
	 m_world = DirectX::SimpleMath::Matrix::Identity;

	 DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	 ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	 // 深度バッファに書き込み参照する
	 //context->OMSetDepthStencilState(m_commonState->DepthRead(), 0);

	 DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	 DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::CreateRotationY(m_rotation.y / 180.0f * 3.14f);
	 DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(0.008f);

	 m_world *= scale * rot * trans;
	/// m_AABBObject->Draw(DirectX::SimpleMath::Matrix::Identity, camera->GetViewMatrix(), camera->GetProjectionMatrix(), DirectX::SimpleMath::Color(1, 1, 0, 1));

	 if (m_blink->IsBlink())
	 {
		
		m_pModel->Draw(context, *m_commonState, m_world, camera->GetViewMatrix(), camera->GetProjectionMatrix());
	 }
	 else
	 {

	 }

 }

// 終了処理
 void Item::Finalize()
 {

 }

//アイテムの取得
 Item::ItemType Item::GetItemType()
 {
	 return m_itemType;
 }

//アイテムの設定
 void Item::SetItemType(ItemType item)
 {
	 m_itemType = item;
 }

 void Item::Reset()
 {
	 m_active = false;
 }

 void Item::Shadow(ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
 {


	 DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	 ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	 
	 m_pModel->Draw(context, *m_commonState, m_world, view, projection, false, [&]()
		 {
			 shadowMap->DrawShadowMap(context);
		 }
	 );

 }

