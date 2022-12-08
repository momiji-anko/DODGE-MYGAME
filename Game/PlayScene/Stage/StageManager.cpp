#include"pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include"StageManager.h"
#include"DeviceResources.h"
#include"../MyRandom.h"

// JSON �`���ŃO���t�f�[�^��ǂݍ���
bool StageManeger::LoadGraphDataByJSON(const std::wstring& fileName)
{
	std::stringstream ss;
	std::string graphData;
	// ���̓t�@�C���X�g���[��
	std::ifstream in(fileName, std::ifstream::in);
	// �o�̓t�@�C���X�g���[�����I�[�v������
	in.open(fileName, std::ifstream::in);
	if (!in.is_open())
		return false;
	// �X�g�����O�X�g���[��
	ss << in.rdbuf();
	// �z����N���A����
	m_stageData.clear();
	// ���s�܂ł̈�s���̕�������擾����
	while (std::getline(ss, graphData)) {
		// �O���t�f�[�^����s�v�ȕ�������������
		graphData.erase(std::remove_if(
			graphData.begin(),
			graphData.end(),
			[](char c) {
				return (
					c == '\r' || c == '\t' || c == ' ' || c == '\n' || c == '\"' ||
					c == '[' || c == ']' || c == '{' || c == '}'
					);
			}),
			graphData.end()
				);
		if (!graphData.empty())
		{
			// �z��Ƀ��[�h��ǉ�����
			m_stageData.push_back(graphData);
		}
	}
	// ���̓t�@�C���X�g���[�����N���[�Y����
	in.close();
	// ����I��
	return true;
}
// JSON �`���̃f�[�^���p�[�X���I�u�W�F�N�g�ɕϊ�����
void StageManeger::ParseJSON()
{
	int m_position;
	int i = 0;
	// JSON �f�[�^���p�[�X����
	for (size_t index = 0; index < m_stageData.size(); index++)
	{
		int number;
		float x;
		float y;
		float z;
		int m_position;
		// �����Fnumber:0,position:x:-40,y:20,z:10,
		std::stringstream ss(m_stageData[index].c_str());
		// ���_�ԍ��̕��������������
		m_position = m_stageData[index].find("number:");
		// �����񂪌�����Ȃ��ꍇ
		if (m_position == std::string::npos)
			continue;
		// "vertex_number:"���폜����
		m_stageData[index].replace(m_position, strlen("number:"), "");
		// ",position:x:"����������
		m_position = m_stageData[index].find(",position:x:");
		// ",position:x:"���󕶎��ɒu��������
		m_stageData[index].replace(m_position, strlen(",position:x:"), " ");
		// ",y:"��T������
		m_position = m_stageData[index].find(",y:");
		// ",y:"���󕶎��ɒu��������
		m_stageData[index].replace(m_position, strlen(",y:"), " ");
		// ",z:"��T������
		m_position = m_stageData[index].find(",z:");
		// ",z:"���󕶎��ɒu��������
		m_stageData[index].replace(m_position, strlen(",z:"), " ");
		ss.clear();
		ss.str(m_stageData[index]);
		// ���W���擾����
		ss >> number >> x >> y >> z;
		// �X�e�[�W�𐶐�����
		m_stage.push_back(std::make_unique<Stage>());

		m_stage[i]->Initialize(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3(x, y, z), true, 0.0f, nullptr, m_stageModel.get(), m_commonState);
		int n = 0;
		switch (m_stageSelect)
		{
		case StageSelect::Stage1:

			break;
		case StageSelect::Stage2:
			n = 4;
			break;
		case StageSelect::Stage3:
			n = 5;
			break;
		default:
			break;
		}


		m_stage[i]->SetStageType(static_cast <Stage::StageType>(i + n));
		i++;
	}
	// ���_�C���f�b�N�X
	int vertexIndex = 0;
	// ���o�t���O
	bool found = false;
	//for (size_t index = 0; index < m_stageData.size(); index++)
	//{
	//	if (!found)
	//	{
	//		// �אڃ��X�g�̕��������������
	//		m_position = m_stageData[index].find("adjacency_list:");
	//		// �����񂪌�����Ȃ��ꍇ
	//		if (m_position == std::string::npos)
	//			continue;


	//		// �אڃ��X�g�̕����񂪌�������
	//		found = true;
	//		// �C���f�b�N�X���C���N�������g����
	//		if (index < m_stageData.size() - 1)
	//			index++;
	//		else
	//			continue;
	//	}
	//	// �אڃ��X�g�̍s���̃R���}�����݂���ꍇ�͍폜����
	//	if (m_stageData[index].at((m_stageData[index].size() - 1)) == ',')
	//		m_stageData[index].erase(--m_stageData[index].end());

	//	// �X�g�����O�X�g���[���𐶐�����
	//	std::stringstream ss(m_stageData[index].c_str());
	//	// ���_�ԍ�
	//	std::string vertexNumber;
	//	// �R���}�܂ł̕�����̎擾���J��Ԃ�
	//	while (std::getline(ss, vertexNumber, ','))
	//	{
	//		// �G�b�W�̒��_�ԍ��𐔒l������
	//		int vertexIndexOfEdge = std::stoi(vertexNumber);
	//		// �אڃ��X�g�ɃG�b�W�̒��_��ǉ�����
	//	}
	//	// ���_�C���f�b�N�X���C���N�������g����
	//	vertexIndex++;
	//}
}

