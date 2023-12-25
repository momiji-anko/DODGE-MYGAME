/*
* 2023/04/20
* Obstacle.cpp
* 障害物クラス
* 麻生　楓
*/
#include"pch.h"
#include"Obstacle.h"
#include"DeviceResources.h"
#include"Game/PlayScene/GameContext/GameContext.h"

/// <summary>
/// コンストラクタ
/// </summary>
Obstacle::Obstacle()
	:
	Actor(),
	m_type(ObstacleType::NONE), 
	m_effect(nullptr),
	m_rotSpeed(0.3f),
	m_mass(0.0f),
	m_maxSpeed{ 1.0f },
	m_maxForce{ 4.0f },
	m_force{ DirectX::SimpleMath::Vector3::Zero },
	m_playerPosition{ DirectX::SimpleMath::Vector3::Zero },
	m_meandelingTime_s(0.0f)
{
	
}

/// <summary>
/// デストラクタ
/// </summary>
Obstacle::~Obstacle()
{
	Reset();
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
void Obstacle::Initialize(const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, bool active, IBehavior* behavia, DirectX::Model* model)
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

	//角度
	SetRotation(rotation);

	//ビヘイビアー
	SetBehavior(behavia);
	//モデル
	SetModel(model);



}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void Obstacle::Update(const DX::StepTimer& timer)
{

	//エフェクトがNULLでなければ更新する
	if (m_effect != nullptr)
	{
		//エフェクト更新
		m_effect->Update(timer);
	}
	
	//ビヘイビアーがNULLでなければ実行する
	if (GetBehavior() != nullptr)
	{
		//実行する
		GetBehavior()->Execute(timer, this);
	}

	//エリア内にいるか確認エリア外であれば死亡
	if (CheckInArea())
	{
		//リセット
		Reset();
	}
}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラの生ポインタ</param>
void Obstacle::Draw(Camera* camera)
{
	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスコンテキスト取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	
	//ワールド行列を計算する
	CalculateWorld();

	//炎でなければモデル表示する
	if (m_type != ObstacleType::NORMAL && m_type != ObstacleType::MEANDERING)
	{
		//モデル表示する
		GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorld(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
	}
	//炎であればエフェクト表示する
	else if(m_type == ObstacleType::NORMAL || m_type == ObstacleType::MEANDERING)
	{
		//エフェクト表示する
		m_effect->SetRenderState(camera->GetEyePosition(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
		m_effect->SetOffsetPosition(GetPosition());
		m_effect->Render();
	}
}

/// <summary>
/// 終了処理
/// </summary>
void Obstacle::Finalize()
{
	Reset();
}

/// <summary>
/// リセット
/// </summary>
void Obstacle::Reset()
{
	SetActive(false);
}

/// <summary>
/// 探索行動
/// </summary>
/// <param name="targetPosition">目標座標</param>
/// <returns>目標へのベクトル</returns>
DirectX::SimpleMath::Vector3 Obstacle::Seek(const DirectX::SimpleMath::Vector3& targetPosition)
{
	//自分からターゲットへの移動量
	DirectX::SimpleMath::Vector3 desiredVelocity = targetPosition - GetPosition();
	//正規化し方向だけにする
	desiredVelocity.Normalize();
	//速度をかける
	desiredVelocity *= GetMaxSpeed();
	//目標への移動量計算
	DirectX::SimpleMath::Vector3 steeringForce = desiredVelocity - GetVelocity();
	//目標への移動量を返す
	return steeringForce;
}

/// <summary>
/// 障害物がエリア外にいるか確認
/// </summary>
/// <returns>true = エリア外、false = エリア内</returns>
bool Obstacle::CheckInArea()
{
	//エリア
	static const float Area = 30.0f;
	//座標設定
	DirectX::SimpleMath::Vector3 position = GetPosition();

	//エリア外にいるか確認
	if ((position.x <= -Area || position.x >= Area)||(position.z <= -Area || position.z >= Area))
	{
		//エリア外
		return true;
	}
	//エリア内
	return false;
}
