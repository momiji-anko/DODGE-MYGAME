/*
* 2023/06/12
* PlayerManager.cpp
* プレイヤーマネージャー
* 麻生　楓
*/#include"pch.h"
#include"PlayerManager.h"
#include"Game/PlayScene/MyRandom.h"
#include"Game/PlayScene/GameContext/GameContext.h"
#include"Game/PlayScene/AliveTimer.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="playerMode">プレイヤーモード</param>
/// <param name="stageManager">ステージマネージャー</param>
/// <param name="itemManager">アイテムマネージャー</param>
/// <param name="obstacleManager">障害物マネージャー</param>
PlayerManager::PlayerManager(GameMain::PlayerMode playerMode,StageManager* stageManager, ItemManager* itemManager, ObstacleManager* obstacleManager)
	:
	m_players{},
	m_playerMode(playerMode),
	m_stageManager(stageManager),
	m_itemManager(itemManager),
	m_obstacleManager(obstacleManager)
{

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerManager::~PlayerManager()
{
}

/// <summary>
/// 初期化
/// </summary>
void PlayerManager::Initialize()
{
	CreatePlayer();
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void PlayerManager::Update(const DX::StepTimer& timer)
{
	//プレイヤーの更新
	for (std::unique_ptr<Player>& player : m_players)
	{
		player->Update(timer);
	}

	//全プレイヤーの盾の数が-1であればタイマーを止める
	TimerStopCheck();

}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラの生ポインタ</param>
void PlayerManager::Draw(Camera* camera)
{
	//プレイヤーの描画
	for (std::unique_ptr<Player>& player : m_players)
	{
		player->Draw(camera);
	}

	//プレイヤーモードがマルチモードの時
	if (m_playerMode == GameMain::PlayerMode::Player2)
	{
		DirectX::SimpleMath::Vector3 c1;
		DirectX::SimpleMath::Vector3 c2;
		//プレイヤー同士が当たっていた場合押し合いする
		if (CapsuleHit::HitCheck_Capsule2Capsule(*m_players[0]->GetCapsule(), *m_players[1]->GetCapsule(), c1, c2))
		{
			PlayerCapuslePenetration(m_players[0].get(), c2 - c1, m_players[0]->GetCapsule()->r, m_players[1]->GetCapsule()->r);
			PlayerCapuslePenetration(m_players[1].get(), c1 - c2, m_players[1]->GetCapsule()->r, m_players[0]->GetCapsule()->r);
		}
	}
}

/// <summary>
/// 終了処理
/// </summary>
void PlayerManager::Finalize()
{
}

/// <summary>
/// １ｐか２ｐのどちらかの座標を取得
/// </summary>
/// <returns>プレイヤー座標</returns>
DirectX::SimpleMath::Vector3 PlayerManager::GetRandomPlayerPosition()
{
	//プレイヤーの番号
	size_t randomPlayer = static_cast<size_t>(MyRandom::GetIntRange(0, static_cast<int>(m_players.size() - 1)));
	//プレイヤー座標を返す
	return m_players[randomPlayer]->GetPosition();
}

/// <summary>
/// プレイヤーが全員死亡している確認
/// </summary>
/// <returns>true=全員死亡,false=全員死亡していない</returns>
bool PlayerManager::GetPlayerIsAllDaed()
{
	//プレイヤーのデスカウント
	int  deathCount = 0;

	//全てのプレイヤーが死んでいるか確認
	for (std::vector<std::unique_ptr<Player>>::iterator player = m_players.begin(); player != m_players.end(); ++player)
	{
		//アクティブ状態状態ではなければカウントを増やす
		if (!(*player)->IsActive())
		{
			deathCount++;
		}
	}

	//デスカウントがプレイヤーの同じ数であれば全てのプレイヤーが死んでいる
	if (deathCount == m_players.size())
	{
		//全てのプレイヤーが死んでいる
		return true;
	}

	//全てのプレイヤーが死んでいない
	return false;
}

void PlayerManager::PlayerShadow(ShadowMap* shadowMap, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	//プレイヤーの更新
	for (std::unique_ptr<Player>& player : m_players)
	{
		if (!player->IsActive())
			continue;

		player->CreateShadow(shadowMap, view, projection);
	}

}

/// <summary>
/// プレイヤー作成
/// </summary>
void PlayerManager::CreatePlayer()
{
	//プレイヤーのキーデータ
	std::vector<std::vector<DirectX::Keyboard::Keys>> playerKeyData =
	{
		//プレイヤー１のキーデータ(右、左、前、後ろ、ジャンプ)
		{DirectX::Keyboard::Keys::Right,DirectX::Keyboard::Keys::Left,DirectX::Keyboard::Keys::Up,DirectX::Keyboard::Keys::Down,DirectX::Keyboard::Keys::RightShift},
		//プレイヤー２のキーデータ(右、左、前、後ろ、ジャンプ)
		{DirectX::Keyboard::Keys::D,DirectX::Keyboard::Keys::A,DirectX::Keyboard::Keys::W,DirectX::Keyboard::Keys::S,DirectX::Keyboard::Keys::Space}
	};

	//プレイヤーのモデルファイルパス
	std::vector<std::vector<std::wstring>>playerModelFile = {
		//プレイヤー１のモデルファイルパス（アイドル状態、左足出している状態、右足出している状態、ジャンプしている状態）
		{ L"Resources/Models/playeraidoru.cmo",L"Resources/Models/playerhidari.cmo",L"Resources/Models/playermigiasi.cmo",L"Resources/Models/playerjanp.cmo"},
		//プレイヤー２のモデルファイルパス（アイドル状態、左足出している状態、右足出している状態、ジャンプしている状態）
		{L"Resources/Models/Player2idoru.cmo",L"Resources/Models/Player2hidari.cmo",L"Resources/Models/Player2Migi.cmo",L"Resources/Models/Player2Janp.cmo"}
	};

	//二人用プレイヤースタート座標
	DirectX::SimpleMath::Vector3 playersStartPos[2] =
	{
		DirectX::SimpleMath::Vector3{3.0f,0.0f,6.0f} ,
		DirectX::SimpleMath::Vector3{-3.0f,0.0f,6.0f}
	};

	//プレイヤーモードが1人モードなら一人用のスタート座標、キーデータにする
	if (m_playerMode == GameMain::PlayerMode::Player1)
	{
		playersStartPos[0] = DirectX::SimpleMath::Vector3(0.0f, 7.0f, 6.0f);
		playerKeyData[0] = { DirectX::Keyboard::Keys::Right,DirectX::Keyboard::Keys::Left,DirectX::Keyboard::Keys::Up,DirectX::Keyboard::Keys::Down,DirectX::Keyboard::Keys::Space };
	}
	//プレイヤーモードの数プレイヤーを作成
	for (int i = 0; i < static_cast<int>(m_playerMode); i++)
	{
		m_players.push_back(std::make_unique<Player>());

		//ステージマネージャー設定
		m_players[i]->SetStageManeger(m_stageManager);
		//アイテムマネージャー設定
		m_players[i]->SetIteManeger(m_itemManager);
		//障害物マネージャー設定
		m_players[i]->SetObstacleManager(m_obstacleManager);

		//プレイヤーモデルファイルパス設定
		m_players[i]->SetPlayerModelFile(playerModelFile[i]);
		//プレイヤーの移動キー設定
		m_players[i]->SetKeys(playerKeyData[i]);
		//プレイヤーID設定
		m_players[i]->SetTypeInt(i);

		//初期化
		m_players[i]->Initialize(DirectX::SimpleMath::Vector3::Zero, playersStartPos[i], DirectX::SimpleMath::Vector3(2.0f, 2.0f, 2.0f), DirectX::SimpleMath::Vector3::Zero, true, nullptr, nullptr);
	}
}



/// <summary>
/// プレイヤーのめり込み処理
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="cupseleToCupseVector">当たったカプセルとカプセルのベクトル（プレイヤーカプセルから障害物カプセルのベクトル）</param>
/// <param name="playerCapsleRadius">プレイヤーのカプセルの半径</param>
/// <param name="obstacleCupsleRadius">プレイヤー２のカプセルの半径</param>
void PlayerManager::PlayerCapuslePenetration(Actor* player, const DirectX::SimpleMath::Vector3& cupseleToCupseVector, float playerCapsleRadius, float obstacleCupsleRadius)
{
	//当たったカプセルとカプセルの距離
	float cupseleToCupselLenth = cupseleToCupseVector.Length();
	//プレイヤーと障害物の半径の合計
	float radius = playerCapsleRadius + obstacleCupsleRadius;

	DirectX::SimpleMath::Vector3 capToCapVector = cupseleToCupseVector;

	//当たったカプセルとカプセルの距離がプレイヤーと障害物の半径の合計より小さければめり込み処理をする
	if (cupseleToCupselLenth < radius)
	{
		//どれだけめり込んでいるか計算
		float capToCapLengthToRasiusDifference = cupseleToCupselLenth - radius;
		//当たったカプセルとカプセルのベクトルを正規化
		capToCapVector.Normalize();
		//当たったカプセルとカプセルのベクトルにめり込んだ量をかける		
		DirectX::SimpleMath::Vector3 playerPenetrationVelocity = capToCapVector * capToCapLengthToRasiusDifference;
		//プレイヤーのめり込んだ量を押し出す
		player->SetPosition(player->GetPosition() + playerPenetrationVelocity);
	}
	
}

/// <summary>
/// タイマーを止めるか確認する
/// </summary>
void PlayerManager::TimerStopCheck()
{
	int count = 0;

	//プレイヤーの更新
	for (std::unique_ptr<Player>& player : m_players)
	{
		//プレイヤーの盾の数が-1であればカウントを増やす
		if (player->GetShield() <= -1)
		{
			count++;
		}
		
		//全てのプレイヤーが盾の数が-1であればタイマーを止める
		if (count == m_players.size())
		{
			//ゲームコンテキストにプレイヤー死亡したと設定
			GameContext::GetInstance().SetIsPlayerDeath(true);

			//タイマーを止める
			AliveTimer::GetInstance().SetTimerStop(true);

		}
		
	}
}

