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
	m_jumpVelcity(0),
	m_flyVelocity{}, 
	m_invalidCount(0),
	m_isInvalid(true)
{

}
//デストラクタ	
Player::~Player() 
{
	
}

// 初期化
void Player::Initialize(const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& position, bool active, float angle, IBehavior* behavia, DirectX::Model* model, DirectX::CommonStates* commonState)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	ID3D11Device1* device = pDR->GetD3DDevice();

	m_commonState = commonState;
	m_velocity = velocity;
	m_position = position;

	m_active = active;

	m_angle = angle;

	m_behavia = behavia;

	m_pModel = model;


	switch (m_playerID)
	{
	case 1:
		Player1CreateModel();
		break;
	case 2:
		Player2CreateModel();
		break;
	default:
		break;
	}
	
	m_itemHit = false;
	m_modelTime = 0.0f;

	m_AABBObject = std::make_unique<AABBFor3D>();
	m_AABBObject->Initialize();
	m_AABBObject->SetData(DirectX::SimpleMath::Vector3(m_position.x - 0.5f, m_position.y - 0.9f, m_position.z - 0.5f), DirectX::SimpleMath::Vector3(m_position.x + 0.5f, m_position.y + 0.5f, m_position.z + 0.5f));

	m_effect = std::make_unique<EffectManager>();
	
	m_effect->Create();
	m_effect->Initialize(3.0f, DirectX::SimpleMath::Vector3(m_position.x, m_position.y, m_position.z + 0.5f));
	m_effect->SetStartPosition(DirectX::SimpleMath::Vector3(m_position.x, m_position.y, m_position.z + 0.5f));

	m_pAdx2 = &ADX2::GetInstance();
	

	m_capsule = std::make_unique<Capsule>();
	m_capsule->a = DirectX::SimpleMath::Vector3(m_position);
	m_capsule->b = DirectX::SimpleMath::Vector3(m_position);
	m_capsule->r = 0.5f;


	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/haet.png",
		nullptr,
		m_invalidTexture.ReleaseAndGetAddressOf()
	);
}

// 更新
void Player::Update(const DX::StepTimer& timer)
{

	// マウス入力情報を取得する
	DirectX::Mouse::State mouseState = DirectX::Mouse::Get().GetState();

	float time = timer.GetElapsedSeconds();

	Item::ItemType itemType = m_itemManger->PlayerHitItemType(GetAABB());

	if (!m_active)
		return;

	if (itemType != Item::ItemType::NONE)
	{
		int playerStateInt = static_cast<int>(itemType);


		InvalidCountUp();

		m_itemHit = false;

	}
	
	m_velocity.z += 0.0f;
	m_velocity.x += 0.0f;
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

		m_flyVelocity *= DirectX::SimpleMath::Vector3(0.93f, 0.9f, 0.93f);

	}


	if (m_playerState == PlayerState::INVINCIBLE)
	{
		m_invincbleTime += timer.GetElapsedSeconds();
		m_effect->Update(timer);




		if (m_invincbleTime > INVINCIBLE_TIME_SECONDS)
		{
			m_playerState = PlayerState::NORMAL;
		
			m_pModel->UpdateEffects([&](DirectX::IEffect* effect)
				{
					DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
					if (lights)
					{
						// ライトの影響をなくす
						lights->SetAmbientLightColor(DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.0f));
						lights->SetLightEnabled(0, false);
						lights->SetLightEnabled(1, false);
						lights->SetLightEnabled(2, false);
					}
					DirectX::BasicEffect* basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
					if (basicEffect)
					{
						// エミッション色を白に設定する
						basicEffect->SetEmissiveColor(DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
						
					}
				});

			m_pAdx2->Stop(m_musicID);
			
		}

	}
	else
	{
		m_invincbleTime = 0;

	}

	m_AABBObject->SetData(DirectX::SimpleMath::Vector3(m_position.x - 0.5f, m_position.y - 0, m_position.z - 0.5f), DirectX::SimpleMath::Vector3(m_position.x + 0.5f, m_position.y + 2.0f, m_position.z + 0.5f));

	m_capsule->a = DirectX::SimpleMath::Vector3(m_position.x, m_position.y+0.5f, m_position.z);
	m_capsule->b = DirectX::SimpleMath::Vector3(m_position.x, m_position.y + 1.5f, m_position.z);

	if (m_invalidCountCoolDownTime_s >= 0.0f)
	{
		m_invalidCountCoolDownTime_s -= time;
		InvalidTime();
	}

	if (GetPosition().y < -50.0f)
	{
		m_active = false;

	}
}

