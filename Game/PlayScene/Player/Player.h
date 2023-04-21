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


class Player :public Actor
{
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

	//変数=====================================================

	//プレイヤーの状態
	PlayerState m_playerState;
	//無敵時間
	float m_invincbleTime;

	//プレイヤーのモデル連想配列
	std::vector< DirectX::Model*> m_playerModel;
	//プレイヤーの表示するモデルの番号
	std::vector<int>                               m_playerModelNum;
	//モデルを切り替えるタイム
	float                                          m_modelTime_s;
	
	//音
	ADX2* m_pAdx2;
	//ジャンプのSEのID
	int   m_jumpMusicID;

	//吹き飛ばす
	DirectX::SimpleMath::Vector3 m_flyVelocity;
	
	//持っている盾の数
	int   m_shieldCount;
	//ダメージを受けた時の無敵時間
	float m_invincibleCountCoolDownTime_s;
	//盾を持っているか
	bool  m_isShield;

	std::unique_ptr<DirectX::Model> m_barrierModel;

	//盾のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shieldTexture;

	//プレイヤーとアイテムの当たり判定用のアイテムマネージャーのポインター
	ItemManager*       m_itemManager;
	//プレイヤーとステージの当たり判定用のステージマネージャーのポインター
	StageManager*      m_stageManager;

	//プレイヤーの番号
	int m_playerID;

	std::unique_ptr<Blink> m_blink;
	
	std::vector<DirectX::Keyboard::Keys> m_keys;

	std::vector<std::wstring> m_modelFiles;

public:

	//コンストラクタ
	Player();
	//デストラクタ	
	~Player()override;

	void SetKeys(std::vector<DirectX::Keyboard::Keys>& keys) { m_keys = keys; }

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
	void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Vector3& rotation,
		bool active,
		IBehavior* behavia,
		DirectX::Model* model,
		DirectX::CommonStates* commonState
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
	/// <param name="spriteBatch">スプライトバッチ</param>
	void TextureDraw(DirectX::SpriteBatch* spriteBatch);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// プレイヤーの状態を取得
	/// </summary>
	/// <returns>プレイヤーの状態</returns>
	PlayerState GetNowPlayerState() { return m_playerState; }


	/// <summary>
	/// 回転する棒の障害物に当たった時に吹き飛ばされる用のベロシティ
	/// </summary>
	/// <param name="velocity">飛ばされるベロシティ</param>
	void SetFlyVelocity(DirectX::SimpleMath::Vector3 velocity) { m_flyVelocity = velocity; }

	/// <summary>
	/// アイテムマネージャーを設定
	/// </summary>
	/// <param name="itemManeger">アイテムマネージャーの生ポインター</param>
	void SetIteManeger(ItemManager* itemManeger) { m_itemManager = itemManeger; }

	/// <summary>
	///　ステージマネージャーの設定
	/// </summary>
	/// <param name="stageManeger">ステージマネージャーの生ポインター</param>
	void SetStageManeger(StageManager* stageManeger) { m_stageManager = stageManeger;  }


	/// <summary>
	/// 持っている盾の数を１増やす
	/// </summary>
	void ShieldCountUp();
	/// <summary>
	/// 持っている盾の数を１減らす　盾を一個も持っていない場合死亡させる
	/// </summary>
	void ShieldCountDown();

	/// <summary>
	/// プレイヤーが持っている盾の数を取得
	/// </summary>
	/// <returns>盾の数</returns>
	int  GetInvalidCount() { return m_shieldCount; }

	/// <summary>
	/// プレイヤーのID取得
	/// </summary>
	/// <returns>プレイヤーのID</returns>
	int GetID() { return m_playerID; }

	/// <summary>
	/// プレイヤーのIDを設定
	/// </summary>
	/// <param name="id">プレイヤーのID</param>
	void SetID(int id) { m_playerID = id; }

	/// <summary>
	/// プレイヤーモデルのファイルパスの設定
	/// </summary>
	/// <param name="files">モデルファイルパス配列</param>
	void SetPlayerModelFile(std::vector<std::wstring> files) { m_modelFiles = files; }

private: 

	/// <summary>
	/// プレイヤーのモデル作成
	/// </summary>
	void CreatePlayerModel();

	

	/// <summary>
	/// プレイヤーの動き
	/// </summary>
	/// <param name="timer">タイマー</param>
	void PlayerMove(const DX::StepTimer& timer);

};
