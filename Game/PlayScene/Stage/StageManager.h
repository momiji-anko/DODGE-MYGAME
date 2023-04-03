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

class StageManeger
{
public:

	enum class StageSelect
	{
		Stage1,
		Stage2,
		Stage3
	};

private:

	std::unique_ptr<DirectX::Model>           m_stageModel;
	std::unique_ptr<DirectX::GeometricPrimitive>           m_geo;

	std::vector<DirectX::SimpleMath::Vector3> m_stagePositions;

	std::vector<std::unique_ptr<Stage>>       m_stage;

	DirectX::CommonStates*                    m_commonState;
	
	StageSelect m_stageSelect;

	std::vector<std::string> m_stageData;

	float m_time_s;
	std::vector<bool> m_flag;

	std::vector<DirectX::SimpleMath::Vector3> m_baseVertices;
	std::vector<DirectX::SimpleMath::Vector3> m_nowVertices;
	std::vector<std::vector<int>> m_indices;

	std::vector<std::unique_ptr<IBehavior>> m_behavior;

public:

	StageManeger();
	~StageManeger();

	// 初期化
	void Initialize(DirectX::CommonStates* commonState, StageSelect stage = StageSelect::Stage1);

	// 更新
	void Update(const DX::StepTimer& timer);

	// 描画
	void Draw(Camera* camera);

	// 終了処理
	void Finalize();

	bool PlayerStageAABBHitCheck(Actor* player);
	bool ItemHitCheck(Actor* item);

	bool LoadGraphDataByJSON(const std::wstring& fileName);

	void ParseJSON();

	void LoadJson(nlohmann::json json);

	void SetShadow(ShadowMap* shadow);

	void CreateBehavior();

	// 点 c と線分 ab の間の距離の平方（2 乗した値）を返す関数
	// a: 線分の始点
	// b: 線分の終点
	// c: 点
	// 返り値: 点 c と線分 ab の間の距離の平方
	float SqDistPointSegment(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b, DirectX::SimpleMath::Vector3 c)
	{
		DirectX::SimpleMath::Vector3 ab = b - a; // ベクトル ab を算出
		DirectX::SimpleMath::Vector3 ac = c - a; // ベクトル ac を算出
		DirectX::SimpleMath::Vector3 bc = c - b; // ベクトル bc を算出
		// ベクトル ac とベクトル ab の内積を計算
		float e = ac.Dot(ab);
		if (e <= 0.0f)
		{
			// c を射影した点が a 側へ外れているので、a と c の距離を返す
			return ac.Dot(ac);
		}
		// ab 同士の内積を計算する
		float f = ab.Dot(ab);
		if (e >= f)
		{
			// c を射影した点が b 側へ外れているので、b と c の距離を返す
			return bc.Dot(bc);
		}
		// c と c を射影した ab 上の点との距離を返す
		return ac.Dot(ac) - e * e / f;
	}


	// 球とカプセルの衝突判定関数
	bool HitCheck_Sphere2Capsule(Sphere sphere, Capsule capsule)
	{
		// 球の中心とカプセルの中心の線分との距離の平方を計算
		float dist2 = SqDistPointSegment(capsule.a, capsule.b, sphere.c);
		// 球の半径とカプセルの半径の合計を算出
		float radius = sphere.r + capsule.r;
		// dist2 が radius の 2 乗の結果以下となっていれば、当たっている
		return dist2 <= radius * radius;
	}

	/// <summary>
	/// 線分と板ポリゴンの当たり判定
	/// </summary>
	/// <param name="vertices">頂点座標</param>
	/// <param name="linePos">線分の両端座標</param>
	/// <param name="normalVector">法線ベクトルのポインタ</param>
	/// <returns>true=当たっている　false=当っていない</returns>
	bool StageHitCheck(std::vector<DirectX::SimpleMath::Vector3> vertices, std::vector<DirectX::SimpleMath::Vector3> linePos, DirectX::SimpleMath::Vector3* normalVector);


};