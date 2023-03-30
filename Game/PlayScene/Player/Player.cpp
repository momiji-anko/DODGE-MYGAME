#include"pch.h"
#include"Player.h"
#include"DeviceResources.h"
#include<Keyboard.h>
#include<Mouse.h>
#include<Effects.h>
//#include"Helpers/DODGESound_acf.h"
const float Player::MOVE_SPEED = 9.0f;
const float Player::GRAVITY_FORCE = -1.4f;
const float Player::JUMP_FORCE = 0.50f;
const float Player::INVINCIBLE_TIME_SECONDS = 8.0f;

//コンストラクタ
Player::Player() 
	:
	Actor(),
	m_invincbleTime(0.0f),
	m_playerState(PlayerState::NORMAL),
	m_playerModelNum{0,1,0,2,3},
	m_flyVelocity{}, 
	m_shieldCount(0),
	m_isShield(true),
	m_blink(nullptr)
{

}
//デストラクタ	
Player::~Player() 
{
	
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="velocity">移動量</param>
/// <param name="position">初期座標</param>
/// <param name="active">存在しているか</param>
/// <param name="angle">角度</param>
/// <param name="behavia">ビヘイビアー（NULLでOK）</param>
/// <param name="model">プレイヤーのモデルだがNULLでOK</param>
/// <param name="commonState">コモンステート</param>
void Player::Initialize(const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& position, bool active, float angle, IBehavior* behavia, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	//デバイスリソースの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスコンテキストの取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	//デバイスの取得
	ID3D11Device1* device = pDR->GetD3DDevice();
	//ステータスの初期化
	m_commonState = commonState;
	m_velocity = velocity;
	m_position = position;
	m_active = active;
	m_angle = angle;
	m_behavia = behavia;
	m_pModel = model;
	m_modelTime_s = 0.0f;

	//IDに応じてモデルを変える
	switch (m_playerID)
	{
	case 1:
		//１Pのモデル生成
		Player1CreateModel();
		break;
	case 2:
		//２Pのモデル生成
		Player2CreateModel();
		break;
	default:
		break;
	}
	
	//当たり判定AABBの作成
	m_AABBObject = std::make_unique<AABBFor3D>();
	//AABBの初期化
	m_AABBObject->Initialize();
	//当たり判定の領域の設定
	m_AABBObject->SetData(DirectX::SimpleMath::Vector3(m_position.x - 0.5f, m_position.y - 0.9f, m_position.z - 0.5f), DirectX::SimpleMath::Vector3(m_position.x + 0.5f, m_position.y + 0.5f, m_position.z + 0.5f));


	//ADX2のインスタンス取得
	m_pAdx2 = &ADX2::GetInstance();

	//当たり判定カプセルの作成
	m_capsule = std::make_unique<Capsule>();
	//当たり判定の領域の設定
	m_capsule->a = DirectX::SimpleMath::Vector3(m_position.x, m_position.y + 0.5f, m_position.z);
	m_capsule->b = DirectX::SimpleMath::Vector3(m_position.x, m_position.y + 1.5f, m_position.z);
	//カプセルの半径
	m_capsule->r = 0.5f;

	//盾のテクスチャ読み込み
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/haet.png",
		nullptr,
		m_shieldTexture.ReleaseAndGetAddressOf()
	);

	m_blink = std::make_unique<Blink>();
	m_blink->Initialize(0.15f, 12, 0.01f, true);

}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void Player::Update(const DX::StepTimer& timer)
{

	// マウス入力情報を取得する
	DirectX::Mouse::State mouseState = DirectX::Mouse::Get().GetState();

	float time = timer.GetElapsedSeconds();

	Item::ItemType itemType = m_itemManger->PlayerHitItemType(GetAABB());

	if (!m_active)
		return;

	if (itemType == Item::ItemType::SHIELD_ITEM)
	{

		InvalidCountUp();
	}
	
	m_velocity.z = 0.0f;
	m_velocity.x = 0.0f;
	switch (m_playerID)
	{
	case 1:
		Player1Move(timer);
		break;
	case 2:
		Player2Move(timer);
		break;

	default:
		break;
	}
	DirectX::SimpleMath::Vector3 moveVec;



	if (m_flyVelocity.Length() != 0.0f)
	{
		m_position += m_flyVelocity;

		m_flyVelocity *= DirectX::SimpleMath::Vector3(0.91f, 0.91f, 0.91f);

	}


	if (m_playerState == PlayerState::INVINCIBLE)
	{
		m_invincbleTime += timer.GetElapsedSeconds();

		if (m_invincbleTime > INVINCIBLE_TIME_SECONDS)
		{
			m_playerState = PlayerState::NORMAL;
		}

	}
	else
	{
		m_invincbleTime = 0;

	}

	m_AABBObject->SetData(DirectX::SimpleMath::Vector3(m_position.x - 0.5f, m_position.y - 0, m_position.z - 0.5f), DirectX::SimpleMath::Vector3(m_position.x + 0.5f, m_position.y + 2.0f, m_position.z + 0.5f));

	m_capsule->a = DirectX::SimpleMath::Vector3(m_position.x, m_position.y+0.5f, m_position.z);
	m_capsule->b = DirectX::SimpleMath::Vector3(m_position.x, m_position.y + 1.5f, m_position.z);



	if (GetPosition().y < -50.0f)
	{
		m_active = false;

	}
	m_blink->Update(timer);


	m_barrierModel->UpdateEffects([&](DirectX::IEffect* effect)
		{
			
			auto fog = dynamic_cast<DirectX::IEffectFog*>(effect);
			if (fog)
			{
				fog->SetFogEnabled(true);
				fog->SetFogStart(6); // assuming RH coordiantes
				fog->SetFogEnd(8);
				fog->SetFogColor(DirectX::Colors::Black);
			}
		});

}

