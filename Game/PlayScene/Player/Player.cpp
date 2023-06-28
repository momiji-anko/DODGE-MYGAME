/*
* 2023/04/23
* Player.cpp
* プレイヤー
* 麻生　楓
*/
#include"pch.h"
#include"Player.h"
#include"DeviceResources.h"
#include<Keyboard.h>
#include<Mouse.h>
#include<Effects.h>

#include"Libraries/MyLibraries/ModelManager.h"
#include"Libraries/MyLibraries/TextureManager.h"
#include"Game/PlayScene/GameContext/GameContext.h"


//	1秒間に進むマスの数
const float Player::MOVE_SPEED = 9.0f;
//	1秒間に落ちるマスの数
const float Player::GRAVITY_FORCE = -1.4f;
//	ジャンプ力
const float Player::JUMP_FORCE = 0.50f;
//無敵時間
const float Player::INVINCIBLE_TIME_SECONDS = 3.0f;

//上のAABB
const DirectX::SimpleMath::Vector3 Player::AABB_AREA_UP = DirectX::SimpleMath::Vector3(0.5f, 2.0f, 0.5f);
//下のAABB
const DirectX::SimpleMath::Vector3 Player::AABB_AREA_DOWN = DirectX::SimpleMath::Vector3(-0.5f, -0.0f, -0.5f);

//上のカプセルの長さ
const float Player::CAPSULE_AREA_UP = 1.3f;
//下のカプセルの長さ
const float Player::CAPSULE_AREA_DOWN = 0.5f;

//落ちた際の死亡する高さ
const float Player::FALL_DEAD_AREA = -50.0f;
//モデルタイムの最大数
const float Player::MAX_MODEL_TIME_S = 4.0f;
//モデルタイムの速度
const float Player::MODEL_TIME_SPEED = 10.0f;