void StageManeger::SetShadow(ShadowMap* shadow)
{
	for (std::unique_ptr<Stage>& stage : m_stage)
	{
		stage->Setshader(shadow);
	}
}

StageManeger::StageManeger()
	:
	m_commonState(nullptr),
	m_stageSelect(StageSelect::Stage1),
	m_time_s(0.0f)
{

}

StageManeger::~StageManeger()
{


}

void StageManeger::Initialize(DirectX::CommonStates* commonState, StageSelect stage)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	std::wstring StageFileName;
	std::wstring ModelFileName;

	m_commonState = commonState;

	m_stageSelect = stage;

	switch (stage)
	{
	case StageSelect::Stage1:
		StageFileName = L"Resources/StageData/Stage1.json";
		ModelFileName = L"Resources/Models/StageObject.cmo";
		break;
	case StageSelect::Stage2:
		StageFileName = L"Resources/StageData/Stage2.json";
		ModelFileName = L"Resources/Models/stage.cmo";
		break;
	case StageSelect::Stage3:
		StageFileName = L"";
		ModelFileName = L"";
		break;
	default:
		break;
	}
	//	�G�t�F�N�g�t�@�N�g���̍쐬
	DirectX::EffectFactory* factory2 = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//	�e�N�X�`���̓ǂݍ��݃p�X�w��
	factory2->SetDirectory(L"Resources/Models");
	//	�t�@�C�����w�肵�ă��f���f�[�^�ǂݍ���
	m_stageModel = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		ModelFileName.c_str(),
		*factory2
	);
	delete factory2;

	LoadGraphDataByJSON(StageFileName);
	ParseJSON();

	m_flag.resize(m_stage.size());


}

void StageManeger::Update(const DX::StepTimer& timer)
{

	float time = timer.GetElapsedSeconds();

	m_time_s += time;


	int count = 0;
	for (int n = 0; n < m_stage.size() ; n++)
	{
		if (!m_stage[n]->GetEndFlag())
		{
			continue;
		}
		count++;
		if (count >= 4)
		{
			int stageState[4];
			stageState[0] = MyRandom::GetIntRange(0, 3);

			do
			{
				stageState[1] = MyRandom::GetIntRange(0, 3);

			} while (stageState[1] == stageState[0]);

			do
			{
				stageState[2] = MyRandom::GetIntRange(0, 3);

			} while (stageState[2] == stageState[0] || stageState[2] == stageState[1]);

			do
			{
				stageState[3] = MyRandom::GetIntRange(0, 3);

			} while (stageState[3] == stageState[0] || (stageState[3] == stageState[1] || stageState[3] == stageState[2]));
			
			

			for (int j = 0; j < m_stage.size(); j++)
			{
				m_stage[j]->SetStageType(static_cast<Stage::StageType>(stageState[j]));
				m_stage[j]->Reset();
			}
			break;
		}
		
		
	}

	for (int i = 0; i < m_stage.size(); i++)
	{
	
		m_stage[i]->Update(timer);

	}
}

void StageManeger::Draw(Camera* camera)
{
	for (std::unique_ptr<Stage>& stage : m_stage)
	{
		stage->Draw(camera);
	}

}

void StageManeger::Finalize()
{


}

bool StageManeger::PlayerStageAABBHitCheck(Actor* player)
{
	for (std::unique_ptr<Stage>& stage : m_stage)
	{
		if (stage->GetAABB()->HitCheck(player->GetAABB()))
		{
			DirectX::SimpleMath::Vector3 pos = player->GetPosition();
			player->SetPosition(DirectX::SimpleMath::Vector3(pos.x, stage->GetPosition().y + 0.48f, pos.z));

			return true;

		}
	}
	return false;
}

//bool StageManeger::StagePlayerHitCheck(Player* player)
//{
//	for (std::unique_ptr<Stage>& stage : m_stage)
//	{
//		if (stage->GetAABB()->HitCheck(player->GetAABB()))
//		{
//			DirectX::SimpleMath::Vector3 pos = player->GetPosition();
//			player->SetPosition(DirectX::SimpleMath::Vector3(pos.x, stage->GetPosition().y + 0.48f, pos.z));
//
//			return true;
//
//		}
//	}
//	
//	return false;
//}

bool StageManeger::ItemHitCheck(AABBFor3D* itemAABB)
{
	for (std::unique_ptr<Stage>& stage : m_stage)
	{
		if (stage->GetAABB()->HitCheck(itemAABB))
		{
			return true;

		}
	}
	return false;
}
