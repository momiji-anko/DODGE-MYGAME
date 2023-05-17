/*
プレイシーン
*/
#pragma once
#include<Keyboard.h>
#include <CommonStates.h>
#include <Model.h>
#include <GeometricPrimitive.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

#include "../IScene.h"
#include "Game/GameMain.h"
#include"Player/Player.h"
#include"Player/PlayerManager.h"
#include"Actor/Actor.h"
#include"Obstacle/ObstacleManager.h"
#include"Stage/Stage.h"
#include"Item/ItemManager.h"
#include"ShadowMap.h"
#include"Stage/StageManager.h"
#include"AliveTimer.h"

#include"Libraries/AXD2/Adx2.h"
#include"../../Helpers/CueSheet_0.h"
#include"../../Helpers/DODGESound_acf.h"
#include"Objects.h"
#include"Game/Shader/Fade.h"

class GridFloor;
class Camera;

/// <summary>
/// プレイシーン
/// </summary>
class PlayScene : public IScene
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <returns>次のシーン</returns>
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// リソース読み込み
	/// </summary>
	void LoadResources() override;


	/// <summary>
	/// プレイヤーのモード（一人か二人か）
	/// </summary>
	/// <param name="mode">Player1=一人、Player2=二人、</param>
	void SetPlayerMode(GameMain::PlayerMode mode) { m_playerMode = mode; }

	/// <summary>
	/// ステージ番号
	/// </summary>
	/// <param name="stageNum">ステージ番号</param>
	void SetStageNum(int stageNum)override { m_stageNum = static_cast<StageManager::StageSelect>(stageNum); }

private:

	/// <summary>
	/// プレイヤーの作成
	/// </summary>
	void CreatePlayer();

	/// <summary>
	/// 全てプレイヤーが死んでいるか確認
	/// </summary>
	/// <returns>true=全て死んでいる、false=全て死んでいない</returns>
	bool AllPlayerIsDead();

	/// <summary>
	/// カウントダウンの表示
	/// </summary>
	void DrawCountDown();

	/// <summary>
	/// 影生成
	/// </summary>
	void CreateShadow();

	/// <summary>
