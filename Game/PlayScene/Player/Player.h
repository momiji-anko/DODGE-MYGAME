/*
* 2023/04/23
* Player.h
* プレイヤー
* 麻生　楓
*/
#pragma once
#include<map>
#include <SpriteBatch.h>
#include"../Actor/Actor.h"
#include<map>
#include"../ShadowMap.h"

#include"Libraries/AXD2/Adx2.h"
#include"../../../Helpers/CueSheet_0.h"
#include"Helpers/DODGESound_acf.h"
#include"../Objects.h"
#include"../Item/ItemManager.h"
#include"../Stage/StageManager.h"
#include<WICTextureLoader.h>
#include"Game/PlayScene/Blink.h"
#include"Game/PlayScene/Obstacle/ObstacleManager.h"

/// <summary>
/// プレイヤー
/// </summary>
class Player :public Actor
{
public:
	/// <summary>
	/// 移動キーの設定
	/// </summary>
	/// <param name="keys"></param>
	void SetKeys(const std::vector<DirectX::Keyboard::Keys>& keys) { m_keys = keys; }

	/// <summary>
	/// 回転する棒の障害物に当たった時に吹き飛ばされる用のベロシティ
	/// </summary>
	/// <param name="velocity">飛ばされるベロシティ</param>
	void SetFlyVelocity(const DirectX::SimpleMath::Vector3& velocity) { m_flyVelocity = velocity; }

	/// <summary>
	/// アイテムマネージャーを設定
	/// </summary>
	/// <param name="itemManeger">アイテムマネージャーの生ポインター</param>
	void SetIteManeger(ItemManager* itemManeger) { m_itemManager = itemManeger; }

	/// <summary>
	///　ステージマネージャーの設定
	/// </summary>
	/// <param name="stageManeger">ステージマネージャーの生ポインター</param>
	void SetStageManeger(StageManager* stageManeger)override { m_stageManager = stageManeger; }

	/// <summary>
	///　障害物マネージャーの設定
	/// </summary>
	/// <param name="stageManeger">障害物マネージャーの生ポインター</param>
	void SetObstacleManager(ObstacleManager* obstacleManager) { m_obstacleManager = obstacleManager; }

	/// <summary>
	/// プレイヤーが持っている盾の数を取得
	/// </summary>
	/// <returns>盾の数</returns>
	int  GetInvalidCount()const { return m_shieldCount; }

	/// <summary>
	/// タイプを整数型で取得
	/// </summary>
	/// <returns>アイテムタイプ</returns>
	int GetTypeInt() const override { return m_playerID; }
	/// <summary>
	/// タイプを整数型で設定
	/// </summary>
	/// <param name="type">タイプ</param>
	void SetTypeInt(int type) override { m_playerID = type; }

	/// <summary>
	/// プレイヤーモデルのファイルパスの設定
	/// </summary>
	/// <param name="files">モデルファイルパス配列</param>
	void SetPlayerModelFile(const std::vector<std::wstring>& files) { m_modelFiles = files; }

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player()override;


	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="velocity">移動量</param>
	/// <param name="position">初期座標</param>
	/// <param name="active">存在しているか</param>
	/// <param name="angle">角度</param>
	/// <param name="behavia">ビヘイビアー（NULLでOK）</param>
	/// <param name="model">プレイヤーのモデルだがNULLでOK</param>
	void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Vector3& rotation,
		bool active,
		IBehavior* behavia,
		DirectX::Model* model
	)override;
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	void Update(const DX::StepTimer& timer)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(Camera* camera)override;
	
	/// <summary>
	/// 盾UIの描画
	/// </summary>
	void TextureDraw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// 持っている盾の数を１増やす
	/// </summary>
	void ShieldCountUp();
	/// <summary>
	/// 持っている盾の数を１減らす　盾を一個も持っていない場合死亡させる
	/// </summary>
	void ShieldCountDown();

	/// <summary>
	/// 影生成
	/// </summary>
	/// <param name="shadow">シャドウマップの生ポインタ</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="projection">プロジェクション行列</param>
	void CreateShadow(ShadowMap* shadow, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)override;

	/// <summary>
	/// リセット
	/// </summary>
	void Reset()override;

private:

	/// <summary>
	/// プレイヤーのモデル作成
	/// </summary>
	void CreatePlayerModel();

	/// <summary>
	/// 当たり判定の領域の更新
	/// </summary>
	void HitAreaUpdate();

	/// <summary>
	/// プレイヤーの動き
	/// </summary>
	/// <param name="timer">タイマー</param>
	void PlayerMove(const DX::StepTimer& timer);

public:

	//プレイヤーの状態
	enum class PlayerState
	{
		//無敵状態
		INVINCIBLE,
		//普通の状態
		NORMAL,
	};

private:

	//定数=====================================================
	//	1秒間に進むマスの数
	static const float  MOVE_SPEED;
	//	1秒間に落ちるマスの数
	static const float GRAVITY_FORCE;
	//	ジャンプ力
	static const float JUMP_FORCE;
	//無敵時間
	static const float INVINCIBLE_TIME_SECONDS;
	//上のAABB
	static const DirectX::SimpleMath::Vector3 AABB_AREA_UP;
	//下のAABB
	static const DirectX::SimpleMath::Vector3 AABB_AREA_DOWN;
	//上のカプセルの長さ
	static const float CAPSULE_AREA_UP;
	//下のカプセルの長さ
	static const float CAPSULE_AREA_DOWN;
	//落ちた際の死亡する高さ
	static const float FALL_DEAD_AREA;
	//モデルタイムの最大数
	static const float MAX_MODEL_TIME_S;
	//モデルタイムの速度
	static const float MODEL_TIME_SPEED;

	//プレイヤーのシールド画面表示位置
	static const DirectX::SimpleMath::Vector2 PLAYERS_SHIELD_TEXTURE_POSITION[2];


	//変数=====================================================

	//無敵時間
	float m_invincbleTime;

	//プレイヤーのモデル連想配列
	std::vector<DirectX::Model*> m_playerModel;
	//プレイヤーの表示するモデルの番号
	std::vector<int>              m_playerModelNum;
	//モデルを切り替えるタイム
	float                         m_modelTime_s;

	//音
	ADX2* m_pAdx2;
	//ジャンプのSEの記憶用ID
	int   m_jumpMusicID;

	//吹き飛ばす
	DirectX::SimpleMath::Vector3 m_flyVelocity;

	//持っている盾の数
	int   m_shieldCount;
	//ダメージを受けた時の無敵時間
	float m_invincibleCountCoolDownTime_s;

	//盾のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shieldTexture;

	//プレイヤーとアイテムの当たり判定用のアイテムマネージャーのポインター
	ItemManager* m_itemManager;
	//プレイヤーとステージの当たり判定用のステージマネージャーのポインター
	StageManager* m_stageManager;
	//プレイヤーと障害物当たり判定用の障害物マネージャーのポインター
	ObstacleManager* m_obstacleManager;

	//プレイヤーの番号
	int m_playerID;

	//点滅
	std::unique_ptr<Blink> m_blink;
	//プレイヤーの移動キー
	std::vector<DirectX::Keyboard::Keys> m_keys;

	//プレイヤーのモデルファイルパス
	std::vector<std::wstring> m_modelFiles;

};