// 描画
void Player::Draw(Camera* camera)
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	
	if (!m_active)
		return;

	

	m_world = DirectX::SimpleMath::Matrix::Identity;
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rotation.y));
	DirectX::SimpleMath::Matrix scale= DirectX::SimpleMath::Matrix::CreateScale(2);
	
	
	context->OMSetBlendState(m_commonState->AlphaBlend(), nullptr, 0xFFFFFFFF);
	if (true)//m_invalidCount > 0)
	{
		DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;
		world *= trans;

		//m_barrierModel->Draw(context, *m_commonState, world, camera->GetViewMatrix(), camera->GetProjectionMatrix());
	}

	m_world *= rot * scale * trans;
	if (m_blink->IsBlink())
	{
		int modelTime = static_cast<int>(m_modelTime_s);

		m_playerModel[m_playerModelNum[modelTime]]->Draw(context, *m_commonState, m_world, camera->GetViewMatrix(), camera->GetProjectionMatrix());

	}





}

void Player::TextureDraw(DirectX::SpriteBatch* spriteBatch)
{
	for (int i = 0; i < m_shieldCount; i++)
	{
		spriteBatch->Draw(m_shieldTexture.Get(), DirectX::SimpleMath::Vector2(20.0f, 10.0f + (i * 64.0f)), nullptr);
	}

}

// 終了処理
void Player::Finalize()
{

}

void Player::PlayerShadow( ShadowMap* shadowMap, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	if (m_blink->IsBlink())
	{
		int modelTime = static_cast<int >(m_modelTime_s);

		m_playerModel[m_playerModelNum[modelTime]]->Draw(context, *m_commonState, m_world, view, projection, false, [&]()
			{
				shadowMap->DrawShadowMap(context);
			}
		);
	}
}




void Player::InvalidCountUp()
{
	m_pAdx2->Play(CRI_CUESHEET_0_COIN04_);
	m_shieldCount++;
}

void Player::InvalidCountDown()
{
	if (m_invincibleCountCoolDownTime_s <= 0.0f)
	{
		m_shieldCount--;
		m_pAdx2->Play(CRI_CUESHEET_0_DAMAGE1);
		m_invincibleCountCoolDownTime_s = 3.0f;
		m_blink->Start();
		if (m_shieldCount <= -1)
		{
			m_active = false;
		}
	}

}

void Player::Player1Move(const DX::StepTimer& timer)
{
	float time = timer.GetElapsedSeconds();

	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();


	if (keyState.Right)
	{
		m_position.x += MOVE_SPEED * time;

		m_rotation.y = -90;

	}

	if (keyState.Left)
	{
		m_position.x -= MOVE_SPEED * time;

		m_rotation.y = 90;
	}

	if (keyState.Down)
	{
		m_position.z += MOVE_SPEED * time;

		m_rotation.y = 180;
	}

	if (keyState.Up)
	{
		m_position.z -= MOVE_SPEED * time;

		m_rotation.y = 0;
	}


	m_position.z += m_velocity.z;
	m_position.x += m_velocity.x;

	if ((keyState.Right) && (keyState.Down))
		m_rotation.y = -(90 + 45);

	if ((keyState.Right) && (keyState.Up))
		m_rotation.y = -(45);

	if ((keyState.Left) && (keyState.Down))
		m_rotation.y = (90 + 45);

	if ((keyState.Left) && (keyState.Up))
		m_rotation.y = (45);


	if (keyState.Right || keyState.Left || keyState.Down || keyState.Up)
	{
		m_modelTime_s += time * 10;

		if (m_modelTime_s >= 4.0f)
		{
			m_modelTime_s = 0;
		}
	}
	else
	{
		m_modelTime_s = 0;
	}


	if (m_stageManeger->PlayerStageAABBHitCheck(this))
	{

		m_velocity.y = 0;


		if (keyState.IsKeyDown(DirectX::Keyboard::Space))
		{

			m_velocity.y += JUMP_FORCE;

			m_pAdx2->Play(CRI_CUESHEET_0_JUMP08);

		}


	}
	else
	{
		m_modelTime_s = 4;
		m_velocity.y += GRAVITY_FORCE * timer.GetElapsedSeconds();

	}
	m_position.y += m_velocity.y;

}

