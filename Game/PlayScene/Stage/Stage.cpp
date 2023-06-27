/*
* 2023/05/25
* Stage.cpp
* ステージ
* 麻生　楓
*/
#include"pch.h"
#include"Stage.h"
#include"../MyRandom.h"
#include"DeviceResources.h"
#include"Game/PlayScene/GameContext/GameContext.h"

const float Stage::ROTARION_TIME_S = 10.0f;


/// <summary>
/// コンストラクタ
/// </summary>
Stage::Stage()
	:
	Actor(),
	m_routine(0),
	m_offsetPosition(DirectX::SimpleMath::Vector3::Zero),
	m_time(0.0f),
	m_shadowMap(nullptr),
	m_endFlag(false),
	m_rotationTime_s(0),
	m_isRotation(false)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Stage::~Stage()
{
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="velocity">ベロシティ</param>
/// <param name="position">座標</param>
/// <param name="scale">拡大率</param>
/// <param name="rotation">角度</param>
/// <param name="active">アクティブ</param>
/// <param name="behavia">ビヘイビアー</param>
/// <param name="model">モデルの生ポインタ</param>
void Stage::Initialize(const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, bool active, IBehavior* behavia, DirectX::Model* model)
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

	//リセット
	Reset();

	//現実の頂点座標の配列確保
	m_nowVertices.resize(m_baseVertices.size());

}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void Stage::Update(const DX::StepTimer& timer)
{
	//ビヘイビアーがNULLでなければ実行
	if (GetBehavior() != nullptr)
	{
		GetBehavior()->Execute(timer, this);
	}

	//頂点座標の更新
	UpdateVertices();

}


/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラの生ポインタ</param>
void Stage::Draw(Camera* camera)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	
	//ワールド行列計算
	DirectX::SimpleMath::Matrix world = CalculateWorld();

	//揺れを計算
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(m_offsetPosition);
	//シャドウマップがあればモデルと影を表示
	if (m_shadowMap != nullptr)
	{
		//モデル＆影描画
		GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), world, camera->GetViewMatrix(), camera->GetProjectionMatrix(), false, [&]()
			{
				m_shadowMap->DrawShadow(context, false);
			}
		);
	}
	else
	{
		//モデル描画
		GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), world, camera->GetViewMatrix(), camera->GetProjectionMatrix());
	}

}

/// <summary>
/// 終了処理
/// </summary>
void Stage::Finalize()
{
}

/// <summary>
/// リセット
/// </summary>
void Stage::Reset()
{
	m_endFlag = false;
	m_routine = 0;
	m_time = 0;
	m_offsetPosition = DirectX::SimpleMath::Vector3::Zero;
	m_isRotation = false;
	m_rotationTime_s = ROTARION_TIME_S;
	m_previousRotation = GetRotation().ToEuler();
	m_nextRotation = DirectX::SimpleMath::Vector3::Zero;
}

/// <summary>
/// 現実の頂点座標の更新
/// </summary>
void Stage::UpdateVertices()
{
	//vertexの数分回す
	for (int j = 0; j < m_baseVertices.size(); j++)
	{
		//現在の当たり判定用の頂点座標行列
		DirectX::SimpleMath::Matrix nowVertexMatrix = DirectX::SimpleMath::Matrix::Identity;
		//StageのXとZの角度の計算
		DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation());
		//現在の頂点座標の計算
		nowVertexMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_baseVertices[j]) * rot * DirectX::SimpleMath::Matrix::CreateTranslation(GetPosition());
		//計算した行列を現在の頂点座標計算に代入する
		m_nowVertices[j] = DirectX::SimpleMath::Vector3(nowVertexMatrix._41, nowVertexMatrix._42, nowVertexMatrix._43);
	}

}
