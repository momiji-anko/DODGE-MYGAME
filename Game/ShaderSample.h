#pragma once

#include "StepTimer.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include<map>

class ShaderSample
{
public:

	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};


	ShaderSample();
	~ShaderSample();

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	//static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_SHADER_LAYOUT;

	void Create();
	void Initialize(float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 velocity);
	void Update(DX::StepTimer timer);
	void Render();
	void Reset();
	void Lost();

	void SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	void SetGravity(bool gravity);

	

	DirectX::SimpleMath::Vector3 GetPosition() 
	{
		return m_position;
	}

	float Lerp(float start, float end, float t)
	{
		return start + (end - start) * t;
	}

	void SetStartPos(DirectX::SimpleMath::Vector3& pos) { m_startPosition = pos; }


private:

	void ShaderDraw();
	void Draw();

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
	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture2;
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;


	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	DirectX::SimpleMath::Vector3		m_camera;
	DirectX::SimpleMath::Vector3		m_gravity;
	DirectX::SimpleMath::Vector3		m_position;
	DirectX::SimpleMath::Vector3		m_velocity;
	float								m_life;

	DirectX::SimpleMath::Vector3		m_startPosition;
	DirectX::SimpleMath::Vector3		m_startVelocity;
	float								m_startLife;

	float								m_wait;

	bool								m_isGravity;

	std::vector < DirectX::SimpleMath::Vector3 > m_positionList;

	float m_rot;

	float m_angle;

	float m_scale;

	DirectX::SimpleMath::Vector3 m_offSetPos;

	DirectX::SimpleMath::Vector4 m_color;

	int m_rutin;

	float m_colorTime;

	std::map<int, DirectX::SimpleMath::Vector4> m_colorMap;

};