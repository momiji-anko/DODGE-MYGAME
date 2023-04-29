/*
* 2023/04/26
* StageManager.h
* ステージマネージャー
* 麻生　楓
*/
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


/// <summary>
/// ステージマネージャー
/// </summary>
class StageManager
{
public:
	//前方宣言
	enum class StageSelect;

	/// <summary>
	/// ステージにシャドウマップを渡す
	/// </summary>
	/// <param name="shadow">シャドウマップの生ポインタ</param>
	void SetShadowMap(ShadowMap* shadow);
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	StageManager();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~StageManager();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="stage">ステージ選択</param>
	void Initialize( StageSelect stage = StageSelect::Stage1);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	void Update(const DX::StepTimer& timer);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラの生ポインタ</param>
	void Draw(Camera* camera);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// ステージとアクターの当たり判定
	/// </summary>
	/// <param name="actor">アクター</param>
	/// <returns>true=当たっている　false=当っていない</returns>
	bool StageToActorHitCheck(Actor* actor);


	/// <summary>
	/// 線分と板ポリゴンの当たり判定
	/// </summary>
	/// <param name="vertices">頂点座標</param>
	/// <param name="linePos">線分の両端座標</param>
	/// <param name="normalVector">法線ベクトルのポインタ</param>
	/// <returns>true=当たっている　false=当っていない</returns>
	bool StageHitCheck(std::vector<DirectX::SimpleMath::Vector3> vertices, std::vector<DirectX::SimpleMath::Vector3> linePos, DirectX::SimpleMath::Vector3* normalVector);
private:

	/// <summary>
	/// ビヘイビアー作成
	/// </summary>
	void CreateBehavior();


	/// <summary>
	/// ステージjsonを読み込み
	/// </summary>
	/// <param name="fileName">jsonのパス</param>
	void LoadStageJson(const std::wstring& fileName);

	/// <summary>
	/// stringをwstringに変換する
	/// </summary>
	/// <param name="str">マルチバイト文字列</param>
	/// <returns>ワイド文字列</returns>
	std::wstring ConvertWString(const std::string& str);


	/// <summary>
	/// ステージが行動を終了しているか確認
	/// </summary>
	void CheckStageMoveEnd();

	/// <summary>
	/// ステージのポリゴンとアクターのめり込み処理
	/// </summary>
	/// <param name="actor">アクター/param>
	/// <param name="polygonVertexPos">ポリゴンの頂点座標</param>
	/// <param name="normalVec">法線ベクトル</param>
	void ActorPolygonPenetration(Actor* actor, std::vector<DirectX::SimpleMath::Vector3> polygonVertexPos, DirectX::SimpleMath::Vector3 normalVec);


	/// <summary>
	/// アクターのスライドベクトルを計算する
	/// </summary>
	/// <param name="normalVec">ポリゴンの法線ベクトル</param>
	/// <param name="actorVel">アクターの移動量</param>
	/// <returns>スライドベクトル</returns>
	DirectX::SimpleMath::Vector3 SlideVecCalculation(DirectX::SimpleMath::Vector3 normalVec, DirectX::SimpleMath::Vector3 actorVel);

public:
	//ステージ選択
	enum class StageSelect
	{
		//ステージ１
		Stage1,
		//ステージ２
		Stage2,
		//ステージ３
		Stage3
	};

private:
	//ステージ配列
	std::vector<std::unique_ptr<Stage>>         m_stage;
	//ステージビヘイビアー
	std::vector<std::unique_ptr<IBehavior>>     m_behavior;


};