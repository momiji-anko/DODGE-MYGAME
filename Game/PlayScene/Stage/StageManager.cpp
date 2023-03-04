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
	
	int i = 0;
	// JSON �f�[�^���p�[�X����
	for (size_t index = 0; index < m_stageData.size(); index++)
	{
		int number;
		float x;
		float y;
		float z;
		int position;
		// �����Fnumber:0,position:x:-40,y:20,z:10,
		std::stringstream ss(m_stageData[index].c_str());
		// ���_�ԍ��̕��������������
		position = m_stageData[index].find("number:");
		// �����񂪌�����Ȃ��ꍇ
		if (position == std::string::npos)
			continue;
		// "vertex_number:"���폜����
		m_stageData[index].replace(position, strlen("number:"), "");
		// ",position:x:"����������
		position = m_stageData[index].find(",position:x:");
		// ",position:x:"���󕶎��ɒu��������
		m_stageData[index].replace(position, strlen(",position:x:"), " ");
		// ",y:"��T������
		position = m_stageData[index].find(",y:");
		// ",y:"���󕶎��ɒu��������
		m_stageData[index].replace(position, strlen(",y:"), " ");
		// ",z:"��T������
		position = m_stageData[index].find(",z:");
		// ",z:"���󕶎��ɒu��������
		m_stageData[index].replace(position, strlen(",z:"), " ");
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

}

void StageManeger::SetShadow(ShadowMap* shadow)
{
	for (std::unique_ptr<Stage>& stage : m_stage)
	{
		stage->SetShadow(shadow);
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
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	std::wstring StageFileName;
	std::wstring ModelFileName;

	m_commonState = commonState;

	m_stageSelect = stage;

	//HACK::TEST
	//m_stageSelect = StageSelect::Stage3;
	
	switch (m_stageSelect)
	{
	case StageSelect::Stage1:
		StageFileName = L"Resources/StageData/Stage1.json";
		ModelFileName = L"Resources/Models/StageObject.cmo";
		break;
	case StageSelect::Stage2:
		StageFileName = L"Resources/StageData/Stage2.json";
		ModelFileName = L"Resources/Models/Stage2.cmo";

		//�����R�@�O�P�O�D�T\0
		m_baseVertices = {
			DirectX::SimpleMath::Vector3( -3.0f , 2.0f ,   3.0f),//0
			DirectX::SimpleMath::Vector3( -3.0f , 2.0f ,  10.0f),//1
			DirectX::SimpleMath::Vector3(  3.0f , 2.0f ,    3.0f),//2
			DirectX::SimpleMath::Vector3(  3.0f , 2.0f ,   10.0f),//3
										  
			DirectX::SimpleMath::Vector3( -3.0f , 2.0f , -10.0f),//4
			DirectX::SimpleMath::Vector3( -3.0f , 2.0f ,  -3.0f),//5
			DirectX::SimpleMath::Vector3(  3.0f , 2.0f , -10.0f),//6
			DirectX::SimpleMath::Vector3(  3.0f , 2.0f ,  -3.0f),//7
										  
			DirectX::SimpleMath::Vector3( -10.5f , 2.0f , -10.0f),//8
			DirectX::SimpleMath::Vector3( -10.5f , 2.0f ,  10.0f),//9
			DirectX::SimpleMath::Vector3(  10.5f , 2.0f , -10.0f),//10
			DirectX::SimpleMath::Vector3(  10.5f , 2.0f ,  10.0f),//11

		};
		m_nowVertices.resize(m_baseVertices.size());

		m_indices = {
			{0, 1, 2},
			{2, 1, 3},

			{4, 5, 6},
			{6, 5, 7},

			{ 8, 9, 4},
			{ 4, 9, 1},

			{ 6, 3, 10},
			{10, 3, 11},
		};

		break;
	case StageSelect::Stage3:
		StageFileName = L"Resources/StageData/Stage2.json";
		ModelFileName = L"Resources/Models/Stage3.cmo";

		//�����R�@�O�P�O�D�T
		//��{���_���W
		m_baseVertices = {
			DirectX::SimpleMath::Vector3(-10.0f ,  0.0f ,  -10.0f),//0
			DirectX::SimpleMath::Vector3(-10.0f ,  0.0f ,   10.0f),//1
			DirectX::SimpleMath::Vector3( 10.0f ,  0.0f ,  -10.0f),//2
			DirectX::SimpleMath::Vector3( 10.0f ,  0.0f ,   10.0f),//3
			DirectX::SimpleMath::Vector3(-10.0f , -20.0f , -10.0f),//4
			DirectX::SimpleMath::Vector3(-10.0f , -20.0f ,  10.0f),//5
			DirectX::SimpleMath::Vector3( 10.0f , -20.0f , -10.0f),//6
			DirectX::SimpleMath::Vector3( 10.0f , -20.0f ,  10.0f),//7
		};
		//���݂�
		m_nowVertices.resize(m_baseVertices.size());

		m_indices = {
			{0, 1, 2},
			{2, 1, 3},

			{4, 6, 5},
			{5, 6, 7},
			
			{2, 3, 6},
			{6, 3, 7},
			
			{4, 5, 0},
			{0, 5, 1},

			{1, 5, 3},
			{3, 5, 7},
			
			{4, 0, 6},
			{6, 0, 2},
			
		};
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
	m_geo= DirectX::GeometricPrimitive::CreateBox(context, DirectX::SimpleMath::Vector3(0.3f, 0.3f, 0.3f));

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
			std::vector<int> stageNum = {0,1,2,3};
			// �V���b�t��
			std::random_device seed_gen;
			std::mt19937 engine(seed_gen());
			std::shuffle(stageNum.begin(), stageNum.end(), engine);

			for (int j = 0; j < m_stage.size(); j++)
			{
				m_stage[j]->SetStageType(static_cast<Stage::StageType>(stageNum[j]));
				m_stage[j]->Reset();
			}
			break;
		}
		
		
	}

	for (int i = 0; i < m_stage.size(); i++)
	{
	
		m_stage[i]->Update(timer);

		for (int j = 0; j < m_baseVertices.size(); j++)
		{
			//���݂̓����蔻��p�̒��_���W�s��
			DirectX::SimpleMath::Matrix nowVertexMatrix = DirectX::SimpleMath::Matrix::Identity;
			//Stage��X��Z�̊p�x�̌v�Z
			DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::CreateRotationX(m_stage[i]->GetRotation().x / 180.0f * 3.14f) * DirectX::SimpleMath::Matrix::CreateRotationZ(m_stage[i]->GetRotation().z / 180.0f * 3.14f);
			//���݂̒��_���W�̌v�Z
			nowVertexMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_baseVertices[j] ) * rot * DirectX::SimpleMath::Matrix::CreateTranslation(m_stage[i]->GetPosition());
			//�v�Z�����s������݂̒��_���W�v�Z�ɑ������
			m_nowVertices[j] = DirectX::SimpleMath::Vector3(nowVertexMatrix._41, nowVertexMatrix._42, nowVertexMatrix._43);
		}

	}
}

