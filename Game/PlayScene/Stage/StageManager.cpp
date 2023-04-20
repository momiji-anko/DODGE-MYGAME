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

//�r�w�C�r�A�[�̃C���N���[�h
#include"StageBehaviors/FirstFloorToFallBehavior.h"
#include"StageBehaviors/SecondFloorToFallBehavior.h"
#include"StageBehaviors/ThirdFloorToFallBehavior.h"
#include"StageBehaviors/TiltingFloorbehavior.h"
#include"StageBehaviors/RotationCubeBehavior.h"

void StageManager::SetShadow(ShadowMap* shadow)
{
	for (std::unique_ptr<Stage>& stage : m_stage)
	{
		stage->SetShadow(shadow);
	}
}

StageManager::StageManager()
	:
	m_commonState(nullptr),
	m_stageSelect(StageSelect::Stage1),
	m_stageModel()
{
	

}

StageManager::~StageManager()
{
}

void StageManager::Initialize(DirectX::CommonStates* commonState, StageSelect stage)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	std::wstring StageFileName;

	
	m_commonState = commonState;

	m_stageSelect = stage;

	
	
	switch (m_stageSelect)
	{
	case StageSelect::Stage1:
		StageFileName = L"Resources/StageData/Stage1.json";
		break;
	case StageSelect::Stage2:
		StageFileName = L"Resources/StageData/Stage2.json";
		break;
	case StageSelect::Stage3:
		StageFileName = L"Resources/StageData/Stage3.json";
		break;
	default:
		break;
	}
	
	
	//�r�w�C�r�A�[�쐬
	CreateBehavior();
	//json��ǂݍ��݃X�e�[�W���쐬
	LoadStageJson(StageFileName);

}

void StageManager::Update(const DX::StepTimer& timer)
{
	
	CheckStageMoveEnd();

	//�X�e�[�W�̍X�V
	for (int i = 0; i < m_stage.size(); i++)
	{
		m_stage[i]->Update(timer);
	}

	//���_���W�̍X�V
	UpdateVertices();
}

void StageManager::Draw(Camera* camera)
{
	for (std::unique_ptr<Stage>& stage : m_stage)
	{
		stage->Draw(camera);
	}



}

void StageManager::Finalize()
{


}

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

	m_behavior[3] = nullptr;
	//�X����
	m_behavior[4] = std::make_unique<TiltingFloorbehavior>();
	//��]����L���[�u
	m_behavior[5] = std::make_unique<RotationCubeBehavior>();




}