//プレイヤーのシールド画面表示位置
const DirectX::SimpleMath::Vector2 Player::PLAYERS_SHIELD_TEXTURE_POSITION[2] = {
	DirectX::SimpleMath::Vector2(1200.0f,10.0f),
	DirectX::SimpleMath::Vector2(20.0f,10.0f)
};

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
	:
	Actor(),
	m_invincbleTime(0.0f),
	m_playerModel{},
	m_playerModelNum{0,1,0,2,3},
	m_modelTime_s(0),
	m_pAdx2(nullptr),
	m_jumpMusicID(0),
	m_flyVelocity{}, 
	m_shieldCount(0),
	m_invincibleCountCoolDownTime_s(0),
	m_shieldTexture{},
	m_itemManager(nullptr),
	m_stageManager(nullptr),
	m_playerID(0),
	m_blink(nullptr),
	m_keys{},
	m_modelFiles{},
	m_obstacleManager{}
{

}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="velocity">移動量</param>
/// <param name="position">初期座標</param>
/// <param name="rotation">角度</param>
/// <param name="active">存在しているか</param>
/// <param name="behavia">ビヘイビアー（Playrでは使わないのでNULLでOK）</param>
/// <param name="model">プレイヤーのモデルだがNULLでOK</param>
void Player::Initialize(const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, bool active, IBehavior* behavia, DirectX::Model* model)
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

	//モデルの生成
	CreatePlayerModel();

	
	//当たり判定の領域更新
	HitAreaUpdate();

	//ADX2のインスタンス取得
	m_pAdx2 = &ADX2::GetInstance();


	//盾のテクスチャ読み込み
	m_shieldTexture = TextureManager::GetInstance().LoadTexture(L"Resources/Textures/haet.png");
	
	//点滅速度
	float blinkTime_s = 0.25f;
	//点滅回数
	int blinkCount = 12;
	//ブリンクする
	m_blink = std::make_unique<Blink>();
	//初期化
	m_blink->Initialize(blinkTime_s, blinkCount);

	m_fireEffect = std::make_unique<FireEffectManager>();
	m_fireEffect->Create();
	m_fireEffect->Initialize(3.0f, GetPosition());

	m_effectLifeTime_s = 1.0f;

	GameContext::GetInstance().SetIsPlayerDeath(false);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void Player::Update(const DX::StepTimer& timer)
{
	//アクティブでなければ処理をしない
	if (!IsActive())
		return;

	//エフェクトタイムが０以下になったら死亡する
	if (m_effectLifeTime_s <= 0)
	{
		SetActive(false);
	}

	//盾が-1になったら死亡エフェクトだす
	if (m_shieldCount <= -1)
	{
		m_effectLifeTime_s -= timer.GetElapsedSeconds();
		
		//エフェクトを複数更新しエフェクトの速度を上げる
		for (int i = 0; i < 10; i++)
		{
			m_fireEffect->Update(timer);
		}

		return;
	}


	//アイテムと当たり判定を取る
	Item::ItemType itemType = m_itemManager->PlayerHitItemType(GetAABB());

	//当たったアイテムがシールドであった場合、シールドカウントを増やす
	if (itemType == Item::ItemType::SHIELD_ITEM)
	{
		ShieldCountUp();
	}

	//プレイヤーの移動
	PlayerMove(timer);

	//吹き飛ばす量
	DirectX::SimpleMath::Vector3 flyVelocity = DirectX::SimpleMath::Vector3::Zero;

	//回転する棒と当たり判定取る,当たっていた場合プレイヤーを吹き飛ばす
	if (m_obstacleManager->PlayerCapsuleHitCheck(this, &flyVelocity))
	{
		m_flyVelocity = flyVelocity;
	}

	//障害物と当たっていた場合シールドを減らす
	if (m_obstacleManager->PlayerHitCheck(GetAABB()))
	{
		ShieldCountDown();
	}

	//無敵時間が０より多ければ無敵時間を減らす
	if (m_invincibleCountCoolDownTime_s > 0.0f)
	{
		//経過時間
		float elapsedTime= static_cast<float>(timer.GetElapsedSeconds());
		//無敵時間を経過時間で引く
		m_invincibleCountCoolDownTime_s -= elapsedTime;
	}
	else
	{
		//無敵時間が０以下であれば点滅を止める
		m_blink->Stop();
	}

	//吹き飛ばされている状態であればだんだん減速する
	if (m_flyVelocity.Length() != 0.0f)
	{
		SetPosition(GetPosition() + m_flyVelocity);
		//減速
		m_flyVelocity *= DirectX::SimpleMath::Vector3(0.91f, 0.91f, 0.91f);
	}


	//当たり判定の領域の更新
	HitAreaUpdate();

	//落下してY座標が-50になったら死亡する
	if (GetPosition().y < FALL_DEAD_AREA)
	{
		//ダメージ音を出す
		m_pAdx2->Play(CRI_CUESHEET_0_DAMAGE1);

		m_shieldCount = -1;

		//カメラの揺れをリセット
		m_camera->ShakeReset();
	}

	//ブリンクの更新
	m_blink->Update(timer);

	

}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラのポインター</param>
void Player::Draw(Camera* camera)
{
	//デバイスリソースの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスコンテキストの取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//アクティブでなければ処理をしない
	if (!IsActive())
		return;

	//ワールド行列計算
	CalculateWorld();

	//現在のモデルの状態
	int modelTime = static_cast<int>(m_modelTime_s);

	//カメラを揺らす
	camera->ShakeCamera();

	m_camera = camera;

	//ブリンクしていなければモデル表示
	if (m_blink->IsBlink())
	{
		m_playerModel[m_playerModelNum[modelTime]]->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorld(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
	}
	
	//盾が-1であればプレイヤー死亡
	if (m_shieldCount <= -1) 
	{

		//プレイヤー死亡演出
		PlayerDeath(m_obstacleManager->GetHitType(),camera);



	}
	//盾UIの描画
	TextureDraw();
}

/// <summary>
/// 盾UIの描画
/// </summary>
void Player::TextureDraw()
{

	DirectX::SpriteBatch* spriteBatch = GameContext::GetInstance().GetSpriteBatcth();

	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, GameContext::GetInstance().GetCommonState()->NonPremultiplied());

	//盾があれば盾描画
	for (int i = 0; i < m_shieldCount; i++)
	{
		//盾同士の離れている距離
		DirectX::SimpleMath::Vector2 shieldTexDistance = DirectX::SimpleMath::Vector2(0.0f, 64.0f * i);


		spriteBatch->Draw(m_shieldTexture.Get(), PLAYERS_SHIELD_TEXTURE_POSITION[m_playerID] + shieldTexDistance, nullptr);
	}
	spriteBatch->End();
}

/// <summary>
/// 終了処理
/// </summary>
void Player::Finalize()
{
	Reset();
}

/// <summary>
/// 持っている盾の数を１増やす
/// </summary>
void Player::ShieldCountUp()
{
	//シールドを取ったら音を鳴らす
	m_pAdx2->Play(CRI_CUESHEET_0_COIN04_);
	//シールドの数を１増やす
	m_shieldCount++;
}


/// <summary>
/// 持っている盾の数を１減らす　盾を一個も持っていない場合死亡させる
/// </summary>
void Player::ShieldCountDown()
{
	//無敵時間ではない場合シールドを１減らす
	if (m_invincibleCountCoolDownTime_s <= 0.0f)
	{
		//シールドを１減らす
		m_shieldCount--;
		//ダメージ音を出す
		m_pAdx2->Play(CRI_CUESHEET_0_DAMAGE1);
		
		//無敵時間
		m_invincibleCountCoolDownTime_s = INVINCIBLE_TIME_SECONDS;
		
		//点滅する
		m_blink->Start();

		//カメラの揺れをリセット
		m_camera->ShakeReset();
	}

}


