/*
* 2023/03/10
* FireShader.h
* ファイヤーシェーダー
* 麻生　楓
*/
#pragma once

#include "StepTimer.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>

/// <summary>
/// ファイヤーシェーダー
/// </summary>
class FireShader
{
public:
	//インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	/// <summary>
	/// コンストバッファー
	/// </summary>
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	FireShader();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~FireShader();

	/// <summary>
	/// シェーダーの作成
	/// </summary>
	void Create();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="life">寿命</param>
	/// <param name="pos">座標</param>
	/// <param name="velocity">移動量</param>
	void Initialize(float life, const DirectX::SimpleMath::Vector3& pos,const DirectX::SimpleMath::Vector3& velocity);
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">更新</param>
	void Update(DX::StepTimer timer);
	/// <summary>
	/// 描画
	/// </summary>
	void Render();
	/// <summary>
	/// リセット
	/// </summary>
	void Reset();
	/// <summary>
	/// 消去
	/// </summary>
	void Lost();

	/// <summary>
	/// 描画設定
	/// </summary>
	/// <param name="camera">カメラの座標</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="proj">プロジェクション行列</param>
	void SetRenderState(const DirectX::SimpleMath::Vector3& camera, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	DirectX::SimpleMath::Vector3 GetPosition() 
	{
		return m_position;
	}

	/// <summary>
	/// 線形補間
	/// </summary>
	float Lerp(float start, float end, float t)
	{
		return start + (end - start) * t;
	}
	/// <summary>
	/// 初期座標
	/// </summary>
	/// <param name="pos">座標</param>
	void SetStartPos(const DirectX::SimpleMath::Vector3& pos) { m_startPosition = pos; }

	/// <summary>
	/// オフセット座標設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetOffsetPosition(const DirectX::SimpleMath::Vector3& pos) { m_offsetPos = pos; }

private:
	/// <summary>
	/// シェーダー描画
	/// </summary>
	void ShaderDraw();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	//シェーダーに渡すバッファー
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	//タイマー
	DX::StepTimer                           m_timer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	
	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader>   m_vertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader>    m_pixelShader;
	//ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

	//ワールド行列
	DirectX::SimpleMath::Matrix  m_world;
	//ビュー行列
	DirectX::SimpleMath::Matrix  m_view;
	//プロジェクション行列
	DirectX::SimpleMath::Matrix  m_proj;
	//カメラの座標
	DirectX::SimpleMath::Vector3 m_camera;
	//座標
	DirectX::SimpleMath::Vector3 m_position;
	//ベロシティ
	DirectX::SimpleMath::Vector3 m_velocity;
	//寿命
	float						 m_life;
	//初期座標						 
	DirectX::SimpleMath::Vector3 m_startPosition;
	//初期ベロシティ
	DirectX::SimpleMath::Vector3 m_startVelocity;
	//初期寿命
	float						 m_startLife;
	//ウェイト						 
	float						 m_wait;
								 
	//ポジションリスト
	std::vector < DirectX::SimpleMath::Vector3 > m_positionList;
	//回転
	float m_rot;
	//角度
	float m_angle;
	//拡縮	
	float m_scale;
	//オフセット座標
	DirectX::SimpleMath::Vector3 m_offsetPos;

};