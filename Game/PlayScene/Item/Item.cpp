/*
* 2023/04/14
* Item.cpp
* アイテム
* 麻生　楓
*/
#include"pch.h"
#include"Item.h"
#include"DeviceResources.h"
#include"Game/PlayScene/GameContext/GameContext.h"

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
/// <param name="rotation">角度</param>
/// <param name="active">アクティブ</param>
/// <param name="behavia">ビヘイビアーの生ポインタ</param>
/// <param name="model">モデルの生ポインタ</param>
/// <param name="commonState">コモンステートの生ポインタ</param>
void Item::Initialize(const DirectX::SimpleMath::Vector3& velocity,const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, bool active,IBehavior* behavia,DirectX::Model* model)
 {

	
	//パラメータの設定
	//移動速度
	SetVelocity(velocity);
	
	//座標
	SetPosition(position);
	
	//スケール
	SetScale(scale);
	 
	//アクティブ
	SetActive(active);
		
	//ビヘイビアー
	SetBehavior(behavia);

	//モデル
	SetModel(model);
	
	
	
	//角度設定
	SetRotation(rotation);

	
	//死亡タイム
	m_deleteTime_s = ITEM_DELETE_TIME_S;
	//地面に当たってない状態にする
	m_stageHit = false;

	//点滅の設定
	m_blink = std::make_unique<Blink>();
	//初期化
	m_blink->Initialize(0.1, 30);

 }

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void Item::Update(const DX::StepTimer& timer)
{
	//経過時間
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	//死亡タイムを経過時間で引く
	m_deleteTime_s -= elapsedTime;

	DirectX::SimpleMath::Vector3 position = GetPosition();

	//当たり判定再設定
	DirectX::SimpleMath::Vector3 length = { 0.5f,0.5f,0.5f };

	GetAABB()->SetData(position - length, position + length);

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

	 CalculateWorld();

	 //点滅状態を取得し、消えている状態でなければ表示する
	 if (m_blink->IsBlink())
	 {
		 //モデル表示
		GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorld(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
	 }

 }

/// <summary>
/// 終了処理
/// </summary>
void Item::Finalize()
 {

 }

/// <summary>
/// リセット
/// </summary>
void Item::Reset()
 {
	SetActive(false);
 }

