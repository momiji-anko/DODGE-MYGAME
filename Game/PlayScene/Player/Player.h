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
//#include"../Obstacle/ObstacleManeger.h"
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

	PlayerState m_playerState;
	float m_invincbleTime;
	std::unique_ptr<EffectManager> m_effect;
	bool m_hit;
	bool m_itemHit;

	std::map<int, std::unique_ptr<DirectX::Model>> m_playerModel;
	std::vector<int> m_playerModelNum;
	float m_modelTime;
	

	ADX2* m_pAdx2;
	int m_musicID;
	int m_jumpmusicID;

	float m_jumpVelcity;

	DirectX::SimpleMath::Vector3 m_flyVelocity;
	
	//std::map<PlayerState,int>
	int m_invalidCount;
	float m_invalidCountCoolDownTime_s;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_invalidTexture;
	bool m_isInvalid;

	//========
	ItemManeger* m_itemManger;
	//ObstacleManeger* m_obstacleManeger;
	StageManeger* m_stageManeger;

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

	void TextureDraw(DirectX::SpriteBatch* spriteBatch);

	// 終了処理
	void Finalize()override;

	void SetGruondHit(bool hit) { m_hit = hit; }

	void SetItemHit(bool hit) { m_itemHit = hit; }

	PlayerState GetNowPlayerState() { return m_playerState; }

	void PlayerShadow(ShadowMap* shadowMap,DirectX::SimpleMath::Matrix view,DirectX::SimpleMath::Matrix projection);

	void InvalidTime();

	void SetFlyVelocity(DirectX::SimpleMath::Vector3 velocity) { m_flyVelocity = velocity; }


	void SetIteManeger(ItemManeger* itemManeger) { m_itemManger = itemManeger; }
	//void SetObstacleManeger(ObstacleManeger* obstacleManeger) { m_obstacleManeger = obstacleManeger; }
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