/// <summary>
/// 影生成
/// </summary>
/// <param name="shadow">シャドウマップの生ポインタ</param>
/// <param name="view">ビュー行列</param>
/// <param name="projection">プロジェクション行列</param>
void Player::CreateShadow(ShadowMap* shadow, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスコンテキスト取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	
	//現在のモデルの番号
	int nowModelNum = m_playerModelNum[static_cast<unsigned __int64>(m_modelTime_s)];

	//モデルがあれば影を生成する
	if (m_playerModel [nowModelNum] != nullptr)
	{
		//ワールド行列を計算する
		CalculateWorld();
		
		//影生成
		m_playerModel[nowModelNum]->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorld(), view, projection, false, [&]()
			{
				shadow->DrawShadowMap(context);
			}
		);
	}


}

/// <summary>
/// リセット
/// </summary>
void Player::Reset()
{
	SetActive(false);
}

/// <summary>
/// プレイヤーのモデル作成
/// </summary>
void Player::CreatePlayerModel()
{
	//モデルファイルパスの数分、モデル配列確保
	m_playerModel.resize(m_modelFiles.size());

	//モデルの読み込み
	for (int i = 0; i < m_modelFiles.size();i++)
	{
		m_playerModel[i] = ModelManager::GetInstance().LoadModel(m_modelFiles[i].c_str());
	}

}

/// <summary>
/// 当たり判定の領域の更新
/// </summary>
void Player::HitAreaUpdate()
{
	DirectX::SimpleMath::Vector3 position = GetPosition();
	//AABB当たり判定の領域の設定
	GetAABB()->SetData(position + AABB_AREA_DOWN, position + AABB_AREA_UP);

	//カプセル当たり判定の領域の設定
	GetCapsule()->a = DirectX::SimpleMath::Vector3(position.x, position.y + CAPSULE_AREA_DOWN, position.z);
	GetCapsule()->b = DirectX::SimpleMath::Vector3(position.x, position.y + CAPSULE_AREA_UP, position.z);

}

/// <summary>
/// プレイヤーの動き
/// </summary>
/// <param name="timer">タイマー</param>
void Player::PlayerMove(const DX::StepTimer& timer)
{
	//キー配列からそれぞれの割り当てられたキーを取得
	//右キー
	const DirectX::Keyboard::Keys& right =   m_keys[0];
	//左キー
	const DirectX::Keyboard::Keys& left =    m_keys[1];
	//前キー
	const DirectX::Keyboard::Keys& forward = m_keys[2];
	//後ろキー
	const DirectX::Keyboard::Keys& back =    m_keys[3];
	//ジャンプキー
	const DirectX::Keyboard::Keys& jump =    m_keys[4];

	//経過時間
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	
	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	

	//ベロシティ取得
	DirectX::SimpleMath::Vector3 velocity = GetVelocity();
	//座標取得
	DirectX::SimpleMath::Vector3 position = GetPosition();
	//角度取得
	DirectX::SimpleMath::Vector3 rotation = GetRotation().ToEuler();

	//ベロシティのＸとＺを０にする
	velocity.x = 0.0f;
	velocity.z = 0.0f;

	//移動しているか
	bool IsMove = false;

	//90度
	static const float NINETY_ANGLE = DirectX::XM_PI / 2.0f;

	//右キーを押していた場合右に移動＆右を向く
	if (keyState.IsKeyDown(right))
	{
		velocity.x += MOVE_SPEED * elapsedTime;

		rotation.y = -NINETY_ANGLE;

		IsMove = true;

	}
	//左キーを押していた場合右に移動＆左を向く
	else if (keyState.IsKeyDown(left))
	{
		velocity.x -= MOVE_SPEED * elapsedTime;

		rotation.y = NINETY_ANGLE;

		IsMove = true;
	}

	//後ろキーを押していた場合右に移動＆後ろを向く
	 if (keyState.IsKeyDown(back))
	{
		velocity.z += MOVE_SPEED * elapsedTime;

		rotation.y = NINETY_ANGLE * 2.0f;

		IsMove = true;
	}
	//前キーを押していた場合右に移動＆前を向く
	else if (keyState.IsKeyDown(forward))
	{
		velocity.z -= MOVE_SPEED * elapsedTime;

		rotation.y = 0;

		IsMove = true;
	}

	 //左キーと前キーを押していた場合左前を向く
	 if ((keyState.IsKeyDown(left)) && (keyState.IsKeyDown(forward)))
	 {
		 //45
		 rotation.y = NINETY_ANGLE / 2.0f;
	 }

	 //左キーと後ろキーを押していた場合左後ろを向く
	 if ((keyState.IsKeyDown(left)) && (keyState.IsKeyDown(back)))
	 {
		 //90+45
		 rotation.y = NINETY_ANGLE + NINETY_ANGLE / 2.0f;
	 }

	//右キーと前キーを押していた場合右前を向く
	if ((keyState.IsKeyDown(right)) && (keyState.IsKeyDown(forward)))
	{
		//-45
		rotation.y = -NINETY_ANGLE / 2.0f;
	}

	//右キーと後ろキーを押していた場合右後ろを向く
	if ((keyState.IsKeyDown(right)) && (keyState.IsKeyDown(back)))
	{
		//-(90+45)
		rotation.y = -(NINETY_ANGLE + NINETY_ANGLE / 2.0f);
	}
		

	//移動している場合モデルタイムを増やす
	if (IsMove)
	{
		m_modelTime_s += MODEL_TIME_SPEED * elapsedTime;;

		//モデルタイムが４を超えたら０にリセット
		if (m_modelTime_s >= MAX_MODEL_TIME_S)
		{
			m_modelTime_s = 0.0f;
		}
	}
	//移動していない場合モデルタイムは０にする
	else
	{
		m_modelTime_s = 0.0f;
	}


	//ステージに当たっている場合落下しない
	if (m_stageManager->StageToActorHitCheck(this))
	{
		//ベロシティＹを０にする
		velocity.y = 0;

		//ジャンプキーを押したらジャンプする
		if (keyState.IsKeyDown(jump))
		{
			
			velocity.y += JUMP_FORCE;

			//ジャンプしたら音を出す
			m_pAdx2->Play(CRI_CUESHEET_0_JUMP08);

		}

	}
	//ステージに当たっていない場合落下する
	else
	{
		//モデルタイムをMAXにする
		m_modelTime_s = MAX_MODEL_TIME_S;
		//落下する
		velocity.y += GRAVITY_FORCE * static_cast<float>(timer.GetElapsedSeconds());

	}

	//ベロシティの設定
	SetVelocity(velocity);
	//移動する
	SetPosition(GetPosition() + velocity);
	//角度設定
	SetRotation(rotation);
}

