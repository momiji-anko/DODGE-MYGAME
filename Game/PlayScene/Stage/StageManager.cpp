/*
* 2023/04/26
* StageManager.cpp
* ステージマネージャー
* 麻生　楓
*/
#include"pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include<cstdlib>
#include <windows.h>
#include"StageManager.h"
#include"DeviceResources.h"
#include"../MyRandom.h"
#include"Game/PlayScene/GameContext/GameContext.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/Json/json.hpp"

//ビヘイビアーのインクルード
#include"StageBehaviors/NormalFloorBehavior.h"
#include"StageBehaviors/TiltingFloorBehavior.h"
#include"StageBehaviors/RotationCubeBehavior.h"
#include"StageBehaviors/FloorToFallBehavior.h"


//ビヘイビアーの数
const int StageManager::BEHAVIOR_NUM = 6;


/// <summary>
/// ステージにシャドウマップを渡す
/// </summary>
/// <param name="shadow">シャドウマップの生ポインタ</param>
void StageManager::SetShadowMap(ShadowMap* shadow)
{
	for (std::unique_ptr<Stage>& stage : m_stage)
	{
		stage->SetShadowMap(shadow);
	}
}

/// <summary>
/// コンストラクタ
/// </summary>
StageManager::StageManager()
	:
	m_stage{},
	m_behavior{}
{
	

}

