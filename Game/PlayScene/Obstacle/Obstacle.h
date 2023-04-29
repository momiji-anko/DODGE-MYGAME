/*
* 2023/04/20
* Obstacle.h
* 障害物クラス
* 麻生　楓
*/
#pragma once 
#include"../Actor/Actor.h"
#include"../../Shader/FireEffectManager.h"
#include"../ShadowMap.h"

/// <summary>
/// 障害物
/// </summary>
class Obstacle :public Actor
{
public:
	/// <summary>
	/// タイプを整数型で取得
	/// </summary>
	/// <returns>アイテムタイプ</returns>
	int GetTypeInt() const override { return static_cast<int>(m_type); }
	/// <summary>
	/// タイプを整数型で設定
	/// </summary>
	/// <param name="type">タイプ</param>
	void SetTypeInt(int type) override { m_type = static_cast<ObstacleType>(type); }


	/// <summary>
	/// エフェクト設定
	/// </summary>
	/// <param name="effect">エフェクトの生ポインタ</param>
	void SetEffect(FireEffectManager* effect) override { m_effect = effect; }
	/// <summary>
	/// エフェクトの取得
	/// </summary>
	/// <returns>エフェクト</returns>
	FireEffectManager* GetEffect()const override { return m_effect; }


	/// <summary>
	/// 回転速度取得
	/// </summary>
	/// <returns>回転速度</returns>
	float GetRotSpeed()const override { return m_rotSpeed; }
	/// <summary>
	/// 回転速度の設定
	/// </summary>
	/// <param name="speed">回転速度</param>
	void SetRotSpeed(float speed) override { m_rotSpeed = speed; }

	/// <summary>
	/// プレイヤーの座標取得
	/// </summary>
	/// <returns>プレイヤーの座標</returns>
	DirectX::SimpleMath::Vector3 GetPlayerPosition()const override { return m_playerPosition; }
	/// <summary>
	/// プレイヤーの座標設定
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	void SetPlayerPosition(const DirectX::SimpleMath::Vector3& playerPos)override { m_playerPosition = playerPos; }

	/// <summary>
	/// 探索行動
	/// </summary>
	/// <param name="targetPosition">目標座標</param>
	/// <returns>目標へのベクトル</returns>
	DirectX::SimpleMath::Vector3 Seek(const DirectX::SimpleMath::Vector3& targetPosition);

	/// <summary>
	/// Maxスピード設定
	/// </summary>
	/// <param name="speed">Maxスピード</param>
	void SetMaxSpeed(float speed) { m_maxSpeed = speed; }
	/// <summary>
	/// Maxスピード取得
	/// </summary>
	/// <returns>Maxスピード</returns>
	float GetMaxSpeed() const { return m_maxSpeed; }

	/// <summary>
	/// Maxフォース設定
	/// </sumary>
	/// <param name="force"></param>
	void SetMaxForce(float force) { m_maxForce = force; }
	/// <summary>
	/// Maxフォース取得
	/// </summary>
	/// <returns>Maxフォース</returns>
	float GetMaxForce() const { return m_maxForce; }

	/// <summary>
	/// 蛇行タイム設定
	/// </summary>
	/// <param name="time">蛇行タイム</param>
	void SetTime(float time) override { m_meandelingTime_s = time; }
	/// <summary>
	/// 蛇行タイム取得
	/// </summary>
	/// <returns>蛇行タイム</returns>
	float GetTime()const override { return m_meandelingTime_s; }

public :
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Obstacle();
	 /// <summary>
	 /// デストラクタ
	 /// </summary>
	 ~Obstacle()override;

	 /// <summary>
	 /// 初期化
	 /// </summary>
	 /// <param name="velocity">移動ベロシティ</param>
	 /// <param name="position">座標</param>
	 /// <param name="scale">スケール</param>
	 /// <param name="rotation">角度</param>
	 /// <param name="active">アクティブ</param>
	 /// <param name="behavia">ビヘイビアーの生ポインタ</param>
	 /// <param name="model">モデルの生ポインタ</param>
	void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Vector3& rotation,
		bool active,
		IBehavior* behavia,
		DirectX::Model* model
		)override;

	 /// <summary>
	 /// 更新
	 /// </summary>
	 /// <param name="timer">タイマー</param>
	 void Update(const DX::StepTimer& timer)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラの生ポインタ</param>
	void Draw(Camera* camera)override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// リセット
	/// </summary>
	void Reset()override;

private:

	/// <summary>
	/// 障害物がエリア外にいるか確認
	/// </summary>
	/// <returns>true = エリア外、false = エリア内</returns>
	bool  CheckInArea();



public:
	//障害物タイプの列挙体
	enum class ObstacleType
	{
		//普通の炎
		NORMAL,
		//棒
		STICK,
		//隕石
		METEORITE, 
		//時計回りに回転する棒
		ROTATESTICK,
		//反時計回りに回転する棒
		REVERSE_ROTATESTICK,
		//蛇行する炎
		MEANDERING,
		//鳥
		BIRD,
		
		NONE
		
	};
private:
				 
	//変数					   
	//障害物のタイプ			   
	ObstacleType                 m_type;
	//炎のエフェクト			   
	FireEffectManager*           m_effect;
					 
	//回転スピード							 
	float                        m_rotSpeed;
	// 質量
	float                        m_mass;           
	// 最大移動速度(スピード)
	float                        m_maxSpeed;       
	// 適用される最大の力
	float                        m_maxForce;       
	// 物体に加わる力
	DirectX::SimpleMath::Vector3 m_force;       
				 
	// プレイヤーの座標							 
	DirectX::SimpleMath::Vector3 m_playerPosition;
	//蛇行用タイム
	float                        m_meandelingTime_s;

};