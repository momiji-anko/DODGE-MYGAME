#pragma once
#include<map>
#include <SpriteBatch.h>
#include"../Actor/Actor.h"
#include"../../EffectManager.h"
#include<map>
#include"../ShadowMap.h"

#include"Libraries/AXD2/Adx2.h"
#include"../../../Helpers/CueSheet_0.h"
#include"Helpers/DODGESound_acf.h"
#include"../Objects.h"
#include"../Item/ItemManeger.h"
#include"../Stage/StageManager.h"

enum class PlayerState
{
	INVINCIBLE,
	SLIP,
	NORMAL,

};

class Player :public Actor
{
private:


	//定数=====================================================

	static const float  MOVE_SPEED;					    //	1秒間に進むマスの数
	static const float GRAVITY_FORCE;					//	1秒間に落ちるマスの数
	static const float JUMP_FORCE;					    //	ジャンプ力
	static const float INVINCIBLE_TIME_SECONDS;	        //無敵時間

	//変数=====================================================

	//プレイヤーの状態
	PlayerState m_playerState;
	//無敵時間
	float m_invincbleTime;

	std::unique_ptr<EffectManager> m_effect;



	//プレイヤーのモデル
	std::map<int, std::unique_ptr<DirectX::Model>> m_playerModel;
	std::vector<int>                               m_playerModelNum;
	float                                          m_modelTime;
	
	//音
	ADX2* m_pAdx2;
	int   m_musicID;
	int   m_jumpmusicID;

	//ジャンプ
	float m_jumpVelcity;
	//吹き飛ばす
	DirectX::SimpleMath::Vector3 m_flyVelocity;
	
	//
	int   m_invalidCount;
	float m_invalidCountCoolDownTime_s;
	bool  m_isInvalid;
	std::unique_ptr<DirectX::Model> m_barrierModel;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_invalidTexture;
	//========
	ItemManeger*       m_itemManger;
	StageManeger*      m_stageManeger;

	//プレイヤーの番号
	int m_playerID;
public:
	//コンストラクタ
	Player();
	//デストラクタ	
	~Player()override;
	// 初期化
	void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
		bool active,
		float angle,
		IBehavior* behavia,
		DirectX::Model* model, 
		DirectX::CommonStates* commonState)override;

	// 更新
	void Update(const DX::StepTimer& timer)override;

	// 描画
	void Draw(Camera* camera)override;
	//盾の画像描画
	void TextureDraw(DirectX::SpriteBatch* spriteBatch);

	// 終了処理
	void Finalize()override;



	PlayerState GetNowPlayerState() { return m_playerState; }

	void PlayerShadow(ShadowMap* shadowMap,DirectX::SimpleMath::Matrix view,DirectX::SimpleMath::Matrix projection);

	void InvalidTime();

	void SetFlyVelocity(DirectX::SimpleMath::Vector3 velocity) { m_flyVelocity = velocity; }


	void SetIteManeger(ItemManeger* itemManeger) { m_itemManger = itemManeger; }
	void SetStageManeger(StageManeger* stageManeger) { m_stageManeger = stageManeger;  }

	void InvalidCountUp();
	void InvalidCountDown();
	int  GetInvalidCount() { return m_invalidCount; }

	int GetID() { return m_playerID; }
	void SetID(int id) { m_playerID = id; }

private: 
	void Player1Move(const DX::StepTimer& timer);
	void Player2Move(const DX::StepTimer& timer);

	void Player1CreateModel();
	void Player2CreateModel();

};