/// <summary>
/// デストラクタ
/// </summary>
StageManager::~StageManager()
{
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="stage">ステージ選択</param>
void StageManager::Initialize( StageSelect stage)
{
	//ビヘイビアー作成
	CreateBehavior();

	//ステージjsonファイルパス
	std::vector<std::wstring> stageFileNames;

	//ステージのjsonがあるファイルパス
	std::wstring stageFilePath = L"Resources/StageData/";
	
	//ファイルを読み込む
	stageFileNames = FileLoadManager::GetInstance().LoadFile(stageFilePath);

	size_t stageNumber = static_cast<size_t>(stage);

	//jsonを読み込みステージを作成
	LoadStageJson(stageFileNames[stageNumber]);

}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void StageManager::Update(const DX::StepTimer& timer)
{
	//ステージが行動を終了しているか確認
	CheckStageMoveEnd();

	//プレイヤー死亡していれば更新しない
	if (GameContext::GetInstance().GetIsPlayerDeath())
		return;

	//ステージの更新
	for (int i = 0; i < m_stage.size(); i++)
	{
		m_stage[i]->Update(timer);
	}
}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラの生ポインタ</param>
void StageManager::Draw(Camera* camera)
{
	//描画
	for (std::unique_ptr<Stage>& stage : m_stage)
	{
		stage->Draw(camera);
	}
}

/// <summary>
/// 終了処理
/// </summary>
void StageManager::Finalize()
{

}

/// <summary>
/// ビヘイビアー作成
/// </summary>
void StageManager::CreateBehavior()
{
	//何番目に落ちる床の配列
	std::vector<int> fallFloorList = {1,2,3};

	//ビヘイビアー
	std::vector<IBehavior*> behaviors =
	{
		//一番目に落ちる床
		new FloorToFallBehavior(fallFloorList[0]),
		//二番目に落ちる床
		new FloorToFallBehavior(fallFloorList[1]),
		//三番目に落ちる床
		new FloorToFallBehavior(fallFloorList[2]),
		//普通の床
		new NormalFloorBehavior(),
		//傾く床
		new TiltingFloorBehavior(),
		//回転するキューブ
		new RotationCubeBehavior()
	};

	m_behavior.resize(BEHAVIOR_NUM);
	
	//ビヘイビアー登録
	for (int i = 0; i < behaviors.size(); i++)
	{
		m_behavior[i].reset(behaviors[i]);
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

	//ステージの座標
	std::vector<DirectX::SimpleMath::Vector3> floorPositions;

	//ベース頂点座標
	std::vector<DirectX::SimpleMath::Vector3> baseVertices;
	//インデックス
	std::vector<std::vector<int>> indices;
	//ステートタイプ
	std::vector<int> floor;

	//ステージの数取得
	size_t stageNum = stageJson["Floor"].size();
	//ステージの数分確保
	floorPositions.resize(stageNum);
	//ステージの数分確保
	floor.resize(stageNum);

	//ステージじの情報を読み込む
	for (int i = 0; i < stageNum; i++)
	{
		//番号をストリングにする
		std::string str = std::to_string(i);
		//ステージタイプ取得
		
		floor[i] = stageJson["Floor"][i]["FloorType"];
		//ステージのポジションを取得
		floorPositions[i] = ConvertIntoVector3(stageJson["Floor"][i]["Position"]);
	}

	//頂点の数取得
	int vertexNum = static_cast<int>(stageJson["VertexPosition"].size());
	//頂点の数分確保
	baseVertices.resize(vertexNum);
	
	//頂点座標の取得
	for (int i = 0; i < vertexNum; i++)
	{
		//番号をストリングにする
		std::string str = std::to_string(i);
		//頂点座標の取得
		baseVertices[i] = ConvertIntoVector3(stageJson["VertexPosition"][i]);
	}

	//頂点座標インデックスの数取得
	size_t vertexIndexNum = stageJson["VertexIndex"].size();
	indices.resize(vertexIndexNum);

	//1ポリゴンの頂点座標の数
	int onePolygonVertexIndex = 3;

	//頂点インデックスの読み込み
	for (int i = 0; i < vertexIndexNum; i++)
	{
		//インデックスの配列確保
		indices[i].resize(onePolygonVertexIndex);
		//インデックス番号の取得
		for (int j = 0; j < onePolygonVertexIndex; j++)
		{
			indices[i][j] = stageJson["VertexIndex"][i][j];
		}		
	}

	//モデルのファイルのパスを取得（string）
	std::string modelFileName = stageJson["FloorModelFileName"];
	//取得したファイルパスをstringからwstringに変換する
	std::wstring modelFile = ConvertWString(modelFileName);
	//モデル作成
	DirectX::Model* stageModel = ModelManager::GetInstance().LoadModel(modelFile.c_str());

	//ステージの数分配列を確保
	m_stage.resize(stageNum);
	
	DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3::One;

	//ステージの作成
	for (int i = 0; i < stageNum; i++)
	{
		//ステージのユニークポインタで作成
		m_stage[i] = std::make_unique<Stage>();

		//ステージタイプ設定
		m_stage[i]->SetTypeInt(floor[i]);
		//ベース頂点座標
		m_stage[i]->SetBaseVertices(baseVertices);
		//インデックス設定
		m_stage[i]->SetIndices(indices);

		//読み込んだステージ情報でステージを初期化する
		m_stage[i]->Initialize(DirectX::SimpleMath::Vector3::Zero, floorPositions[i], scale, DirectX::SimpleMath::Vector3::Zero, true, m_behavior[floor[i]].get(), stageModel);
		
		
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

/// <summary>
/// ステージが行動を終了しているか確認
/// </summary>
void StageManager::CheckStageMoveEnd()
{

	int stageEndCount = 0;

	//全てのステージが行動を終了したか確認
	for (int n = 0; n < m_stage.size(); n++)
	{

		//終わっていなければ次へ行く
		if (!m_stage[n]->GetMoveEndFlag())
		{
			continue;
		}

		//終わっているのでカウントを増やす
		stageEndCount++;

		//全てのステージが行動を終了した場合ステージの行動を変化させる
		if (stageEndCount == m_stage.size())
		{
			//行動する番号
			std::vector<int> stageNum = { 0,1,2,3 };
			// シャッフル
			std::random_device seed_gen;
			std::mt19937 engine(seed_gen());
			//番号をシャッフルさせる
			std::shuffle(stageNum.begin(), stageNum.end(), engine);

			//シャッフルした番号をステージに渡す
			for (int j = 0; j < m_stage.size(); j++)
			{
				//ステージにリセットをかける
				m_stage[j]->Reset();

				//番号をセット
				m_stage[j]->SetBehavior(m_behavior[stageNum[j]].get());

			}

		}
	}
}


/// <summary>
/// ステージのポリゴンとアクターのめり込み処理
/// </summary>
/// <param name="actor">アクター/param>
/// <param name="polygonVertexPos">ポリゴンの頂点座標</param>
/// <param name="normalVec">法線ベクトル</param>
void StageManager::ActorPolygonPenetration(Actor* actor, const std::vector<DirectX::SimpleMath::Vector3>& polygonVertexPos, const DirectX::SimpleMath::Vector3& normalVec)
{
	//アクターを少しめり込ませる用変数
	float actorPenetration = 0.1f;

	//めり込み処理
	//アクターの座標取得
	DirectX::SimpleMath::Vector3 actorPos = actor->GetPosition();
	//アクターの立つY座標計算
	actorPos.y = polygonVertexPos[0].y + (-normalVec.x * (actorPos.x - polygonVertexPos[0].x) - normalVec.z * (actorPos.z - polygonVertexPos[0].z)) / normalVec.y;
	//アクターがガクガクするので少しだけ下にめり込ませる
	actorPos.y -= actorPenetration;
	//計算したポジションをアクターにセットする
	actor->SetPosition(actorPos);
}


/// <summary>
/// アクターのスライドベクトルを計算する
/// </summary>
/// <param name="normalVec">ポリゴンの法線ベクトル</param>
/// <param name="actorVel">アクターの移動量</param>
/// <returns>スライドベクトル</returns>
DirectX::SimpleMath::Vector3 StageManager::SlideVecCalculation(const DirectX::SimpleMath::Vector3& normalVec, const DirectX::SimpleMath::Vector3& actorVel)
{
	DirectX::SimpleMath::Vector3 normalVector = normalVec;
	//法線ベクトルの正規化
	normalVector.Normalize();
	//プレイヤーの移動ベクトルと法線ベクトルの内積の計算
	float dot = actorVel.Dot(normalVector);
	//スライドする量計算
	DirectX::SimpleMath::Vector3 slideAmount = { actorVel.x - dot,actorVel.y - dot,actorVel.z - dot };
	//壁擦りベクトルを計算
	DirectX::SimpleMath::Vector3 slideVec = normalVector * slideAmount;
	//スライドベクトルを返す
	return slideVec;
}

/// <summary>
/// jsonで読み込んだ座標をVector3に変換
/// </summary>
/// <param name="nums">jsonで読み込んだ座標</param>
/// <returns>変換した座標</returns>
DirectX::SimpleMath::Vector3 StageManager::ConvertIntoVector3(const std::vector<float> nums)
{
	return DirectX::SimpleMath::Vector3(nums[0], nums[1], nums[2]);
}

/// <summary>
/// ステージとアクターの当たり判定
/// </summary>
/// <param name="actor">アクター</param>
/// <returns>true=当たっている　false=当っていない</returns>
bool StageManager::StageToActorHitCheck(Actor* actor)
{
	//全てステージと当たり判定を取る
	for (std::unique_ptr<Stage>& stage : m_stage)
	{
		//ステージからインデックス取得
		std::vector<std::vector<int>> index(stage->GetIndices());
		//現在の頂点座標取得
		std::vector<DirectX::SimpleMath::Vector3> nowVertices(stage->GetNowVertices());

		//ステージのポリゴンとアクター線分の当たり判定を取る
		for (int i = 0; i < index.size(); i++)
		{
			//１ポリゴンを座標取得
			std::vector<DirectX::SimpleMath::Vector3> polygonVertexPos = { nowVertices[index[i][0]],nowVertices[index[i][1]] ,nowVertices[index[i][2]] };
			//アクターのポジション取得
			const DirectX::SimpleMath::Vector3 actorPos = actor->GetPosition();

			//アクターの当たり判定の線分
			std::vector<DirectX::SimpleMath::Vector3> actorLinePos = { DirectX::SimpleMath::Vector3(actorPos),DirectX::SimpleMath::Vector3(actorPos.x,actorPos.y + 1.5f,actorPos.z) };
			//法線ベクトル
			DirectX::SimpleMath::Vector3 normalVec = DirectX::SimpleMath::Vector3::Zero;

			//当たっていた場合スライドする
			if (StageDetectCollition(polygonVertexPos, actorLinePos, &normalVec))
			{
				//めり込み処理
				ActorPolygonPenetration(actor, polygonVertexPos, normalVec);

				//アクターのベクトル
				DirectX::SimpleMath::Vector3 actorVel = actor->GetVelocity();

				//アクターのスライドする量
				float actorSlideGravity = -1.9f;
				actorVel.y = actorSlideGravity;

				//スライドベクトル計算
				DirectX::SimpleMath::Vector3 slideVec = SlideVecCalculation(normalVec, actorVel);

				//アクターをスライドさせる
				actor->SetPosition((actor->GetPosition() + slideVec));

				//当たった
				return true;

			}

		}

	}
	
	//当っていない
	return false;
}





/// <summary>
/// 線分と板ポリゴンの当たり判定
/// </summary>
/// <param name="vertices">頂点座標</param>
/// <param name="linePos">線分の両端座標</param>
/// <param name="normalVector">法線ベクトルのポインタ</param>
/// <returns>true=当たっている　false=当っていない</returns>
bool StageManager::StageDetectCollition(const std::vector<DirectX::SimpleMath::Vector3>& vertices, const std::vector<DirectX::SimpleMath::Vector3>& linePos, DirectX::SimpleMath::Vector3* normalVector)
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