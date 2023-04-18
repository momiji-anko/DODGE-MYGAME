/*
* 2023/04/14
* Item.cpp
* アイテム
* 麻生　楓
*/
#include"pch.h"
#include"Item.h"
#include"DeviceResources.h"

const float Item::ITEM_DELETE_TIME_S = 20.0f;

/// <summary>
/// コンストラクタ
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
/// デストラクタ
/// </summary>
Item:: ~Item()
{

}

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
void Item::Initialize(const DirectX::SimpleMath::Vector3& velocity,const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale,bool active,float angle,IBehavior* behavia,DirectX::Model* model,DirectX::CommonStates* commonState)
 {
	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスリソースからデバイスコンテキスト取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	
	//パラメータの設定
	//移動速度
	SetVelocity(velocity);
	
	//座標
	SetPosition(position);
	
	//スケール
	SetScale(scale);
	 
	//アクティブ
	
	SetActive(active);
	//アングル
	SetAngle(angle);
	
	//ビヘイビアー
	SetBehavior(behavia);
	//モデル
	SetModel(model);
	
	//コモンステート
	SetCommonState(commonState);
	
	
	//角度設定
	DirectX::SimpleMath::Vector3 rot = GetRotation().ToEuler();
	rot.y = DirectX::XM_PI;
	SetRotation(DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rot));

	
	//死亡タイム
	m_deleteTime_s = ITEM_DELETE_TIME_S;
	//地面に当たってない状態にする
	m_stageHit = false;

	//点滅の設定
	m_blink = std::make_unique<Blink>();
	//初期化
	m_blink->Initialize(0.3, 30, true, 0.003f);

 }

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void Item::Update(const DX::StepTimer& timer)
{
	//経過時間
	float elapsedTime = timer.GetElapsedSeconds();
	//死亡タイムを経過時間で引く
	m_deleteTime_s -= elapsedTime;

	DirectX::SimpleMath::Vector3 position = GetPosition();

	//当たり判定再設定
	DirectX::SimpleMath::Vector3 length = { 0.5f,0.5f,0.5f };

	GetAABB()->SetData(position + length, position + length);

	//点滅の更新
	m_blink->Update(timer);

	//死亡タイムが５秒になったら点滅を開始する
	if (m_deleteTime_s < ITEM_DELETE_TIME_S / 4.0f)
	{
		//点滅する
		m_blink->Start();
	}

	//地面に当っていなければ落下する
	if(!m_stageHit)
	{
		//移動
		SetPosition(position + (GetVelocity() * elapsedTime));
	}
	
	//死亡タイムが０になったら非アクティブ状態にする
	if (m_deleteTime_s <= 0)
	{
		//非アクティブ状態にする
		SetActive(false);
	}

}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラの生ポインタ</param>
void Item::Draw(Camera* camera)
 {

	 //デバイスリソース取得
	 DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	 //デバイスリソースからデバイスコンテキスト取得
	 ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	 CalculationWorld();

	 //点滅状態を取得し、消えている状態でなければ表示する
	 if (m_blink->IsBlink())
	 {
		 //モデル表示
		GetModel()->Draw(context, *GetCommonState(), GetWorld(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
	 }

 }

/// <summary>
/// 終了処理
/// </summary>
void Item::Finalize()
 {

 }

/// <summary>
/// アイテムのタイプの取得
/// </summary>
/// <returns>タイプ</returns>
Item::ItemType Item::GetItemType()
 {
	 return m_itemType;
 }

/// <summary>
/// アイテムタイプの設定
/// </summary>
/// <param name="item">アイテムタイプ</param>
void Item::SetItemType(ItemType item)
 {
	 m_itemType = item;
 }

/// <summary>
/// リセット
/// </summary>
void Item::Reset()
 {
	SetActive(false);
 }

/// <summary>
/// アイテムの影生成
/// </summary>
/// <param name="shadowMap">シャドウマップ</param>
/// <param name="view">ビュー行列</param>
/// <param name="projection">プロジェクション行列</param>
void Item::Shadow(ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
 {

	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスリソースからデバイスコンテキスト取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	 //影作成
	 GetModel()->Draw(context, *GetCommonState(), GetWorld(), view, projection, false, [&]()
		 {
			 shadowMap->DrawShadowMap(context);
		 }
	 );

 }