/// クランプ関数
/// </summary>
/// <param name="n">丸め処理を行いたい数値</param>
/// <param name="min">丸め処理を行う最小値</param>
/// <param name="max">丸め処理を行う最大値</param>
/// <returns>丸め処理を行った結果となる数値</returns>
	float Clamp(float n, float min, float max)
	{
		return std::min(std::max(min, n), max);
	}

	/// <summary>
	/// ２つの線分の最短距離の平方を返す関数
	/// </summary>
	/// <param name="p1">線分 1 の始点</param>
	/// <param name="q1">線分 1 の終点</param>
	/// <param name="p2">線分 2 の始点</param>
	/// <param name="q2">線分 2 の終点</param>
	/// <param name="s">線分１上の最短位置を表す係数</param>
	/// <param name="t">線分 2 上の最短位置を表す係数</param>
	/// <param name="c1">線分１上の最短距離の位置 </param>
	/// <param name="c2">線分 2 上の最短距離の位置</param>
	/// <returns>２つの線分の最短距離の平方</returns>
	float ClosestPtSegmentSegment(const DirectX::SimpleMath::Vector3& p1, const DirectX::SimpleMath::Vector3& q1, const DirectX::SimpleMath::Vector3& p2, const DirectX::SimpleMath::Vector3& q2, float& s, float& t, DirectX::SimpleMath::Vector3& c1, DirectX::SimpleMath::Vector3& c2)
	{
		// p1→q1 のベクトルを算出
		DirectX::SimpleMath::Vector3 d1 = q1 - p1;
		// p2→q2 のベクトルを算出
		DirectX::SimpleMath::Vector3 d2 = q2 - p2;
		// 二つのカプセルが持つ線分の、始点間のベクトルを半径の合計として算出
		DirectX::SimpleMath::Vector3 r = p1 - p2;
		// p1→q1 のベクトルの長さの 2 乗を算出
		float a = d1.Dot(d1);
		// p2→q2 のベクトルの長さの 2 乗を算出
		float e = d2.Dot(d2);
		// p1→q1 のベクトルと半径の合計となるベクトルを内積する
		float f = d2.Dot(r);
		if (a <= FLT_EPSILON && e <= FLT_EPSILON)
		{
			// a と e の両方がほぼ 0 だったら、二つのカプセルは平行である
			s = t = 0.0f;
			// 仮として各始点を獲得する
			c1 = p1;
			c2 = p2;
			// 仮取得した点同士の距離を求める
			return (c1 - c2).Dot(c1 - c2);
		}
		if (a <= FLT_EPSILON)
		{
			// a がほぼ 0 だった場合は、f/e を t として計算する
			// ただし、0～1 の間に補正する
			s = 0.0f;
			t = f / e;
			t = Clamp(t, 0.0f, 1.0f);
		}
		else
		{
			// p1→q1 のベクトルと半径の合計を内積する
			float c = d1.Dot(r);
			if (e <= FLT_EPSILON)
			{
				// e がほぼ 0 だった場合は、s として-c/a を計算する。
				// ただし、0～1 の間に補正する
				t = 0.0f;
				s = Clamp(-c / a, 0.0f, 1.0f);
			}
			else
			{
				// 特に 0 除算が起きる危険はないので、式通りの計算を行う
				// 各カプセルのベクトルで内積する
				float b = d1.Dot(d2);
				// それぞれのカプセルの長さの 2 乗を掛けた値と、
				// 各カプセルのベクトルで内積した結果の 2 乗を比べる
				float denom = a * e - b * b;
				if (denom != 0.0f)
				{
					// 長さが違っていれば、式通りの計算を行う
					s = Clamp((b * f - c * e) / denom, 0.0f, 1.0f);
				}
				else
				{
					// 二つのカプセルの向きがほぼ 90 度の場合、s を 0 とする
					s = 0.0f;
				}
				// b * s + f を計算して検証する
				float tnom = (b * s + f);
				if (tnom < 0.0f)
				{
					// マイナスの値になっていたら、-c / a の値を s とする
					t = 0.0f;
					s = Clamp(-c / a, 0.0f, 1.0f);
				}
				else if (tnom > e)
				{
					// e より大きければ、(b - c) / a の値を s とする
					t = 1.0f;
					s = Clamp((b - c) / a, 0.0f, 1.0f);
				}
				else
				{
					// 式通りの計算を行う
					t = tnom / e;
				}
			}
		}
		// それぞれのカプセルにおける、一番近い点を算出する
		c1 = p1 + d1 * s;
		c2 = p2 + d2 * t;

		// 一番近い二点の距離の二乗を返す
		return (c1 - c2).Dot(c1 - c2);
	}

	/// <summary>
	/// カプセルとカプセルの衝突判定関数
	/// </summary>
	/// <param name="capsule1">当たり判定を取りたいカプセル 1 つ目</param>
	/// <param name="capsule2">当たり判定を取りたいカプセル 2 つ目</param>
	/// <returns>true= 当たった、false=当たってない</returns>
	bool HitCheck_Capsule2Capsule(Capsule capsule1, Capsule capsule2)
	{
		float s, t;
		DirectX::SimpleMath::Vector3 c1, c2;
		// カプセルの中心の線分間の距離の平方を計算
		// s, t, c1, c2 は戻り値
		float dist2 = ClosestPtSegmentSegment(capsule1.a, capsule1.b, capsule2.a, capsule2.b, s, t, c1, c2);
		float radius = capsule1.r + capsule2.r;
		if (dist2 <= radius * radius)
		{
			PlayerCapuslePenetration(m_players[0].get(), c2 - c1, capsule1.r, capsule2.r);
			PlayerCapuslePenetration(m_players[1].get(), c1 - c2, capsule1.r, capsule1.r);
		}

		return dist2 <= radius * radius;
	}

	void PlayerCapuslePenetration(Actor* player, const DirectX::SimpleMath::Vector3& cupseleToCupseVector, float playerCapsleRadius, float obstacleCupsleRadius)
	{
		//当たったカプセルとカプセルの距離
		float cupseleToCupselLenth = cupseleToCupseVector.Length();
		//プレイヤーと障害物の半径の合計
		float radius = playerCapsleRadius + obstacleCupsleRadius;

		DirectX::SimpleMath::Vector3 capToCapVector = cupseleToCupseVector;

		//当たったカプセルとカプセルの距離がプレイヤーと障害物の半径の合計より小さければめり込み処理をする
		if (cupseleToCupselLenth < radius)
		{
			//どれだけめり込んでいるか計算
			float capToCapLengthToRasiusDifference = cupseleToCupselLenth - radius;
			//当たったカプセルとカプセルのベクトルを正規化
			capToCapVector.Normalize();
			//当たったカプセルとカプセルのベクトルにめり込んだ量をかける		
			DirectX::SimpleMath::Vector3 playerPenetrationVelocity = capToCapVector * capToCapLengthToRasiusDifference;
			//プレイヤーのめり込んだ量を押し出す
			player->SetPosition(player->GetPosition() + playerPenetrationVelocity);
		}

	}

private:
	//カウントダウンタイム
	static const float COUNT_DOWN_TIME_S;

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	//天球
	DirectX::Model* m_pModel;

	//障害物マネージャー
	std::unique_ptr<ObstacleManager>                 m_obstacleManager;
	//アイテムマネージャー
	std::unique_ptr<ItemManager>                     m_itemManager;
	//ステージマネージャー
	std::unique_ptr<StageManager>                    m_stageManager;

	//カメラ
	std::unique_ptr<Camera> m_pCamera;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch>            m_spriteBatch;
	//カウントダウンテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_countDownTexture;
	//カウントダウン
	float                                            m_countDownTime;

	//キーボードステートトラッカー
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardStateTracker;

	//シャドウマップ
	std::unique_ptr<ShadowMap>                       m_shadowMap;


	//音関連
	ADX2* m_pAdx2;
	//BGMのID
	int  m_musicID;

	//フェード
	std::unique_ptr<Fade> m_fadeInOut;

	//アライブタイム
	AliveTimer* m_aliveTime;
	//2P用に配列でプレイヤーを定義
	std::vector<std::unique_ptr<Player>> m_players;
	//プレイヤーマネージャー
	std::unique_ptr<PlayerManager> m_playerManager;
	//プレイヤーモード
	GameMain::PlayerMode m_playerMode;
	//ステージ
	StageManager::StageSelect m_stageNum;

	//プレイヤーの操作方法画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_playerMoveKey;
	bool m_isTabKey;

};