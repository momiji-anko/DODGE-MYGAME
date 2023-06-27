/*
* 2023/04/26
* StageManager.cpp
* �X�e�[�W�}�l�[�W���[
* �����@��
*/
#include"pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include"StageManager.h"
#include"DeviceResources.h"
#include"../MyRandom.h"
#include<stdlib.h>
#include"Libraries/MyLibraries/ModelManager.h"
#include <windows.h>
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Game/PlayScene/GameContext/GameContext.h"

//�r�w�C�r�A�[�̃C���N���[�h
#include"StageBehaviors/FirstFloorToFallBehavior.h"
#include"StageBehaviors/SecondFloorToFallBehavior.h"
#include"StageBehaviors/ThirdFloorToFallBehavior.h"
#include"StageBehaviors/NormalFloorBehavior.h"
#include"StageBehaviors/TiltingFloorbehavior.h"
#include"StageBehaviors/RotationCubeBehavior.h"


/// <summary>
/// �X�e�[�W�ɃV���h�E�}�b�v��n��
/// </summary>
/// <param name="shadow">�V���h�E�}�b�v�̐��|�C���^</param>
void StageManager::SetShadowMap(ShadowMap* shadow)
{
	for (std::unique_ptr<Stage>& stage : m_stage)
	{
		stage->SetShadowMap(shadow);
	}
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
StageManager::StageManager()
	:
	m_stage{},
	m_behavior{}
{
	

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
StageManager::~StageManager()
{
}

/// <summary>
/// ������
/// </summary>
/// <param name="stage">�X�e�[�W�I��</param>
void StageManager::Initialize( StageSelect stage)
{

	//�r�w�C�r�A�[�쐬
	CreateBehavior();


	//�X�e�[�Wjson�t�@�C���p�X
	std::vector<std::wstring> stageFileNames;

	//�X�e�[�W��json������t�@�C���p�X
	std::wstring stageFilePath = L"Resources/StageData/";
	
	//�t�@�C����ǂݍ���
	stageFileNames = FileLoadManager::GetInstance().LoadFile(stageFilePath);

	size_t stageNumber = static_cast<size_t>(stage);

	//json��ǂݍ��݃X�e�[�W���쐬
	LoadStageJson(stageFileNames[stageNumber]);

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void StageManager::Update(const DX::StepTimer& timer)
{
	//�X�e�[�W���s�����I�����Ă��邩�m�F
	CheckStageMoveEnd();

	if (GameContext::GetInstance().GetIsPlayerDeath() == false)
		return;


	//�X�e�[�W�̍X�V
	for (int i = 0; i < m_stage.size(); i++)
	{
		m_stage[i]->Update(timer);
	}

}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̐��|�C���^</param>
void StageManager::Draw(Camera* camera)
{
	//�`��
	for (std::unique_ptr<Stage>& stage : m_stage)
	{
		stage->Draw(camera);
	}

}

/// <summary>
/// �I������
/// </summary>
void StageManager::Finalize()
{

}

/// <summary>
/// �r�w�C�r�A�[�쐬
/// </summary>
void StageManager::CreateBehavior()
{
	//�r�w�C�r�A�[�̐�
	static const int BEHAVIOR_NUM = 6;

	m_behavior.resize(BEHAVIOR_NUM);
	//��Ԗڂɗ����鏰�̃r�w�C�r�A�[
	m_behavior[0] = std::make_unique<FirstFloorToFallBehavior>();
	//��Ԗڂɗ����鏰�̃r�w�C�r�A�[
	m_behavior[1] = std::make_unique<SecondFloorToFallBehavior>();
	//�O�Ԗڂɗ����鏰�̃r�w�C�r�A�[
	m_behavior[2] = std::make_unique<ThirdFloorToFallBehavior>();
	//���ʂ̏�
	m_behavior[3] = std::make_unique<NormalFloorBehavior>();
	//�X����
	m_behavior[4] = std::make_unique<TiltingFloorbehavior>();
	//��]����L���[�u
	m_behavior[5] = std::make_unique<RotationCubeBehavior>();

}


/// <summary>
/// �X�e�[�Wjson��ǂݍ���
/// </summary>
/// <param name="fileName">json�̃p�X</param>
void StageManager::LoadStageJson(const std::wstring& fileName)
{
	//fstream�쐬
 	std::fstream file(fileName, std::ifstream::in);
	//json�ǂݍ���
	nlohmann::json stageJson = nlohmann::json::parse(file);
	//�t�@�C�������
	file.close();

	//�X�e�[�W�̍��W
	std::vector<DirectX::SimpleMath::Vector3> stagePositions;

	//�x�[�X���_���W
	std::vector<DirectX::SimpleMath::Vector3> baseVertices;
	//�C���f�b�N�X
	std::vector<std::vector<int>> indices;
	//�X�e�[�g�^�C�v
	std::vector<int> stageType;

	//�X�e�[�W�̐��擾
	int stageNum = stageJson["StageNum"];
	//�X�e�[�W�̐����m��
	stagePositions.resize(stageNum);
	//�X�e�[�W�̐����m��
	stageType.resize(stageNum);

	//�X�e�[�W���̏���ǂݍ���
	for (int i = 0; i < stageNum; i++)
	{
		//�ԍ����X�g�����O�ɂ���
		std::string str = std::to_string(i);
		//�X�e�[�W�^�C�v�擾
		stageType[i] = stageJson["Stage"][str.c_str()]["StageType"];
		//�X�e�[�W�̃|�W�V�������擾
		stagePositions[i].x = stageJson["Stage"][str.c_str()]["X"];
		stagePositions[i].y = stageJson["Stage"][str.c_str()]["Y"];
		stagePositions[i].z = stageJson["Stage"][str.c_str()]["Z"];
	}

	//���_�̐��擾
	int vertexNum = stageJson["VertexNum"];
	//���_�̐����m��
	baseVertices.resize(vertexNum);
	
	//���_���W�̎擾
	for (int i = 0; i < vertexNum; i++)
	{
		//�ԍ����X�g�����O�ɂ���
		std::string str = std::to_string(i);
		//���_���W�̎擾
		baseVertices[i].x = stageJson["VertexPosition"][str.c_str()]["X"];
		baseVertices[i].y = stageJson["VertexPosition"][str.c_str()]["Y"];
		baseVertices[i].z = stageJson["VertexPosition"][str.c_str()]["Z"];
	}

	

	//���_���W�C���f�b�N�X�̐��擾
	int vertexIndex = stageJson["VertexIndexNum"];

	//���_�C���f�b�N�X�̐����m��
	indices.resize(vertexIndex);

	//���_�C���f�b�N�X�̓ǂݍ���
	for (int i = 0; i < vertexIndex; i++)
	{
		//�C���f�b�N�X�̔z��m��
		indices[i].resize(3);
		//�ԍ����X�g�����O�ɂ���
		std::string str = std::to_string(i);
		//�C���f�b�N�X�ԍ��̎擾
		indices[i][0] = stageJson["VertexIndex"][str.c_str()]["1"];
		indices[i][1] = stageJson["VertexIndex"][str.c_str()]["2"];
		indices[i][2] = stageJson["VertexIndex"][str.c_str()]["3"];
	}

	//���f���̃t�@�C���̃p�X���擾�istring�j
	std::string modelFileName = stageJson["StageModelFileName"];
	//�擾�����t�@�C���p�X��string����wstring�ɕϊ�����
	std::wstring modelFile = ConvertWString(modelFileName);
	//���f���쐬
	DirectX::Model* stageModel = ModelManager::GetInstance().LoadModel(modelFile.c_str());

	//�X�e�[�W�̐����z����m��
	m_stage.resize(stageNum);
	
	DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3::One;

	//�X�e�[�W�̍쐬
	for (int i = 0; i < stageNum; i++)
	{
		//�X�e�[�W�̃��j�[�N�|�C���^�ō쐬
		m_stage[i] = std::make_unique<Stage>();

		//�X�e�[�W�^�C�v
		m_stage[i]->SetTypeInt(stageType[i]);
		//�x�[�X���_���W
		m_stage[i]->SetBaseVertices(baseVertices);
		//�C���f�b�N�X
		m_stage[i]->SetIndices(indices);

		//�ǂݍ��񂾃X�e�[�W���ŃX�e�[�W������������
		m_stage[i]->Initialize(DirectX::SimpleMath::Vector3::Zero, stagePositions[i], scale, DirectX::SimpleMath::Vector3::Zero, true, m_behavior[stageType[i]].get(), stageModel);
		
		
	}

}

/// <summary>
/// string��wstring�ɕϊ�����
/// </summary>
/// <param name="str">�}���`�o�C�g������</param>
/// <returns>���C�h������</returns>
std::wstring StageManager::ConvertWString(const std::string& str)
{
	//�������擾�p�ϐ��쐬�i�����ŕK�v�Ȃ��߁j
	size_t strLengthNum;
	//������o�b�t�@�쐬
	wchar_t* buffer = new wchar_t[str.size() + 1];
	//string��wchar_t*�ɂ��ĕ�����o�b�t�@�ɑ��
	mbstowcs_s(&strLengthNum, buffer, str.size() + 1, str.c_str(), _TRUNCATE);
	//������o�b�t�@��wstring�ɑ��
	std::wstring result = buffer;
	//������o�b�t�@���폜
	delete[] buffer;
	//���ʂ�Ԃ�
	return result;
}

/// <summary>
/// �X�e�[�W���s�����I�����Ă��邩�m�F
/// </summary>
void StageManager::CheckStageMoveEnd()
{

	int stageEndCount = 0;

	//�S�ẴX�e�[�W���s�����I���������m�F
	for (int n = 0; n < m_stage.size(); n++)
	{

		//�I����Ă��Ȃ���Ύ��֍s��
		if (!m_stage[n]->GetMoveEndFlag())
		{
			continue;
		}

		//�I����Ă���̂ŃJ�E���g�𑝂₷
		stageEndCount++;

		//�S�ẴX�e�[�W���s�����I�������ꍇ�X�e�[�W�̍s����ω�������
		if (stageEndCount == m_stage.size())
		{
			//�s������ԍ�
			std::vector<int> stageNum = { 0,1,2,3 };
			// �V���b�t��
			std::random_device seed_gen;
			std::mt19937 engine(seed_gen());
			//�ԍ����V���b�t��������
			std::shuffle(stageNum.begin(), stageNum.end(), engine);

			//�V���b�t�������ԍ����X�e�[�W�ɓn��
			for (int j = 0; j < m_stage.size(); j++)
			{
				//�X�e�[�W�Ƀ��Z�b�g��������
				m_stage[j]->Reset();

				//�ԍ����Z�b�g
				m_stage[j]->SetBehavior(m_behavior[stageNum[j]].get());

			}

		}
	}
}


/// <summary>
/// �X�e�[�W�̃|���S���ƃA�N�^�[�̂߂荞�ݏ���
/// </summary>
/// <param name="actor">�A�N�^�[/param>
/// <param name="polygonVertexPos">�|���S���̒��_���W</param>
/// <param name="normalVec">�@���x�N�g��</param>
void StageManager::ActorPolygonPenetration(Actor* actor, const std::vector<DirectX::SimpleMath::Vector3>& polygonVertexPos, const DirectX::SimpleMath::Vector3& normalVec)
{
	//�A�N�^�[�������߂荞�܂���p�ϐ�
	float actorPenetration = 0.1f;

	//�߂荞�ݏ���
	//�A�N�^�[�̍��W�擾
	DirectX::SimpleMath::Vector3 actorPos = actor->GetPosition();
	//�A�N�^�[�̗���Y���W�v�Z
	actorPos.y = polygonVertexPos[0].y + (-normalVec.x * (actorPos.x - polygonVertexPos[0].x) - normalVec.z * (actorPos.z - polygonVertexPos[0].z)) / normalVec.y;
	//�A�N�^�[���K�N�K�N����̂ŏ����������ɂ߂荞�܂���
	actorPos.y -= actorPenetration;
	//�v�Z�����|�W�V�������A�N�^�[�ɃZ�b�g����
	actor->SetPosition(actorPos);
}


/// <summary>
/// �A�N�^�[�̃X���C�h�x�N�g�����v�Z����
/// </summary>
/// <param name="normalVec">�|���S���̖@���x�N�g��</param>
/// <param name="actorVel">�A�N�^�[�̈ړ���</param>
/// <returns>�X���C�h�x�N�g��</returns>
DirectX::SimpleMath::Vector3 StageManager::SlideVecCalculation(const DirectX::SimpleMath::Vector3& normalVec, const DirectX::SimpleMath::Vector3& actorVel)
{
	DirectX::SimpleMath::Vector3 normalVector = normalVec;
	//�@���x�N�g���̐��K��
	normalVector.Normalize();
	//�v���C���[�̈ړ��x�N�g���Ɩ@���x�N�g���̓��ς̌v�Z
	float dot = actorVel.Dot(normalVector);
	//�X���C�h����ʌv�Z
	DirectX::SimpleMath::Vector3 slideAmount = { actorVel.x - dot,actorVel.y - dot,actorVel.z - dot };
	//�X���C�h�x�N�g�����v�Z
	DirectX::SimpleMath::Vector3 slideVec = normalVector * slideAmount;
	//�X���C�h�x�N�g����Ԃ�
	return slideVec;
}

/// <summary>
/// �X�e�[�W�ƃA�N�^�[�̓����蔻��
/// </summary>
/// <param name="actor">�A�N�^�[</param>
/// <returns>true=�������Ă���@false=�����Ă��Ȃ�</returns>
bool StageManager::StageToActorHitCheck(Actor* actor)
{
	//�S�ăX�e�[�W�Ɠ����蔻������
	for (std::unique_ptr<Stage>& stage : m_stage)
	{
		//�X�e�[�W����C���f�b�N�X�擾
		std::vector<std::vector<int>> index(stage->GetIndices());
		//���݂̒��_���W�擾
		std::vector<DirectX::SimpleMath::Vector3> nowVertices(stage->GetNowVertices());

		//�X�e�[�W�̃|���S���ƃA�N�^�[�����̓����蔻������
		for (int i = 0; i < index.size(); i++)
		{
			//�P�|���S�������W�擾
			std::vector<DirectX::SimpleMath::Vector3> polygonVertexPos = { nowVertices[index[i][0]],nowVertices[index[i][1]] ,nowVertices[index[i][2]] };
			//�A�N�^�[�̃|�W�V�����擾
			const DirectX::SimpleMath::Vector3 actorPos = actor->GetPosition();

			//�A�N�^�[�̓����蔻��̐���
			std::vector<DirectX::SimpleMath::Vector3> actorLinePos = { DirectX::SimpleMath::Vector3(actorPos),DirectX::SimpleMath::Vector3(actorPos.x,actorPos.y + 2.5f,actorPos.z) };
			//�@���x�N�g��
			DirectX::SimpleMath::Vector3 normalVec = DirectX::SimpleMath::Vector3::Zero;

			//�������Ă����ꍇ�X���C�h����
			if (StageHitCheck(polygonVertexPos, actorLinePos, &normalVec))
			{
				//�߂荞�ݏ���
				ActorPolygonPenetration(actor, polygonVertexPos, normalVec);

				//�A�N�^�[�̃x�N�g��
				DirectX::SimpleMath::Vector3 actorVel = actor->GetVelocity();

				//�A�N�^�[�̃X���C�h�����
				float actorSlideGravity = -0.7f;
				actorVel.y = actorSlideGravity;

				//�X���C�h�x�N�g���v�Z
				DirectX::SimpleMath::Vector3 slideVec = SlideVecCalculation(normalVec, actorVel);

				//�A�N�^�[���X���C�h������
				actor->SetPosition((actor->GetPosition() + slideVec));

				//��������
				return true;

			}

		}

	}
	
	//�����Ă��Ȃ�
	return false;
}





/// <summary>
/// �����Ɣ|���S���̓����蔻��
/// </summary>
/// <param name="vertices">���_���W</param>
/// <param name="linePos">�����̗��[���W</param>
/// <param name="normalVector">�@���x�N�g���̃|�C���^</param>
/// <returns>true=�������Ă���@false=�����Ă��Ȃ�</returns>
bool StageManager::StageHitCheck(const std::vector<DirectX::SimpleMath::Vector3>& vertices, const std::vector<DirectX::SimpleMath::Vector3>& linePos, DirectX::SimpleMath::Vector3* normalVector)
{
	//�e���_���W
	DirectX::SimpleMath::Vector3 vertex0 = vertices[0];
	DirectX::SimpleMath::Vector3 vertex1 = vertices[1];
	DirectX::SimpleMath::Vector3 vertex2 = vertices[2];

	//�����̗��[���W
	DirectX::SimpleMath::Vector3 startLine = linePos[0];
	DirectX::SimpleMath::Vector3 endLine = linePos[1];

	//�@���x�N�g���Z�o�̂��߂̃f�[�^
	//���_�O���璸�_1�̃x�N�g��
	DirectX::SimpleMath::Vector3 v1 = vertex1 - vertex0;
	//���_1���璸�_2�̃x�N�g��
	DirectX::SimpleMath::Vector3 v2 = vertex2 - vertex1;

	//�@���x�N�g��
	DirectX::SimpleMath::Vector3 normal;
	//�@���x�N�g���̎Z�o
	normal = v1.Cross(v2);
	DirectX::SimpleMath::Vector3 polygonPosAverage = (vertex0 + vertex1 + vertex2) / 3;

	//�|���S���̒��S�_��������̗��[�ւ̃x�N�g�����v�Z
	DirectX::SimpleMath::Vector3 polygonPosAverageToStartLine = startLine - polygonPosAverage;
	DirectX::SimpleMath::Vector3 polygonPosAverageToEndLine = endLine - polygonPosAverage;

	///�����𕽖ʂ̏Փ˔���========================================================================
	
	//�|���S���̒��S�_��������̗��[�ւ̃x�N�g���Ɩ@���x�N�g���œ��ς��Z�o����
	float dot1 = polygonPosAverageToStartLine.Dot(normal);
	float dot2 = polygonPosAverageToEndLine.Dot(normal);
	//�v�Z�������ς��|���Z���A�O�ȉ��ł���Γ������Ă���
	if (dot1 * dot2 <= 0)
	{

		//�|���S����������̗��[�̋����̌v�Z
		float d1 = std::abs(polygonPosAverageToStartLine.Dot(normal));
		float d2 = std::abs(polygonPosAverageToEndLine.Dot(normal));
		//������̌v�Z
		float a = d1 / (d1 + d2);

		//���_�O��������̗��[�̃x�N�g��
		DirectX::SimpleMath::Vector3 vertex0TostartLine = startLine - vertex0;
		DirectX::SimpleMath::Vector3 vertex0ToendLine = endLine - vertex0;
		//���_�O��������̓����_�v�Z
		DirectX::SimpleMath::Vector3 v3 = ((1 - a) * vertex0TostartLine) + (a * vertex0ToendLine);
		//�����_�̍��W�v�Z
		DirectX::SimpleMath::Vector3 v3Pos = vertex0 + v3;

		//�|���S����ɓ_�����邩���肷��
		
		//V3Pos����e���_�̍��W�ւ̃x�N�g���쐬
		DirectX::SimpleMath::Vector3 vertex0ToV3Pos = v3;
		DirectX::SimpleMath::Vector3 vertex1ToV3Pos = v3Pos - vertex1;
		DirectX::SimpleMath::Vector3 vertex2ToV3Pos = v3Pos - vertex2;

		//�����ɂȂ�悤�Ƀx�N�g�����쐬����
		//���_�O���璸�_�P�ւ̃x�N�g��
		DirectX::SimpleMath::Vector3 vertex0ToVertex1 = vertex1 - vertex0;
		//���_�P���璸�_�Q�ւ̃x�N�g��
		DirectX::SimpleMath::Vector3 vertex1ToVertex2 = vertex2 - vertex1;
		//���_�Q���璸�_�O�ւ̃x�N�g��
		DirectX::SimpleMath::Vector3 vertex2ToVertex0 = vertex0 - vertex2;

		//V3Pos����O���_�̍��W�ւ̃x�N�g���ƒ��_�O���璸�_�P�ւ̃x�N�g���̊O�ώZ�o
		DirectX::SimpleMath::Vector3 cross1 = vertex0ToV3Pos.Cross(vertex0ToVertex1);
		//V3Pos����P���_�̍��W�ւ̃x�N�g���ƒ��_�P���璸�_�Q�ւ̃x�N�g���̊O�ώZ�o
		DirectX::SimpleMath::Vector3 cross2 = vertex1ToV3Pos.Cross(vertex1ToVertex2);
		//V3Pos����Q���_�̍��W�ւ̃x�N�g���ƒ��_�Q���璸�_�O�ւ̃x�N�g���̊O�ώZ�o
		DirectX::SimpleMath::Vector3 cross3 = vertex2ToV3Pos.Cross(vertex2ToVertex0);

		//�Z�o�����O�ϓ��m����ςŌv�Z
		float checkDot1 = cross1.Dot(cross2);
		float checkDot2 = cross2.Dot(cross3);
		float checkDot3 = cross3.Dot(cross1);
		//�Z�o�������ς��|���Z���O�ȏ�ł���΃|���S���̒��ɐ���������
		if (checkDot1 * checkDot2 >= 0 && checkDot2 * checkDot3 >= 0 && checkDot3 * checkDot1 >= 0)
		{
			*normalVector = normal;
			return true;
		}
	}
	return false;
}