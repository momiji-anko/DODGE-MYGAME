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
	float                        m_seekTime_s;


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
	 /// <param name="commonState">コモンステートの生ポインタ</param>
	void Initialize(
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Vector3& rotation,
		bool active,
		IBehavior* behavia,
		DirectX::Model* model,
		DirectX::CommonStates* commonState)override;

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
	void Reset();

	/// <summary>
	/// タイプ設定
	/// </summary>
	/// <param name="type">障害物のタイプ</param>
	void SetType(ObstacleType type) { m_type = type; }
	/// <summary>
	/// タイプ取得
	/// </summary>
	/// <returns>障害物のタイプ</returns>
	ObstacleType GetObstacleType() { return m_type; }


	/// <summary>
	/// エフェクト設定
	/// </summary>
	/// <param name="effect">エフェクトの生ポインタ</param>
	void SetEffect(FireEffectManager* effect) { m_effect = effect; }
	/// <summary>
	/// エフェクトの取得
	/// </summary>
	/// <returns>エフェクト</returns>
	FireEffectManager* GetEffect() { return m_effect; }

	
	/// <summary>
	/// 回転速度取得
	/// </summary>
	/// <returns>回転速度</returns>
	float& GetRotSpeed() { return m_rotSpeed; }
	/// <summary>
	/// 回転速度の設定
	/// </summary>
	/// <param name="speed">回転速度</param>
	void SetRotSpeed(float speed) { m_rotSpeed = speed; }

	/// <summary>
	/// プレイヤーの座標取得
	/// </summary>
	/// <returns>プレイヤーの座標</returns>
	DirectX::SimpleMath::Vector3 GetPlayerPosition() { return m_playerPosition; }
	/// <summary>
	/// プレイヤーの座標設定
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	void SetPlayerPosition(DirectX::SimpleMath::Vector3 playerPos) { m_playerPosition = playerPos; }

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
	/// シークタイム設定
	/// </summary>
	/// <param name="time">シークタイム</param>
	void SetSeekTime(float time) { m_seekTime_s = time; }
	/// <summary>
	/// シークタイム取得
	/// </summary>
	/// <returns>シークタイム</returns>
	float GetSeekTime() { return m_seekTime_s; }

private:
	/// <summary>
	/// 障害物がエリア外にいるか確認
	/// </summary>
	/// <returns>true = エリア外、false = エリア内</returns>
	bool  CheckInArea();
};