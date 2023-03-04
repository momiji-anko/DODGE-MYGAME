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

		//内側３　外１０．５\0
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

		//内側３　外１０．５
		//基本頂点座標
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
		//現在の
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
			// シャッフル
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
			//現在の当たり判定用の頂点座標行列
			DirectX::SimpleMath::Matrix nowVertexMatrix = DirectX::SimpleMath::Matrix::Identity;
			//StageのXとZの角度の計算
			DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::CreateRotationX(m_stage[i]->GetRotation().x / 180.0f * 3.14f) * DirectX::SimpleMath::Matrix::CreateRotationZ(m_stage[i]->GetRotation().z / 180.0f * 3.14f);
			//現在の頂点座標の計算
			nowVertexMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_baseVertices[j] ) * rot * DirectX::SimpleMath::Matrix::CreateTranslation(m_stage[i]->GetPosition());
			//計算した行列を現在の頂点座標計算に代入する
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
					//めり込み処理
					//プレイヤーの座標取得
					DirectX::SimpleMath::Vector3 newPlayerPos = player->GetPosition();
					//プレイヤーの立つY座標計算
					newPlayerPos.y = v[0].y + (-normalVec.x * (newPlayerPos.x - v[0].x) - normalVec.z * (newPlayerPos.z - v[0].z)) / normalVec.y;
					//プレイヤーがガクガクするので少しだけ下にずらす
					newPlayerPos.y -= 0.1;
					//計算したポジションをプレイヤーにセットする
					player->SetPosition(newPlayerPos);

					//スライドベクトル計算
					//法線ベクトルの正規化
					normalVec.Normalize();
					//プレイヤーの移動ベクトルと法線ベクトルの内積の計算
					float aa = playerVel.Dot(normalVec);
					//プレイヤーの移動ベクトルを計算した内積で引く
					DirectX::SimpleMath::Vector3  f = { playerVel.x - aa,playerVel.y - aa,playerVel.z - aa };
					//法線ベクトルと	ｆ掛け算しスライドベクトルをけいさん
					DirectX::SimpleMath::Vector3 slideVec = normalVec * f  ;
					//現在のプレイヤー座標にスライドベクトルを足しプレイヤーにセットする
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
/// 線分と板ポリゴンの当たり判定
/// </summary>
/// <param name="vertices">頂点座標</param>
/// <param name="linePos">線分の両端座標</param>
/// <param name="normalVector">法線ベクトルのポインタ</param>
/// <returns>true=当たっている　false=当っていない</returns>
bool StageManeger::StageHitCheck(std::vector<DirectX::SimpleMath::Vector3> vertices, std::vector<DirectX::SimpleMath::Vector3> linePos, DirectX::SimpleMath::Vector3* normalVector)
{
	//各頂点座標
	DirectX::SimpleMath::Vector3 vertex0 = vertices[0];
	DirectX::SimpleMath::Vector3 vertex1 = vertices[1];
	DirectX::SimpleMath::Vector3 vertex2 = vertices[2];

	//線分の両端座標
	DirectX::SimpleMath::Vector3 startLine = linePos[0];
	DirectX::SimpleMath::Vector3 endLine = linePos[1];

	//法線ベクトル算出のためのデータ
	//頂点０から頂点1のベクトル
	DirectX::SimpleMath::Vector3 v1 = vertex1 - vertex0;
	//頂点1から頂点2のベクトル
	DirectX::SimpleMath::Vector3 v2 = vertex2 - vertex1;

	//法線ベクトル
	DirectX::SimpleMath::Vector3 normal;
	//法線ベクトルの算出
	normal = v1.Cross(v2);
	DirectX::SimpleMath::Vector3 polygonPosAverage = (vertex0 + vertex1 + vertex2) / 3;

	//ポリゴンの中心点から線分の両端へのベクトルを計算
	DirectX::SimpleMath::Vector3 polygonPosAverageToStartLine = startLine - polygonPosAverage;
	DirectX::SimpleMath::Vector3 polygonPosAverageToEndLine = endLine - polygonPosAverage;

	///線分を平面の衝突判定========================================================================
	//http://marupeke296.com/COL_3D_No3_LineToPlane.html
	//ポリゴンの中心点から線分の両端へのベクトルと法線ベクトルで内積を算出する
	float dot1 = polygonPosAverageToStartLine.Dot(normal);
	float dot2 = polygonPosAverageToEndLine.Dot(normal);
	//計算した内積を掛け算し、０以下であれば当たっている
	if (dot1 * dot2 <= 0)
	{

		//http://marupeke296.com/COL_Basic_No2_ShortTec.html#%E3%81%82%E3%82%8B%E7%82%B9%E3%81%8B%E3%82%89%E5%B9%B3%E9%9D%A2%E3%81%BE%E3%81%A7%E3%81%AE%E8%B7%9D%E9%9B%A2
		//ポリゴンから線分の両端の距離の計算
		float d1 = std::abs(polygonPosAverageToStartLine.Dot(normal));
		float d2 = std::abs(polygonPosAverageToEndLine.Dot(normal));
		//内分比の計算
		float a = d1 / (d1 + d2);

		//頂点０から線分の両端のベクトル
		DirectX::SimpleMath::Vector3 vertex0TostartLine = startLine - vertex0;
		DirectX::SimpleMath::Vector3 vertex0ToendLine = endLine - vertex0;
		//頂点０から線分の内分点計算
		DirectX::SimpleMath::Vector3 v3 = ((1 - a) * vertex0TostartLine) + (a * vertex0ToendLine);
		//内分点の座標計算
		DirectX::SimpleMath::Vector3 v3Pos = vertex0 + v3;

		//ポリゴン上に点があるか判定する
		//http://marupeke296.com/COL_Basic_No2_ShortTec.html#%E3%83%9D%E3%83%AA%E3%82%B4%E3%83%B3%E4%B8%8A%E3%81%AB%E7%82%B9%E3%81%8C%E5%90%AB%E3%81%BE%E3%82%8C%E3%82%8B%E3%81%8B%E3%82%92%E5%88%A4%E5%AE%9A

		//V3Posから各頂点の座標へのベクトル作成
		DirectX::SimpleMath::Vector3 vertex0ToV3Pos = v3;
		DirectX::SimpleMath::Vector3 vertex1ToV3Pos = v3Pos - vertex1;
		DirectX::SimpleMath::Vector3 vertex2ToV3Pos = v3Pos - vertex2;

		//左回りになるようにベクトルを作成する
		//頂点０から頂点１へのベクトル
		DirectX::SimpleMath::Vector3 vertex0ToVertex1 = vertex1 - vertex0;
		//頂点１から頂点２へのベクトル
		DirectX::SimpleMath::Vector3 vertex1ToVertex2 = vertex2 - vertex1;
		//頂点２から頂点０へのベクトル
		DirectX::SimpleMath::Vector3 vertex2ToVertex0 = vertex0 - vertex2;

		//V3Posから０頂点の座標へのベクトルと頂点０から頂点１へのベクトルの外積算出
		DirectX::SimpleMath::Vector3 cross1 = vertex0ToV3Pos.Cross(vertex0ToVertex1);
		//V3Posから１頂点の座標へのベクトルと頂点１から頂点２へのベクトルの外積算出
		DirectX::SimpleMath::Vector3 cross2 = vertex1ToV3Pos.Cross(vertex1ToVertex2);
		//V3Posから２頂点の座標へのベクトルと頂点２から頂点０へのベクトルの外積算出
		DirectX::SimpleMath::Vector3 cross3 = vertex2ToV3Pos.Cross(vertex2ToVertex0);

		//算出した外積同士を内積で計算
		float checkDot1 = cross1.Dot(cross2);
		float checkDot2 = cross2.Dot(cross3);
		float checkDot3 = cross3.Dot(cross1);
		//算出した内積を掛け算し０以上であればポリゴンの中に線分がある
		if (checkDot1 * checkDot2 >= 0 && checkDot2 * checkDot3 >= 0 && checkDot3 * checkDot1 >= 0)
		{
			*normalVector = normal;
			return true;
		}
	}
	return false;
}