#pragma once 
#include"../Actor/Actor.h"
#include"../ShadowMap.h"
#include"Game/PlayScene/IBehavior.h"


/// <summary>
/// ステージ
/// </summary>
class Stage :public Actor
{
public:
	/// <summary>
	/// タイムの設定
	/// </summary>
	/// <param name="time"></param>
	void SetTime(float time) { m_time = time; }
	/// <summary>
	/// タイムの取得
	/// </summary>
	/// <returns>タイム</returns>
	float GetTime() { return m_time; }

	/// <summary>
	/// 行動終了しているか設定
	/// </summary>
	/// <param name="flag">true=終了している、false=終了していない</param>
	void SetMoveEndFlag(bool flag) { m_endFlag = flag; }
	/// <summary>
	/// 行動終了しているか取得
	/// </summary>
	/// <returns>true=終了している、false=終了していない</returns>
	bool GetMoveEndFlag() { return m_endFlag; }

	/// <summary>
	/// ローテーションタイムの設定
	/// </summary>
	/// <param name="time">ローテーションタイム</param>
	void SetRotationTime(float time) { m_rotationTime_s = time; }
	/// <summary>
	/// ローテーションタイムの取得
	/// </summary>
	/// <returns>ローテーションタイム</returns>
	float GetRotationTime() { return m_rotationTime_s; }

	/// <summary>
	/// オフセット座標設定
	/// </summary>
	/// <param name="position">オフセット座標</param>
	void SetOffsetPosition(const DirectX::SimpleMath::Vector3& position) { m_offsetPosition = position; }
	/// <summary>
	/// オフセット座標取得
	/// </summary>
	/// <returns>オフセット座標</returns>
	DirectX::SimpleMath::Vector3 GetOffsetPosition() { return m_offsetPosition; }

	/// <summary>
	/// ルーチン設定
	/// </summary>
	/// <param name="routine">ルーチン</param>
	void SetRoutine(int routine) { m_routine = routine; }
	/// <summary>
	/// ルーチン取得
	/// </summary>
	/// <returns>ルーチン</returns>
	int GetRoutine() { return m_routine; }

	/// <summary>
	/// 次の角度設定
	/// </summary>
	/// <param name="rotetion">次の角度</param>
	void SetNextRotation(const DirectX::SimpleMath::Vector3& rotetion) { m_nextRotation = rotetion; }
	/// <summary>
	/// 次の角度取得
	/// </summary>
	/// <returns>次の角度</returns>
	DirectX::SimpleMath::Vector3 GetNextRotation()const { return m_nextRotation; }

	/// <summary>
	/// 前の角度設定
	/// </summary>
	/// <param name="rotetion">前の角度</param>
	void SetPreviousRotation(const DirectX::SimpleMath::Vector3& rotetion) { m_previousRotation = rotetion; }
	/// <summary>
	/// 前の角度取得
	/// </summary>
	/// <returns>前の角度</returns>
	DirectX::SimpleMath::Vector3 GetPreviousRotation()const { return m_previousRotation; }

	/// <summary>
	/// 回転しているか
	/// </summary>
	/// <returns>true=回転している、false=回転していない</returns>
	bool  IsRotation() const{ return m_isRotation; }
	/// <summary>
	/// 回転状態の設定
	/// </summary>
	/// <param name="flag">true=回転している、false=回転していない</param>
	void SetIsRotation(bool flag) { m_isRotation = flag; }

	/// <summary>
	/// ベース頂点座標設定
	/// </summary>
	/// <param name="baseVertices">ベース頂点座標</param>
	void SetBaseVertices(const std::vector<DirectX::SimpleMath::Vector3>& baseVertices) { m_baseVertices = baseVertices; }

	/// <summary>
	/// 現在の頂点座標取得
	/// </summary>
	/// <returns>現在の頂点座標</returns>
	std::vector<DirectX::SimpleMath::Vector3> GetNowVertices()const { return m_nowVertices; }

	/// <summary>
	/// インデックス設定
	/// </summary>
	/// <param name="indices">インデックス</param>
	void SetIndices(const std::vector<std::vector<int>>& indices) { m_indices = indices; }
	/// <summary>
	/// インデックス取得
	/// </summary>
	/// <returns>インデックス</returns>
	std::vector<std::vector<int>> GetIndices()const { return m_indices; }

	/// <summary>
	/// シャドウマップの設定
	/// </summary>
	/// <param name="shadowMap">シャドウマップの生ポインタ</param>
	void SetShadowMap(ShadowMap* shadowMap) { m_shadowMap = shadowMap; }


public:
	

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Stage();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Stage()override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="velocity">ベロシティ</param>
	/// <param name="position">座標</param>
	/// <param name="scale">拡大率</param>
	/// <param name="rotation">角度</param>
	/// <param name="active">アクティブ</param>
	/// <param name="behavia">ビヘイビアー</param>
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
	 /// ラープ
	 /// </summary>
	 /// <param name="start">開始数値</param>
	 /// <param name="end">終了数値</param>
	 /// <param name="t">現在の位置</param>
	 /// <returns>現在の数値</returns>
	 float Lerp(float start, float end, float t)
	 {
		 return start + (end - start) * t;
	 }


	 /// <summary>
	 /// リセット
	 /// </summary>
	 void Reset()override;
	
	 /// <summary>
	 /// 現実の頂点座標の更新
	 /// </summary>
	 void UpdateVertices();

private:

	static const float ROTARION_TIME_S;

	//変数
	//ルーチン
	int m_routine;
	//オフセット座標
	DirectX::SimpleMath::Vector3 m_offsetPosition;
	//タイム
	float m_time;
	//シャドウマップの生ポインタ
	ShadowMap* m_shadowMap;
	//行動を終了したか
	bool m_endFlag;
	//回転タイム
	float m_rotationTime_s;
	//前の角度
	DirectX::SimpleMath::Vector3 m_previousRotation;
	//次の角度
	DirectX::SimpleMath::Vector3 m_nextRotation;
	//回転してるか
	bool m_isRotation;
	
	//頂点座標
	std::vector<DirectX::SimpleMath::Vector3>   m_baseVertices;
	//現在の頂点座標
	std::vector<DirectX::SimpleMath::Vector3>   m_nowVertices;
	//インデックス
	std::vector<std::vector<int>>               m_indices;

};