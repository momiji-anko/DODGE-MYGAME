#pragma once 
#include"../Actor/Actor.h"
#include"../ShadowMap.h"
#include"Game/PlayScene/Objects.h"
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
	//�萔
	static const float MOVE_SPEED;
	static const float MOVE_TIME;
	//�ϐ�
	int m_routine;
	StageType m_type;
	DirectX::SimpleMath::Vector3 m_offsetPosition;
	float m_time;
	ShadowMap* m_shadowMap;
	bool m_endFlag;

	//std::unique_ptr<Sphere> sphere;
public:
	//�R���X�g���N�^
	Stage();
	//�f�X�g���N�^
	 ~Stage()override;

	// ������
	 void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
		bool active,
		float angle,
		IBehavior* behavia,
		DirectX::Model* model,
		DirectX::CommonStates* commonState)override;

	// �X�V
	 void Update(const DX::StepTimer& timer)override;

	// �`��
	 void Draw(Camera* camera)override;

	// �I������
	 void Finalize()override;

	 void SetStageType(Stage::StageType type) { m_type = type; }

	 bool PlayerCheckGruondHit(AABBFor3D* playerAABB);

	 float Lerp(float start, float end, float t)
	 {
		 return start + (end - start) * t;
	 }
	 void SetShadow(ShadowMap* shadowMap) { m_shadowMap = shadowMap; }

	 bool GetEndFlag() { return m_endFlag; }

	 void Reset();
private: 
	void Stage1_1();
	void Stage1_2();
	void Stage1_3();
	void Stage1_4();
};