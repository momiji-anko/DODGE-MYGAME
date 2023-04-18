#pragma once 
#include"../Actor/Actor.h"
#include"../ShadowMap.h"
#include"Game/PlayScene/Objects.h"
#include"Game/PlayScene/IBehavior.h"

class Stage :public Actor
{
public:
	enum class StageType
	{
		Stage1_1,
		Stage1_2,
		Stage1_3,
		Stage1_4,

		Stage2_1,

		Stage3_1,
		
		
		NONE

	};
private:
	//定数
	static const float MOVE_SPEED;
	static const float MOVE_TIME;
	//変数
	int m_routine;
	StageType m_type;
	DirectX::SimpleMath::Vector3 m_offsetPosition;
	float m_time;
	ShadowMap* m_shadowMap;
	bool m_endFlag;

	float m_rotationTime_s;
	DirectX::SimpleMath::Vector3 m_previousRotation;
	DirectX::SimpleMath::Vector3 m_nextRotation;
	bool m_isRotation;
	
	IBehavior* m_behavior;

	
public:
	void SetTime(float time) { m_time = time; }
	float GetTime() { return m_time; }

	void SetEndFlag(bool flag) { m_endFlag = flag; }
	bool GetEndFlag() { return m_endFlag; }

	void SetRotationTime(float time) { m_rotationTime_s = time; }
	float GetRotationTime() { return m_rotationTime_s; }

	void SetOffsetPosition(DirectX::SimpleMath::Vector3& position) { m_offsetPosition = position; }
	DirectX::SimpleMath::Vector3 GetOffsetPosition() { return m_offsetPosition; }

	void SetRoutine(int routine) { m_routine = routine; }
	float GetRoutine() { return m_routine; }

	void SetNextRotation(DirectX::SimpleMath::Vector3 rotetion) { m_nextRotation = rotetion;}
	DirectX::SimpleMath::Vector3 GetNextRotation() { return m_nextRotation; }

	void SetPreviousRotation(DirectX::SimpleMath::Vector3 rotetion) { m_previousRotation = rotetion; }
	DirectX::SimpleMath::Vector3 GetPreviousRotation() { return m_previousRotation; }

	bool  IsRotation() { return m_isRotation;}
	void SetIsRotation(bool flag) { m_isRotation = flag; }


	

	//コンストラクタ
	Stage();
	//デストラクタ
	 ~Stage()override;

	// 初期化
	 void Initialize(
		 const DirectX::SimpleMath::Vector3& velocity,
		 const DirectX::SimpleMath::Vector3& position,
		 const DirectX::SimpleMath::Vector3& scale,
		 bool active,
		float angle,
		IBehavior* behavia,
		DirectX::Model* model,
		DirectX::CommonStates* commonState)override;

	// 更新
	 void Update(const DX::StepTimer& timer)override;

	// 描画
	 void Draw(Camera* camera)override;

	// 終了処理
	 void Finalize()override;

	 void SetStageType(Stage::StageType type) { m_type = type; }

	 bool PlayerCheckGruondHit(AABBFor3D* playerAABB);

	 float Lerp(float start, float end, float t)
	 {
		 return start + (end - start) * t;
	 }
	 void SetShadow(ShadowMap* shadowMap) { m_shadowMap = shadowMap; }


	 void Reset();




	
};