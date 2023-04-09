#pragma once
#include"Stage.h"
#include"Game/GameMain.h"
#include <CommonStates.h>
#include<SimpleMath.h>
#include<Model.h>
#include <string>

#include"../IBehavior.h"

#include<GeometricPrimitive.h>

#include"Libraries/Json/json.hpp"

class StageManager
{
public:

	enum class StageSelect
	{
		Stage1,
		Stage2,
		Stage3
	};

private:

	std::unique_ptr<DirectX::Model>           m_stageModel;
	std::unique_ptr<DirectX::GeometricPrimitive>           m_geo;

	std::vector<DirectX::SimpleMath::Vector3> m_stagePositions;

	std::vector<std::unique_ptr<Stage>>       m_stage;

	DirectX::CommonStates*                    m_commonState;
	
	StageSelect m_stageSelect;

	std::vector<std::string> m_stageData;

	std::vector<DirectX::SimpleMath::Vector3> m_baseVertices;
	std::vector<DirectX::SimpleMath::Vector3> m_nowVertices;
	std::vector<std::vector<int>> m_indices;

	std::vector<int> m_stageType;
	
	std::vector<std::unique_ptr<IBehavior>> m_behavior;

public:

	StageManager();
	~StageManager();

	// ������
	void Initialize(DirectX::CommonStates* commonState, StageSelect stage = StageSelect::Stage1);

	// �X�V
	void Update(const DX::StepTimer& timer);

	// �`��
	void Draw(Camera* camera);

	// �I������
	void Finalize();

	bool PlayerStageAABBHitCheck(Actor* player);
	bool ItemHitCheck(Actor* item);

	void SetShadow(ShadowMap* shadow);


	/// <summary>
	/// ���f���쐬
	/// </summary>
	/// <param name="fileName">���f���t�@�C���p�X</param>
	/// <returns>���f���̃��j�[�N�|�C���^�[</returns>
	std::unique_ptr<DirectX::Model> CreateModel(const wchar_t* fileName);

	// �_ c �Ɛ��� ab �̊Ԃ̋����̕����i2 �悵���l�j��Ԃ��֐�
	// a: �����̎n�_
	// b: �����̏I�_
	// c: �_
	// �Ԃ�l: �_ c �Ɛ��� ab �̊Ԃ̋����̕���
	float SqDistPointSegment(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b, DirectX::SimpleMath::Vector3 c)
	{
		DirectX::SimpleMath::Vector3 ab = b - a; // �x�N�g�� ab ���Z�o
		DirectX::SimpleMath::Vector3 ac = c - a; // �x�N�g�� ac ���Z�o
		DirectX::SimpleMath::Vector3 bc = c - b; // �x�N�g�� bc ���Z�o
		// �x�N�g�� ac �ƃx�N�g�� ab �̓��ς��v�Z
		float e = ac.Dot(ab);
		if (e <= 0.0f)
		{
			// c ���ˉe�����_�� a ���֊O��Ă���̂ŁAa �� c �̋�����Ԃ�
			return ac.Dot(ac);
		}
		// ab ���m�̓��ς��v�Z����
		float f = ab.Dot(ab);
		if (e >= f)
		{
			// c ���ˉe�����_�� b ���֊O��Ă���̂ŁAb �� c �̋�����Ԃ�
			return bc.Dot(bc);
		}
		// c �� c ���ˉe���� ab ��̓_�Ƃ̋�����Ԃ�
		return ac.Dot(ac) - e * e / f;
	}


	// ���ƃJ�v�Z���̏Փ˔���֐�
	bool HitCheck_Sphere2Capsule(Sphere sphere, Capsule capsule)
	{
		// ���̒��S�ƃJ�v�Z���̒��S�̐����Ƃ̋����̕������v�Z
		float dist2 = SqDistPointSegment(capsule.a, capsule.b, sphere.c);
		// ���̔��a�ƃJ�v�Z���̔��a�̍��v���Z�o
		float radius = sphere.r + capsule.r;
		// dist2 �� radius �� 2 ��̌��ʈȉ��ƂȂ��Ă���΁A�������Ă���
		return dist2 <= radius * radius;
	}

	/// <summary>
	/// �����Ɣ|���S���̓����蔻��
	/// </summary>
	/// <param name="vertices">���_���W</param>
	/// <param name="linePos">�����̗��[���W</param>
	/// <param name="normalVector">�@���x�N�g���̃|�C���^</param>
	/// <returns>true=�������Ă���@false=�����Ă��Ȃ�</returns>
	bool StageHitCheck(std::vector<DirectX::SimpleMath::Vector3> vertices, std::vector<DirectX::SimpleMath::Vector3> linePos, DirectX::SimpleMath::Vector3* normalVector);
	private:

	void CreateBehavior();

	void UpdateVertices();

	/// <summary>
	/// �X�e�[�Wjson��ǂݍ���
	/// </summary>
	/// <param name="fileName">json�̃p�X</param>
	void LoadStageJson(const std::wstring& fileName);

	/// <summary>
	/// string��wstring�ɕϊ�����
	/// </summary>
	/// <param name="str">�}���`�o�C�g������</param>
	/// <returns>���C�h������</returns>
	std::wstring ConvertWString(const std::string& str);

	void ChackStageMoveEnd();

};