/// <summary>
/// プレイヤーの死亡演出
/// </summary>
/// <param name="hitType">当たった障害物の種類</param>
/// <param name="camera">カメラ</param>
void Player::PlayerDeath(Obstacle::ObstacleType hitType, Camera* camera)
{
	//デバイスリソースの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//当たった障害物がファイアーだったらプレイヤーが燃える
	if (hitType == Obstacle::ObstacleType::NORMAL || hitType == Obstacle::ObstacleType::MEANDERING)
	{
		//描画の設定
		m_fireEffect->SetRenderState(camera->GetEyePosition(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
		
		std::vector<DirectX::SimpleMath::Vector3> firePositions = 
		{
			DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.3f),
			DirectX::SimpleMath::Vector3(-0.1f, 0.0f, 0.3f),
			DirectX::SimpleMath::Vector3(0.1f, 0.0f, 0.3f)
		};

		//エフェクト表示する
		for (const DirectX::SimpleMath::Vector3& firePosition : firePositions)
		{
			m_fireEffect->SetOffsetPosition(GetPosition() + firePosition);
			m_fireEffect->Render();
		}

	}
	//当たった障害物が棒だったらプレイヤーが紫色になる
	else if (hitType != Obstacle::ObstacleType::NORMAL && hitType != Obstacle::ObstacleType::MEANDERING)
	{

		ID3D11Device1* device = pDR->GetD3DDevice();

		static std::shared_ptr<DirectX::BasicEffect> effect;

		if (effect == nullptr)
			effect = std::make_shared<DirectX::BasicEffect>(device);

		effect->SetWorld(DirectX::SimpleMath::Matrix::Identity);
		effect->SetView(camera->GetViewMatrix());
		effect->SetProjection(camera->GetProjectionMatrix());
		effect->SetColorAndAlpha(DirectX::SimpleMath::Vector4(0.5f,0.0f,0.5f,0.5f));

		effect->SetTextureEnabled(false);

		effect->EnableDefaultLighting();

		for (const auto& mesh : m_playerModel[m_playerModelNum[static_cast<int>(m_modelTime_s)]]->meshes)
		{
			for (const auto& part : mesh->meshParts)
			{
				part->ModifyEffect(pDR->GetD3DDevice(), effect);
			}
		}

		DirectX::SimpleMath::Vector3 rotation = GetRotation().ToEuler();
		rotation.x = DirectX::XM_PI/2;
		//角度設定
		SetRotation(rotation);
	}

}