void StageManager::UpdateVertices()
{
	//�X�e�[�W�̐���for���ŉ�
	for (int i = 0; i < m_stage.size(); i++)
	{
		//vertex�̐�����
		for (int j = 0; j < m_baseVertices.size(); j++)
		{
			//���݂̓����蔻��p�̒��_���W�s��
			DirectX::SimpleMath::Matrix nowVertexMatrix = DirectX::SimpleMath::Matrix::Identity;
			//Stage��X��Z�̊p�x�̌v�Z
			DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_stage[i]->GetRotation());
			//���݂̒��_���W�̌v�Z
			nowVertexMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_baseVertices[j]) * rot * DirectX::SimpleMath::Matrix::CreateTranslation(m_stage[i]->GetPosition());
			//�v�Z�����s������݂̒��_���W�v�Z�ɑ������
			m_nowVertices[j] = DirectX::SimpleMath::Vector3(nowVertexMatrix._41, nowVertexMatrix._42, nowVertexMatrix._43);
		}
	}
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

	//�X�e�[�W�̐��擾
	int stageNum = stageJson["StageNum"];
	//�X�e�[�W�̐����m��
	m_stagePositions.resize(stageNum);
	//�X�e�[�W�̐����m��
	m_stageType.resize(stageNum);

	//�X�e�[�W���̏���ǂݍ���
	for (int i = 0; i < stageNum; i++)
	{
		//�ԍ����X�g�����O�ɂ���
		std::string str = std::to_string(i);
		//�X�e�[�W�^�C�v�擾
		m_stageType[i] = stageJson["Stage"][str.c_str()]["StageType"];
		//�X�e�[�W�̃|�W�V�������擾
		m_stagePositions[i].x = stageJson["Stage"][str.c_str()]["X"];
		m_stagePositions[i].y = stageJson["Stage"][str.c_str()]["Y"];
		m_stagePositions[i].z = stageJson["Stage"][str.c_str()]["Z"];
	}

	//���_�̐��擾
	int vertexNum = stageJson["VertexNum"];
	//���_�̐����m��
	m_baseVertices.resize(vertexNum);
	m_nowVertices.resize(vertexNum);
	
	//���_���W�̎擾
	for (int i = 0; i < vertexNum; i++)
	{
		//�ԍ����X�g�����O�ɂ���
		std::string str = std::to_string(i);
		//���_���W�̎擾
		m_baseVertices[i].x = stageJson["VertexPosition"][str.c_str()]["X"];
		m_baseVertices[i].y = stageJson["VertexPosition"][str.c_str()]["Y"];
		m_baseVertices[i].z = stageJson["VertexPosition"][str.c_str()]["Z"];
	}

	//�x�[�X���_���W�����݂̒��_���W�ɑ��
	m_nowVertices = m_baseVertices;

	//���_���W�C���f�b�N�X�̐��擾
	int vertexIndex = stageJson["VertexIndexNum"];

	//���_�C���f�b�N�X�̐����m��
	m_indices.resize(vertexIndex);

	//���_�C���f�b�N�X�̓ǂݍ���
	for (int i = 0; i < vertexIndex; i++)
	{
		//�C���f�b�N�X�̔z��m��
		m_indices[i].resize(3);
		//�ԍ����X�g�����O�ɂ���
		std::string str = std::to_string(i);
		//�C���f�b�N�X�ԍ��̎擾
		m_indices[i][0] = stageJson["VertexIndex"][str.c_str()]["1"];
		m_indices[i][1] = stageJson["VertexIndex"][str.c_str()]["2"];
		m_indices[i][2] = stageJson["VertexIndex"][str.c_str()]["3"];
	}

	//���f���̃t�@�C���̃p�X���擾�istring�j
	std::string modelFileName = stageJson["StageModelFileName"];
	//�擾�����t�@�C���p�X��string����wstring�ɕϊ�����
	std::wstring modelFile = ConvertWString(modelFileName);
	//���f���쐬
	m_stageModel = ModelManager::GetInstance().LoadModel(modelFile.c_str());

	//�X�e�[�W�̐����z����m��
	m_stage.resize(stageNum);
	
	DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3::One;

	//�X�e�[�W�̍쐬
	for (int i = 0; i < stageNum; i++)
	{
		//�X�e�[�W�̃��j�[�N�|�C���^�ō쐬
		m_stage[i] = std::make_unique<Stage>();
		//�ǂݍ��񂾃X�e�[�W���ŃX�e�[�W������������
		m_stage[i]->Initialize(DirectX::SimpleMath::Vector3::Zero, m_stagePositions[i], scale, DirectX::SimpleMath::Vector3::Zero, true, m_behavior[m_stageType[i]].get(), m_stageModel, m_commonState);
		//�X�e�[�W�̃^�C�v��ݒ�
		m_stage[i]->SetStageType(static_cast<Stage::StageType>(m_stageType[i]));

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

void StageManager::CheckStageMoveEnd()
{
	//�S�ẴX�e�[�W���s�����I���������m�F
	for (int n = 0; n < m_stage.size(); n++)
	{
		//�m�F�p�J�E���g
		int count = 0;
		//�I����Ă��Ȃ���Ύ��֍s��
		if (!m_stage[n]->GetEndFlag())
		{
			continue;
		}
		//�I����Ă���̃J�E���g�𑝂₷
		count++;
		//�S�ẴX�e�[�W���s�����I�������ꍇ�X�e�[�W�̍s����ω�������
		if (count >= m_stage.size())
		{
			//�s������ԍ�
			std::vector<int> stageNum = { 0,1,2,3 };
			// �V���b�t��
			std::random_device seed_gen;
			std::mt19937 engine(seed_gen());
			//�ԍ����V���b�t��������
			std::shuffle(stageNum.begin(), stageNum.end(), engine);
			//�V���b�t���������ԍ����X�e�[�W�ɓn��
			for (int j = 0; j < m_stage.size(); j++)
			{
				//�ԍ����Z�b�g
				m_stage[j]->SetStageType(static_cast<Stage::StageType>(stageNum[j]));
				//�X�e�[�W�Ƀ��Z�b�g��������
				m_stage[j]->Reset();
			}
			//for���𔲂���
			break;
		}
	}
}



bool StageManager::PlayerStageAABBHitCheck(Actor* player)
{
	switch (m_stageSelect)
	{
	case StageSelect::Stage1:
		for (std::unique_ptr<Stage>& stage : m_stage)
		{
			if (stage->GetAABB()->HitCheck(player->GetAABB()))
			{
				DirectX::SimpleMath::Vector3 pos = player->GetPosition();
				player->SetPosition(DirectX::SimpleMath::Vector3(pos.x, stage->GetPosition().y + 0.48f, pos.z));

				return true;

			}
		}
		break;
	case StageSelect::Stage2:
	case StageSelect::Stage3:

		for (std::unique_ptr<Stage>& stage : m_stage)
		{
			for (int i = 0; i < m_indices.size(); i++)
			{
				std::vector<DirectX::SimpleMath::Vector3> v = { m_nowVertices[m_indices[i][0] ],m_nowVertices[m_indices[i][1] ] ,m_nowVertices[m_indices[i][2] ] };
				const DirectX::SimpleMath::Vector3& playerPos = player->GetPosition();
				 DirectX::SimpleMath::Vector3& playerVel = player->GetVelocity();

				 playerVel.y = -0.7f;
				
				std::vector<DirectX::SimpleMath::Vector3> playerLinePos = { DirectX::SimpleMath::Vector3(playerPos),DirectX::SimpleMath::Vector3(playerPos.x,playerPos.y + 2.5f,playerPos.z) };

				DirectX::SimpleMath::Vector3 normalVec = DirectX::SimpleMath::Vector3::Zero;
				if (StageHitCheck(v, playerLinePos, &normalVec))
				{
					//�߂荞�ݏ���
					//�v���C���[�̍��W�擾
					DirectX::SimpleMath::Vector3 newPlayerPos = player->GetPosition();
					//�v���C���[�̗���Y���W�v�Z
					newPlayerPos.y = v[0].y + (-normalVec.x * (newPlayerPos.x - v[0].x) - normalVec.z * (newPlayerPos.z - v[0].z)) / normalVec.y;
					//�v���C���[���K�N�K�N����̂ŏ����������ɂ��炷
					newPlayerPos.y -= 0.1;
					//�v�Z�����|�W�V�������v���C���[�ɃZ�b�g����
					player->SetPosition(newPlayerPos);

					//�X���C�h�x�N�g���v�Z
					//�@���x�N�g���̐��K��
					normalVec.Normalize();
					//�v���C���[�̈ړ��x�N�g���Ɩ@���x�N�g���̓��ς̌v�Z
					float aa = playerVel.Dot(normalVec);
					//�v���C���[�̈ړ��x�N�g�����v�Z�������ςň���
					DirectX::SimpleMath::Vector3  f = { playerVel.x - aa,playerVel.y - aa,playerVel.z - aa };
					//�@���x�N�g����	���|���Z���X���C�h�x�N�g������������
					DirectX::SimpleMath::Vector3 slideVec = normalVec * f  ;
					//���݂̃v���C���[���W�ɃX���C�h�x�N�g���𑫂��v���C���[�ɃZ�b�g����
					player->SetPosition((player->GetPosition() + slideVec));
					return true;
				}


			}
		}
		break;
	default:
		break;
	}
	
	return false;
}




bool StageManager::ItemHitCheck(Actor* item)
{
	
	switch (m_stageSelect)
	{
	case StageSelect::Stage1:
		for (std::unique_ptr<Stage>& stage : m_stage)
		{
			if (stage->GetAABB()->HitCheck(item->GetAABB()))
			{
				return true;

			}
		}
		break;
	case StageSelect::Stage2:
	case StageSelect::Stage3:
		for (std::unique_ptr<Stage>& stage : m_stage)
		{
			for (int i = 0; i < m_indices.size(); i++)
			{
				std::vector<DirectX::SimpleMath::Vector3> v = { m_nowVertices[m_indices[i][0]],m_nowVertices[m_indices[i][1]] ,m_nowVertices[m_indices[i][2]] };
				const DirectX::SimpleMath::Vector3& itemPos = item->GetPosition();
				const DirectX::SimpleMath::Vector3& itemVel = item->GetVelocity();
				std::vector<DirectX::SimpleMath::Vector3> itemLinePos = { DirectX::SimpleMath::Vector3(itemPos),DirectX::SimpleMath::Vector3(itemPos.x,itemPos.y + 0.5f,itemPos.z) };
				DirectX::SimpleMath::Vector3 normalVec = DirectX::SimpleMath::Vector3::Zero;
				if (StageHitCheck(v, itemLinePos, &normalVec))
				{
					normalVec.Normalize();
					float aa = itemVel.Dot(normalVec);
					DirectX::SimpleMath::Vector3  f = { itemVel.x - aa,itemVel.y - aa,itemVel.z - aa };
					DirectX::SimpleMath::Vector3 slideVec = f * normalVec;
					item->SetPosition(itemPos + slideVec);
					return true;
				}
			}
		}
		break;

	default:
		break;
	}
	return false;
}

/// <summary>
/// �����Ɣ|���S���̓����蔻��
/// </summary>
/// <param name="vertices">���_���W</param>
/// <param name="linePos">�����̗��[���W</param>
/// <param name="normalVector">�@���x�N�g���̃|�C���^</param>
/// <returns>true=�������Ă���@false=�����Ă��Ȃ�</returns>
bool StageManager::StageHitCheck(std::vector<DirectX::SimpleMath::Vector3> vertices, std::vector<DirectX::SimpleMath::Vector3> linePos, DirectX::SimpleMath::Vector3* normalVector)
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