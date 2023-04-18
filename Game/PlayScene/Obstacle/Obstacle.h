#pragma once 
#include"../Actor/Actor.h"
#include"../../Shader/FireEffectManager.h"
#include"../ShadowMap.h"
#include"../Objects.h"
#include <GeometricPrimitive.h>


class Obstacle :public Actor
{
public:
	//障害物タイプの列挙体
	enum class ObstacleType
	{
		//普通の炎
		NORMAL,
		//棒
		STICK,
		//隕石
		METEORITE, 
		//時計回りに回転する棒
		ROTATESTICK,
		//反時計回りに回転する棒
		REVERSE_ROTATESTICK,
		//蛇行する炎
		MEANDERING,
		//鳥
		BIRD,
		
		NONE
		
	};
private:
	//定数
	//移動速度
	static const float MOVE_SPEED;


	//変数
	//障害物のタイプ
	ObstacleType                                 m_type;
	//炎のエフェクト
	FireEffectManager*                           m_effect;
	

	DirectX::SimpleMath::Vector3                 m_scale;

	float                                        m_rotSpeed;
	float                                        m_meanderingSpeed;


	float                                        m_mass;           // 質量
	float                                        m_maxSpeed;       // 最大移動速度(スピード)
	float                                        m_maxForce;       // 適用される最大の力

	DirectX::SimpleMath::Vector3                 m_force;       // 物体に加わる力

		// 徘徊行動用パラメータ
	float                                        m_wanderRadius;
	float                                        m_wanderDistance;
	float                                        m_wanderAngularVelocity;
	float                                        m_wanderAngle;
	DirectX::SimpleMath::Vector3                 m_playerPosition;
	float                                        m_seekTime_s;
public :
	//コンストラクタ
	Obstacle();
	//デストラクタ
	virtual ~Obstacle()override;

	// 初期化
	virtual void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& scale,
		bool active,
		float angle,
		IBehavior* behavia,
		DirectX::Model* model,
		DirectX::CommonStates* commonState)override;

	// 更新
	virtual void Update(const DX::StepTimer& timer)override;

	// 描画
	virtual void Draw(Camera* camera)override;

	// 終了処理
	virtual void Finalize()override;

	//リセット
	virtual void Reset();


	void SetType(ObstacleType type) { m_type = type; }

	void ObstacleShadow(ShadowMap* shadow, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	void SetEffect(FireEffectManager* effect) { m_effect = effect; }
	FireEffectManager* GetEffect() { return m_effect; }
	ObstacleType GetObstacleType() { return m_type; }

	float& GetRotSpeed() { return m_rotSpeed; }
	void SetRotSpeed(float speed) { m_rotSpeed = speed; }
	float GetMeanderingSpeed() { return m_meanderingSpeed; }


	DirectX::SimpleMath::Vector3 GetPlayerPosition() { return m_playerPosition; }
	void SetPlayerPosition(DirectX::SimpleMath::Vector3 playerPos) { m_playerPosition = playerPos; }


	DirectX::SimpleMath::Vector3 Seek(const DirectX::SimpleMath::Vector3& targetPosition);

	float GetMaxSpeed() const { return m_maxSpeed; }
	float GetMaxForce() const { return m_maxForce; }
	void SetMaxSpeed(float speed) { m_maxSpeed = speed; }
	void SetMaxForce(float force) { m_maxForce = force; }

	

	float GetSeekTime() { return m_seekTime_s; }

private:
	bool  CheckInArea();


	
};