void StageManeger::Draw(Camera* camera)
{
	for (std::unique_ptr<Stage>& stage : m_stage)
	{
		stage->Draw(camera);
	}

	for (int i = 0; i < m_indices.size(); i++)
	{
		m_geo->Draw(DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(m_nowVertices[m_indices[i][0]]) ), camera->GetViewMatrix(), camera->GetProjectionMatrix());
		m_geo->Draw(DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(m_nowVertices[m_indices[i][1]])	), camera->GetViewMatrix(), camera->GetProjectionMatrix());
		m_geo->Draw(DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(m_nowVertices[m_indices[i][2]])	), camera->GetViewMatrix(), camera->GetProjectionMatrix());
	}

}

void StageManeger::Finalize()
{


}

bool StageManeger::PlayerStageAABBHitCheck(Actor* player)
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

				 if (playerVel.Length() == 0)
				 {
					 playerVel.y = -1.4f;
				 }
				
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




bool StageManeger::ItemHitCheck(Actor* item)
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
	case StageSelect::Stage3:
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
bool StageManeger::StageHitCheck(std::vector<DirectX::SimpleMath::Vector3> vertices, std::vector<DirectX::SimpleMath::Vector3> linePos, DirectX::SimpleMath::Vector3* normalVector)
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
	//http://marupeke296.com/COL_3D_No3_LineToPlane.html
	//�|���S���̒��S�_��������̗��[�ւ̃x�N�g���Ɩ@���x�N�g���œ��ς��Z�o����
	float dot1 = polygonPosAverageToStartLine.Dot(normal);
	float dot2 = polygonPosAverageToEndLine.Dot(normal);
	//�v�Z�������ς��|���Z���A�O�ȉ��ł���Γ������Ă���
	if (dot1 * dot2 <= 0)
	{

		//http://marupeke296.com/COL_Basic_No2_ShortTec.html#%E3%81%82%E3%82%8B%E7%82%B9%E3%81%8B%E3%82%89%E5%B9%B3%E9%9D%A2%E3%81%BE%E3%81%A7%E3%81%AE%E8%B7%9D%E9%9B%A2
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
		//http://marupeke296.com/COL_Basic_No2_ShortTec.html#%E3%83%9D%E3%83%AA%E3%82%B4%E3%83%B3%E4%B8%8A%E3%81%AB%E7%82%B9%E3%81%8C%E5%90%AB%E3%81%BE%E3%82%8C%E3%82%8B%E3%81%8B%E3%82%92%E5%88%A4%E5%AE%9A

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