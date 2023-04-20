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

//ビヘイビアーのインクルード
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
	
	
	//ビヘイビアー作成
	CreateBehavior();
	//jsonを読み込みステージを作成
	LoadStageJson(StageFileName);

}

void StageManager::Update(const DX::StepTimer& timer)
{
	
	CheckStageMoveEnd();

	//ステージの更新
	for (int i = 0; i < m_stage.size(); i++)
	{
		m_stage[i]->Update(timer);
	}

	//頂点座標の更新
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
	//ビヘイビアーの数
	static const int BEHAVIOR_NUM = 6;

	m_behavior.resize(BEHAVIOR_NUM);
	//一番目に落ちる床のビヘイビアー
	m_behavior[0] = std::make_unique<FirstFloorToFallBehavior>();
	//二番目に落ちる床のビヘイビアー
	m_behavior[1] = std::make_unique<SecondFloorToFallBehavior>();
	//三番目に落ちる床のビヘイビアー
	m_behavior[2] = std::make_unique<ThirdFloorToFallBehavior>();

	m_behavior[3] = nullptr;
	//傾く床
	m_behavior[4] = std::make_unique<TiltingFloorbehavior>();
	//回転するキューブ
	m_behavior[5] = std::make_unique<RotationCubeBehavior>();




}

void StageManager::UpdateVertices()
{
	//ステージの数分for文で回す
	for (int i = 0; i < m_stage.size(); i++)
	{
		//vertexの数分回す
		for (int j = 0; j < m_baseVertices.size(); j++)
		{
			//現在の当たり判定用の頂点座標行列
			DirectX::SimpleMath::Matrix nowVertexMatrix = DirectX::SimpleMath::Matrix::Identity;
			//StageのXとZの角度の計算
			DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_stage[i]->GetRotation());
			//現在の頂点座標の計算
			nowVertexMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_baseVertices[j]) * rot * DirectX::SimpleMath::Matrix::CreateTranslation(m_stage[i]->GetPosition());
			//計算した行列を現在の頂点座標計算に代入する
			m_nowVertices[j] = DirectX::SimpleMath::Vector3(nowVertexMatrix._41, nowVertexMatrix._42, nowVertexMatrix._43);
		}
	}
}

/// <summary>
/// ステージjsonを読み込み
/// </summary>
/// <param name="fileName">jsonのパス</param>
void StageManager::LoadStageJson(const std::wstring& fileName)
{
	//fstream作成
 	std::fstream file(fileName, std::ifstream::in);
	//json読み込み
	nlohmann::json stageJson = nlohmann::json::parse(file);
	//ファイルを閉じる
	file.close();

	//ステージの数取得
	int stageNum = stageJson["StageNum"];
	//ステージの数分確保
	m_stagePositions.resize(stageNum);
	//ステージの数分確保
	m_stageType.resize(stageNum);

	//ステージじの情報を読み込む
	for (int i = 0; i < stageNum; i++)
	{
		//番号をストリングにする
		std::string str = std::to_string(i);
		//ステージタイプ取得
		m_stageType[i] = stageJson["Stage"][str.c_str()]["StageType"];
		//ステージのポジションを取得
		m_stagePositions[i].x = stageJson["Stage"][str.c_str()]["X"];
		m_stagePositions[i].y = stageJson["Stage"][str.c_str()]["Y"];
		m_stagePositions[i].z = stageJson["Stage"][str.c_str()]["Z"];
	}

	//頂点の数取得
	int vertexNum = stageJson["VertexNum"];
	//頂点の数分確保
	m_baseVertices.resize(vertexNum);
	m_nowVertices.resize(vertexNum);
	
	//頂点座標の取得
	for (int i = 0; i < vertexNum; i++)
	{
		//番号をストリングにする
		std::string str = std::to_string(i);
		//頂点座標の取得
		m_baseVertices[i].x = stageJson["VertexPosition"][str.c_str()]["X"];
		m_baseVertices[i].y = stageJson["VertexPosition"][str.c_str()]["Y"];
		m_baseVertices[i].z = stageJson["VertexPosition"][str.c_str()]["Z"];
	}

	//ベース頂点座標を現在の頂点座標に代入
	m_nowVertices = m_baseVertices;

	//頂点座標インデックスの数取得
	int vertexIndex = stageJson["VertexIndexNum"];

	//頂点インデックスの数分確保
	m_indices.resize(vertexIndex);

	//頂点インデックスの読み込み
	for (int i = 0; i < vertexIndex; i++)
	{
		//インデックスの配列確保
		m_indices[i].resize(3);
		//番号をストリングにする
		std::string str = std::to_string(i);
		//インデックス番号の取得
		m_indices[i][0] = stageJson["VertexIndex"][str.c_str()]["1"];
		m_indices[i][1] = stageJson["VertexIndex"][str.c_str()]["2"];
		m_indices[i][2] = stageJson["VertexIndex"][str.c_str()]["3"];
	}

	//モデルのファイルのパスを取得（string）
	std::string modelFileName = stageJson["StageModelFileName"];
	//取得したファイルパスをstringからwstringに変換する
	std::wstring modelFile = ConvertWString(modelFileName);
	//モデル作成
	m_stageModel = ModelManager::GetInstance().LoadModel(modelFile.c_str());

	//ステージの数分配列を確保
	m_stage.resize(stageNum);
	
	DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3::One;

	//ステージの作成
	for (int i = 0; i < stageNum; i++)
	{
		//ステージのユニークポインタで作成
		m_stage[i] = std::make_unique<Stage>();
		//読み込んだステージ情報でステージを初期化する
		m_stage[i]->Initialize(DirectX::SimpleMath::Vector3::Zero, m_stagePositions[i], scale, DirectX::SimpleMath::Vector3::Zero, true, m_behavior[m_stageType[i]].get(), m_stageModel, m_commonState);
		//ステージのタイプを設定
		m_stage[i]->SetStageType(static_cast<Stage::StageType>(m_stageType[i]));

	}

}

