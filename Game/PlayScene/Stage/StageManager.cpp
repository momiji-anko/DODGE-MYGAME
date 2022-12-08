#include"pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include"StageManager.h"
#include"DeviceResources.h"
#include"../MyRandom.h"

// JSON 形式でグラフデータを読み込む
bool StageManeger::LoadGraphDataByJSON(const std::wstring& fileName)
{
	std::stringstream ss;
	std::string graphData;
	// 入力ファイルストリーム
	std::ifstream in(fileName, std::ifstream::in);
	// 出力ファイルストリームをオープンする
	in.open(fileName, std::ifstream::in);
	if (!in.is_open())
		return false;
	// ストリングストリーム
	ss << in.rdbuf();
	// 配列をクリアする
	m_stageData.clear();
	// 改行までの一行分の文字列を取得する
	while (std::getline(ss, graphData)) {
		// グラフデータから不要な文字を消去する
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
			// 配列にワードを追加する
			m_stageData.push_back(graphData);
		}
	}
	// 入力ファイルストリームをクローズする
	in.close();
	// 正常終了
	return true;
}
// JSON 形式のデータをパースしオブジェクトに変換する
void StageManeger::ParseJSON()
{
	int m_position;
	int i = 0;
	// JSON データをパースする
	for (size_t index = 0; index < m_stageData.size(); index++)
	{
		int number;
		float x;
		float y;
		float z;
		int m_position;
		// 書式：number:0,position:x:-40,y:20,z:10,
		std::stringstream ss(m_stageData[index].c_str());
		// 頂点番号の文字列を検索する
		m_position = m_stageData[index].find("number:");
		// 文字列が見つからない場合
		if (m_position == std::string::npos)
			continue;
		// "vertex_number:"を削除する
		m_stageData[index].replace(m_position, strlen("number:"), "");
		// ",position:x:"を検索する
		m_position = m_stageData[index].find(",position:x:");
		// ",position:x:"を空文字に置き換える
		m_stageData[index].replace(m_position, strlen(",position:x:"), " ");
		// ",y:"を探索する
		m_position = m_stageData[index].find(",y:");
		// ",y:"を空文字に置き換える
		m_stageData[index].replace(m_position, strlen(",y:"), " ");
		// ",z:"を探索する
		m_position = m_stageData[index].find(",z:");
		// ",z:"を空文字に置き換える
		m_stageData[index].replace(m_position, strlen(",z:"), " ");
		ss.clear();
		ss.str(m_stageData[index]);
		// 座標を取得する
		ss >> number >> x >> y >> z;
		// ステージを生成する
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
	// 頂点インデックス
	int vertexIndex = 0;
	// 検出フラグ
	bool found = false;
	//for (size_t index = 0; index < m_stageData.size(); index++)
	//{
	//	if (!found)
	//	{
	//		// 隣接リストの文字列を検索する
	//		m_position = m_stageData[index].find("adjacency_list:");
	//		// 文字列が見つからない場合
	//		if (m_position == std::string::npos)
	//			continue;


	//		// 隣接リストの文字列が見つかった
	//		found = true;
	//		// インデックスをインクリメントする
	//		if (index < m_stageData.size() - 1)
	//			index++;
	//		else
	//			continue;
	//	}
	//	// 隣接リストの行末のコンマが存在する場合は削除する
	//	if (m_stageData[index].at((m_stageData[index].size() - 1)) == ',')
	//		m_stageData[index].erase(--m_stageData[index].end());

	//	// ストリングストリームを生成する
	//	std::stringstream ss(m_stageData[index].c_str());
	//	// 頂点番号
	//	std::string vertexNumber;
	//	// コンマまでの文字列の取得を繰り返す
	//	while (std::getline(ss, vertexNumber, ','))
	//	{
	//		// エッジの頂点番号を数値化する
	//		int vertexIndexOfEdge = std::stoi(vertexNumber);
	//		// 隣接リストにエッジの頂点を追加する
	//	}
	//	// 頂点インデックスをインクリメントする
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
	//	エフェクトファクトリの作成
	DirectX::EffectFactory* factory2 = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//	テクスチャの読み込みパス指定
	factory2->SetDirectory(L"Resources/Models");
	//	ファイルを指定してモデルデータ読み込み
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