void Player::Player2Move(const DX::StepTimer& timer)
{
	float time = timer.GetElapsedSeconds();

	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	if (keyState.D)
	{
		m_velocity.x += MOVE_SPEED * time;

		m_rotation.y = -90;

	}

	if (keyState.A)
	{
		m_velocity.x -= MOVE_SPEED * time;

		m_rotation.y = 90;
	}

	if (keyState.S)
	{
		m_velocity.z += MOVE_SPEED * time;

		m_rotation.y = 180;
	}

	if (keyState.W)
	{
		m_velocity.z -= MOVE_SPEED * time;

		m_rotation.y = 0;
	}


	m_position.z += m_velocity.z;
	m_position.x += m_velocity.x;

	if ((keyState.D) && (keyState.S))
		m_rotation.y = -(90 + 45);

	if ((keyState.D) && (keyState.W))
		m_rotation.y = -(45);

	if ((keyState.A) && (keyState.S))
		m_rotation.y = (90 + 45);

	if ((keyState.A) && (keyState.W))
		m_rotation.y = (45);


	if (keyState.D || keyState.A || keyState.S || keyState.W)
	{
		m_modelTime_s += time * 10;

		if (m_modelTime_s >= 4.0f)
		{
			m_modelTime_s = 0;
		}
	}
	else
	{
		m_modelTime_s = 0;
	}


	if (m_stageManeger->PlayerStageAABBHitCheck(this))
	{

		m_velocity.y = 0;


		if (keyState.IsKeyDown(DirectX::Keyboard::Z))
		{

			m_velocity.y += JUMP_FORCE;

			m_pAdx2->Play(CRI_CUESHEET_0_JUMP08);

		}


	}
	else
	{
		m_modelTime_s = 4;
		m_velocity.y += GRAVITY_FORCE * timer.GetElapsedSeconds();

	}
	m_position.y += m_velocity.y;
}

void Player::Player1CreateModel()
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//	エフェクトファクトリの作成
	DirectX::EffectFactory* factory0 = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//	テクスチャの読み込みパス指定
	factory0->SetDirectory(L"Resources/Models");

	//	ファイルを指定してモデルデータ読み込み
	m_playerModel[0] = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/playeraidoru.cmo",
		*factory0
	);

	delete factory0;

	//	エフェクトファクトリの作成
	DirectX::EffectFactory* factory1 = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//	テクスチャの読み込みパス指定
	factory1->SetDirectory(L"Resources/Models");

	//	ファイルを指定してモデルデータ読み込み
	m_playerModel[1] = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/playerhidari.cmo",
		*factory1
	);
	
	delete factory1;

	//	エフェクトファクトリの作成
	DirectX::EffectFactory* factory2 = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//	テクスチャの読み込みパス指定
	factory2->SetDirectory(L"Resources/Models");

	//	ファイルを指定してモデルデータ読み込み
	m_playerModel[2] = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/playermigiasi.cmo",
		*factory2
	);

	delete factory2;

	//	エフェクトファクトリの作成
	DirectX::EffectFactory* factory3 = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//	テクスチャの読み込みパス指定
	factory3->SetDirectory(L"Resources/Models");

	//	ファイルを指定してモデルデータ読み込み
	m_playerModel[3] = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/playerjanp.cmo",
		*factory3
	);

	delete factory3;

	//	エフェクトファクトリの作成
	DirectX::EffectFactory* factory4 = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//	テクスチャの読み込みパス指定
	factory4->SetDirectory(L"Resources/Models");

	//	ファイルを指定してモデルデータ読み込み
	m_barrierModel = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/barrier.cmo",
		*factory4
	);

	delete factory4;
}

void Player::Player2CreateModel()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//	エフェクトファクトリの作成
	DirectX::EffectFactory* factory0 = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//	テクスチャの読み込みパス指定
	factory0->SetDirectory(L"Resources/Models");

	//	ファイルを指定してモデルデータ読み込み
	m_playerModel[0] = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/Player2idoru.cmo",
		*factory0
	);

	delete factory0;

	//	エフェクトファクトリの作成
	DirectX::EffectFactory* factory1 = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//	テクスチャの読み込みパス指定
	factory1->SetDirectory(L"Resources/Models");

	//	ファイルを指定してモデルデータ読み込み
	m_playerModel[1] = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/Player2hidari.cmo",
		*factory1
	);

	delete factory1;

	//	エフェクトファクトリの作成
	DirectX::EffectFactory* factory2 = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//	テクスチャの読み込みパス指定
	factory2->SetDirectory(L"Resources/Models");

	//	ファイルを指定してモデルデータ読み込み
	m_playerModel[2] = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/Player2Migi.cmo",
		*factory2
	);

	delete factory2;

	//	エフェクトファクトリの作成
	DirectX::EffectFactory* factory3 = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//	テクスチャの読み込みパス指定
	factory3->SetDirectory(L"Resources/Models");

	//	ファイルを指定してモデルデータ読み込み
	m_playerModel[3] = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/Player2Janp.cmo",
		*factory3
	);

	delete factory3;
}