/// <summary>
/// stringをwstringに変換する
/// </summary>
/// <param name="str">マルチバイト文字列</param>
/// <returns>ワイド文字列</returns>
std::wstring StageManager::ConvertWString(const std::string& str)
{
	//文字数取得用変数作成（引数で必要なため）
	size_t strLengthNum;
	//文字列バッファ作成
	wchar_t* buffer = new wchar_t[str.size() + 1];
	//stringをwchar_t*にして文字列バッファに代入
	mbstowcs_s(&strLengthNum, buffer, str.size() + 1, str.c_str(), _TRUNCATE);
	//文字列バッファをwstringに代入
	std::wstring result = buffer;
	//文字列バッファを削除
	delete[] buffer;
	//結果を返す
	return result;
}

void StageManager::CheckStageMoveEnd()
{
	//全てのステージが行動を終了したか確認
	for (int n = 0; n < m_stage.size(); n++)
	{
		//確認用カウント
		int count = 0;
		//終わっていなければ次へ行く
		if (!m_stage[n]->GetEndFlag())
		{
			continue;
		}
		//終わっているのカウントを増やす
		count++;
		//全てのステージが行動を終了した場合ステージの行動を変化させる
		if (count >= m_stage.size())
		{
			//行動する番号
			std::vector<int> stageNum = { 0,1,2,3 };
			// シャッフル
			std::random_device seed_gen;
			std::mt19937 engine(seed_gen());
			//番号をシャッフルさせる
			std::shuffle(stageNum.begin(), stageNum.end(), engine);
			//シャッフルさせた番号をステージに渡す
			for (int j = 0; j < m_stage.size(); j++)
			{
				//番号をセット
				m_stage[j]->SetStageType(static_cast<Stage::StageType>(stageNum[j]));
				//ステージにリセットをかける
				m_stage[j]->Reset();
			}
			//for文を抜ける
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
/// 線分と板ポリゴンの当たり判定
/// </summary>
/// <param name="vertices">頂点座標</param>
/// <param name="linePos">線分の両端座標</param>
/// <param name="normalVector">法線ベクトルのポインタ</param>
/// <returns>true=当たっている　false=当っていない</returns>
bool StageManager::StageHitCheck(std::vector<DirectX::SimpleMath::Vector3> vertices, std::vector<DirectX::SimpleMath::Vector3> linePos, DirectX::SimpleMath::Vector3* normalVector)
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
	
	//ポリゴンの中心点から線分の両端へのベクトルと法線ベクトルで内積を算出する
	float dot1 = polygonPosAverageToStartLine.Dot(normal);
	float dot2 = polygonPosAverageToEndLine.Dot(normal);
	//計算した内積を掛け算し、０以下であれば当たっている
	if (dot1 * dot2 <= 0)
	{

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