#pragma once 
#include"../Actor/Actor.h"
#include"../../EffectManager3.h"
#include"../ShadowMap.h"
#include"../Objects.h"
#include <GeometricPrimitive.h>


class Obstacle :public Actor
{
public:
	//列挙体
	enum class ObstacleType
	{
		NORMAL,
		STICK,
		METEORITE, 
		ROTATESTICK,
		REVERSE_ROTATESTICK,
		MEANDERING,
		
		STRAIGHT_MOVE_RIGHT_LEFT,
		STRAIGHT_MOVE_LEFT_RIGHT,
		
		STRAIGHT_MOVE_TOP_BOTTOM,
		STRAIGHT_MOVE_BOTTOM_TOP,

		BIRD,
		
	};
private:
	//定数
	static const float MOVE_SPEED;

	//変数
	ObstacleType m_type;
	EffectManager3* m_effect;
	std::unique_ptr<DirectX::GeometricPrimitive> m_geo;
	DirectX::SimpleMath::Matrix m_geoWorld;

	DirectX::SimpleMath::Vector3 m_scale;

	float m_rotSpeed;
	float m_meanderingSpeed;

public :
	//コンストラクタ
	Obstacle();
	//デストラクタ
	virtual ~Obstacle()override;

	// 初期化
	virtual void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
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

	void SetAABB(AABBFor3D* aabb) { m_AABBObject.reset(aabb); }

	void SetType(ObstacleType type) { m_type = type; }

	void ObstacleShadow(ShadowMap* shadow, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	void SetEffect(EffectManager3* effect) { m_effect = effect; }

	ObstacleType GetObstacleType() { return m_type; }

	float& GetRotSpeed() { return m_rotSpeed; }
	void SetRotSpeed(float speed) { m_rotSpeed = speed; }
	float& GetMeanderingSpeed() { return m_meanderingSpeed; }

	DirectX::SimpleMath::Vector3 GetScale(){ return m_scale; }
	void SetScale(DirectX::SimpleMath::Vector3 scale) { m_scale = scale; }
};