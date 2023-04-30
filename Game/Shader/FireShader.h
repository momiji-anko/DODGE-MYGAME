#pragma once

#include "StepTimer.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>

class FireShader
{
public:

	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};


	FireShader();
	~FireShader();

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	//static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_SHADER_LAYOUT;

	void Create();
	void Initialize(float life, const DirectX::SimpleMath::Vector3& pos,const DirectX::SimpleMath::Vector3& velocity);
	void Update(DX::StepTimer timer);
	void Render();
	void Reset();
	void Lost();

	void SetRenderState(const DirectX::SimpleMath::Vector3& camera, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);


	

	DirectX::SimpleMath::Vector3 GetPosition() 
	{
		return m_position;
	}

	float Lerp(float start, float end, float t)
	{
		return start + (end - start) * t;
	}

	void SetStartPos(const DirectX::SimpleMath::Vector3& pos) { m_startPosition = pos; }

	void SetOffsetPosition(const DirectX::SimpleMath::Vector3& pos) { m_offsetPos = pos; }

private:

	void ShaderDraw();
	void Draw();

private:

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	DX::StepTimer                           m_timer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputShaderLayout;

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


	DirectX::SimpleMath::Matrix  m_world;
	DirectX::SimpleMath::Matrix  m_view;
	DirectX::SimpleMath::Matrix  m_proj;

	DirectX::SimpleMath::Vector3 m_camera;
	DirectX::SimpleMath::Vector3 m_gravity;
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_velocity;
	float						 m_life;
								 
	DirectX::SimpleMath::Vector3 m_startPosition;
	DirectX::SimpleMath::Vector3 m_startVelocity;
	float						 m_startLife;
								 
	float						 m_wait;
								 

	std::vector < DirectX::SimpleMath::Vector3 > m_positionList;

	float m_rot;

	float m_angle;

	float m_scale;


	DirectX::SimpleMath::Vector4 m_color;

	int m_rutin;

	float m_colorTime;

	float m_time;

	DirectX::SimpleMath::Vector3 m_offsetPos;

};