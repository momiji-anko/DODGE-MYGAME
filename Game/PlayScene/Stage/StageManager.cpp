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
	
	int i = 0;
	// JSON データをパースする
	for (size_t index = 0; index < m_stageData.size(); index++)
	{
		int number;
		float x;
		float y;
		float z;
		int position;
		// 書式：number:0,position:x:-40,y:20,z:10,
		std::stringstream ss(m_stageData[index].c_str());
		// 頂点番号の文字列を検索する
		position = m_stageData[index].find("number:");
		// 文字列が見つからない場合
		if (position == std::string::npos)
			continue;
		// "vertex_number:"を削除する
		m_stageData[index].replace(position, strlen("number:"), "");
		// ",position:x:"を検索する
		position = m_stageData[index].find(",position:x:");
		// ",position:x:"を空文字に置き換える
		m_stageData[index].replace(position, strlen(",position:x:"), " ");
		// ",y:"を探索する
		position = m_stageData[index].find(",y:");
		// ",y:"を空文字に置き換える
		m_stageData[index].replace(position, strlen(",y:"), " ");
		// ",z:"を探索する
		position = m_stageData[index].find(",z:");
		// ",z:"を空文字に置き換える
		m_stageData[index].replace(position, strlen(",z:"), " ");
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
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	std::wstring StageFileName;
	std::wstring ModelFileName;

	m_commonState = commonState;

	m_stageSelect = stage;

	switch (m_stageSelect)
	{
	case StageSelect::Stage1:
		StageFileName = L"Resources/StageData/Stage1.json";
		ModelFileName = L"Resources/Models/StageObject.cmo";
		break;
	case StageSelect::Stage2:
		StageFileName = L"Resources/StageData/Stage2.json";
		ModelFileName = L"Resources/Models/stage2.cmo";
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
		for (std::unique_ptr<Stage>& stage : m_stage)
		{
			//プレイヤーとの距離を図る用の変数
			DirectX::SimpleMath::Vector2 stagePosition = DirectX::SimpleMath::Vector2(stage->GetPosition().x, stage->GetPosition().z);
			//ステージとの距離を図る用の変数
			DirectX::SimpleMath::Vector2 playerPosition = DirectX::SimpleMath::Vector2(player->GetPosition().x, player->GetPosition().z);
			//ステージとプレイヤーの距離
			float distance = DirectX::SimpleMath::Vector2::Distance(stagePosition, playerPosition);
			//範囲
			static const float OUT_RANGE =14.5f;
			static const float IN_RANGE  =4.0f;
			//範囲内かつプレイヤーのY座標が０以上であればtrue
			if ((distance <= OUT_RANGE && distance >= IN_RANGE) && player->GetPosition().y >= -2.0f&& player->GetPosition().y <= -1.0f)
			{
				player->SetPosition(DirectX::SimpleMath::Vector3(playerPosition.x, stage->GetPosition().y + 1.5f, playerPosition.y));
				//範囲内
				return true;
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
			//プレイヤーとの距離を図る用の変数
			DirectX::SimpleMath::Vector2 stagePosition = DirectX::SimpleMath::Vector2(stage->GetPosition().x, stage->GetPosition().z);
			//ステージとの距離を図る用の変数
			DirectX::SimpleMath::Vector2 itemPosition = DirectX::SimpleMath::Vector2(item->GetPosition().x, item->GetPosition().z);
			//ステージとプレイヤーの距離
			float distance = DirectX::SimpleMath::Vector2::Distance(stagePosition, itemPosition);
			//範囲
			static const float OUT_RANGE = 100.0f;
			static const float IN_RANGE = 1.0f;
			//範囲内かつプレイヤーのY座標が０以上であればtrue
			if ((distance <= OUT_RANGE && distance >= IN_RANGE) && item->GetPosition().y >= -1.0f && item->GetPosition().y <= -2.0f)
			{
				//範囲内
				return true;
			}

		}



		break;
	case StageSelect::Stage3:
		break;
	default:
		break;
	}

	return false;
	return false;
}