// 描画
void Player::Draw(Camera* camera)
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	
	if (!m_active)
		return;

	m_effect->SetRenderState(
		camera->GetEyePosition(),
		camera->GetViewMatrix(),
		camera->GetProjectionMatrix());
	m_effect->SetStartPosition(DirectX::SimpleMath::Vector3(m_position.x, m_position.y - 0.05, m_position.z + 0.5f));

	m_world = DirectX::SimpleMath::Matrix::Identity;
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::CreateRotationY(m_rotation.y / 180.0f * 3.14f);
	DirectX::SimpleMath::Matrix scale= DirectX::SimpleMath::Matrix::CreateScale(2);
	

	m_world *= rot * scale * trans;
	if (m_isInvalid)
	{
		m_playerModel[m_playerModelNum[m_modelTime]]->Draw(context, *m_commonState, m_world, camera->GetViewMatrix(), camera->GetProjectionMatrix());

	}
	if (m_playerState == PlayerState::INVINCIBLE)
	{
		m_effect->Render();
	}



}

void Player::TextureDraw(DirectX::SpriteBatch* spriteBatch)
{
	for (int i = 0; i < m_invalidCount; i++)
	{
		spriteBatch->Draw(m_invalidTexture.Get(), DirectX::SimpleMath::Vector2(20.0f, 10.0f + (i * 64.0f)), nullptr);
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

	if (m_isInvalid)
	{

		m_playerModel[m_playerModelNum[m_modelTime]]->Draw(context, *m_commonState, m_world, view, projection, false, [&]()
			{
				shadowMap->DrawShadowMap(context);
			}
		);
	}
}

void Player::InvalidTime()
{

	if (m_invalidCountCoolDownTime_s>=2.75f)
	{
		m_isInvalid = false;
	}
	else if (m_invalidCountCoolDownTime_s >= 2.5f)
	{
		m_isInvalid = true;
	}
	else if (m_invalidCountCoolDownTime_s >= 2.25f)
	{
		m_isInvalid = false;
	}
	else if (m_invalidCountCoolDownTime_s >= 2.0f)
	{
		m_isInvalid = true;
	}
	else if (m_invalidCountCoolDownTime_s >= 1.75f)
	{
		m_isInvalid = false;
	}
	else if (m_invalidCountCoolDownTime_s >= 1.5f)
	{
		m_isInvalid = true;
	}
	else if (m_invalidCountCoolDownTime_s >= 1.25f)
	{
		m_isInvalid = false;
	}
	else if (m_invalidCountCoolDownTime_s >= 1.0f)
	{
		m_isInvalid = true;
	}
	else if (m_invalidCountCoolDownTime_s >= 0.75f)
	{
		m_isInvalid = false;
	}
	else if (m_invalidCountCoolDownTime_s >= 0.5f)
	{
		m_isInvalid = true;
	}
	else if (m_invalidCountCoolDownTime_s >= 0.25f)
	{
		m_isInvalid = false;
	}
	else
	{
		m_isInvalid = true;
	}


}


void Player::InvalidCountUp()
{
	m_pAdx2->Play(CRI_CUESHEET_0_COIN04_);
	m_invalidCount++;
}

void Player::InvalidCountDown()
{
	if (m_invalidCountCoolDownTime_s <= 0.0f)
	{
		m_invalidCount--;
		m_pAdx2->Play(CRI_CUESHEET_0_DAMAGE1);
		m_invalidCountCoolDownTime_s = 3.0f;
		if (m_invalidCount <= 0)
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
		m_modelTime += time * 10;

		if (m_modelTime >= 4.0f)
		{
			m_modelTime = 0;
		}
	}
	else
	{
		m_modelTime = 0;
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
		m_modelTime = 4;
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
		m_position.x += MOVE_SPEED * time;

		m_rotation.y = -90;

	}

	if (keyState.A)
	{
		m_position.x -= MOVE_SPEED * time;

		m_rotation.y = 90;
	}

	if (keyState.S)
	{
		m_position.z += MOVE_SPEED * time;

		m_rotation.y = 180;
	}

	if (keyState.W)
	{
		m_position.z -= MOVE_SPEED * time;

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
		m_modelTime += time * 10;

		if (m_modelTime >= 4.0f)
		{
			m_modelTime = 0;
		}
	}
	else
	{
		m_modelTime = 0;
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
		m_modelTime = 4;
		m_velocity.y += GRAVITY_FORCE * timer.GetElapsedSeconds();

	}
	m_position.y += m_velocity.y;
}

void Player::Player1CreateModel()
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();

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
}

void Player::Player2CreateModel()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
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
