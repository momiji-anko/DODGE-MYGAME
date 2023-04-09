#include"pch.h"
#include"Player.h"
#include"DeviceResources.h"
#include<Keyboard.h>
#include<Mouse.h>
#include<Effects.h>

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
	m_blink(nullptr),
	m_playerModel{},
	m_pAdx2(nullptr),
	m_jumpMusicID(0),
	m_stageManager(nullptr),
	m_itemManager(nullptr),
	m_invincibleCountCoolDownTime_s(0),
	m_modelTime_s(0),
	m_playerID(0),
	m_keys{}
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
/// <param name="behavia">ビヘイビアー（Playrでは使わないのでNULLでOK）</param>
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
	

	//モデルの生成
	CreatePlayerModel();

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

	Item::ItemType itemType = m_itemManager->PlayerHitItemType(GetAABB());

	//アクティブでなければ処理を終わる
	if (!m_active)
		return;

	if (itemType == Item::ItemType::SHIELD_ITEM)
	{
		ShieldCountUp();
	}
	
	//ベロシティのXとZを０にする
	m_velocity.z = 0.0f;
	m_velocity.x = 0.0f;


	PlayerMove(timer);

	m_position += m_velocity;
	
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

}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラのポインター</param>
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


void Player::ShieldCountUp()
{
	m_pAdx2->Play(CRI_CUESHEET_0_COIN04_);
	m_shieldCount++;
}

void Player::ShieldCountDown()
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



void Player::CreatePlayerModel()
{
	for (int i = 0; i < m_modelFiles.size();i++)
	{
		m_playerModel[i] = CreateModel(m_modelFiles[i].c_str());
	}

}

/// <summary>
/// モデル作成
/// </summary>
/// <param name="fileName">モデルファイルパス</param>
/// <returns>モデルのユニークポインター</returns>
std::unique_ptr<DirectX::Model> Player::CreateModel(const wchar_t* fileName)
{
	//DeviceResourcesからデバイスの取得
	ID3D11Device1* device = DX::DeviceResources::GetInstance()->GetD3DDevice();

	//エフェクトファクトリの作成
	std::unique_ptr<DirectX::EffectFactory> effectFactry = std::make_unique<DirectX::EffectFactory>(device);

	//	テクスチャの読み込みパス指定
	effectFactry->SetDirectory(L"Resources/Models");

	//	モデルデータ読み込み＆読み込んだモデルを返す
	return DirectX::Model::CreateFromCMO(
		device,
		fileName,
		*effectFactry
	);

}

void Player::PlayerMove(const DX::StepTimer& timer)
{

	const DirectX::Keyboard::Keys& right =    m_keys[0];
	const DirectX::Keyboard::Keys& left =     m_keys[1];
	const DirectX::Keyboard::Keys& forward =  m_keys[2];
	const DirectX::Keyboard::Keys& backward = m_keys[3];
	const DirectX::Keyboard::Keys& jump =     m_keys[4];

	float elapsedTime = timer.GetElapsedSeconds();

	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	if (keyState.IsKeyDown(right))
	{
		m_position.x += MOVE_SPEED * elapsedTime;

		m_rotation.y = -90;

	}

	if (keyState.IsKeyDown(left))
	{
		m_position.x -= MOVE_SPEED * elapsedTime;

		m_rotation.y = 90;
	}

	if (keyState.IsKeyDown(backward))
	{
		m_position.z += MOVE_SPEED * elapsedTime;

		m_rotation.y = 180;
	}

	if (keyState.IsKeyDown(forward))
	{
		m_position.z -= MOVE_SPEED * elapsedTime;

		m_rotation.y = 0;
	}




	if ((keyState.IsKeyDown(right)) && (keyState.IsKeyDown(backward)))
		m_rotation.y = -(90 + 45);

	if ((keyState.IsKeyDown(right)) && (keyState.IsKeyDown(forward)))
		m_rotation.y = -(45);

	if ((keyState.IsKeyDown(left)) && (keyState.IsKeyDown(backward)))
		m_rotation.y = (90 + 45);

	if ((keyState.IsKeyDown(left)) && (keyState.IsKeyDown(forward)))
		m_rotation.y = (45);


	if (keyState.IsKeyDown(right) || keyState.IsKeyDown(left) || keyState.IsKeyDown(forward) || keyState.IsKeyDown(backward))
	{
		m_modelTime_s += elapsedTime * 10;

		if (m_modelTime_s >= 4.0f)
		{
			m_modelTime_s = 0;
		}
	}
	else
	{
		m_modelTime_s = 0;
	}


	if (m_stageManager->PlayerStageAABBHitCheck(this))
	{

		m_velocity.y = 0;


		if (keyState.IsKeyDown(jump))
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
}
