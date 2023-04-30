#pragma once

#include "StepTimer.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <Model.h>
#include <vector>

class Fade
{
public:


	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Time;
	};


	Fade();
	~Fade();

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	void Create();
	void Initialize(const DirectX::SimpleMath::Vector3& pos,float fade = 1.0f);
	void Update(DX::StepTimer timer);
	void Render();
	void Lost();

	void SetRenderState(const DirectX::SimpleMath::Vector3& camera, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	void Draw(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

	void RenderModel();

	void FadeIn()  { m_fadeState = State::FADE_IN; }
	void FadeOut() { m_fadeState = State::FADE_OUT; }


	bool ISClose() { return m_fadeTime_s >= 1; }
	bool ISOpen()  { return m_fadeTime_s <= 0; }

private:

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	DX::StepTimer                           m_timer;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;

	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;


	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// �W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

	DirectX::SimpleMath::Vector3 m_centerPosition;

	DirectX::SimpleMath::Matrix m_billboardTranslation;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	std::unique_ptr<DirectX::Model> m_model;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_capture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_rtv;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;


	enum class State
	{
		FADE_IN,
		FADE_OUT,
		FADE_NONE,
	};

	float m_fadeTime_s;
	State m_fadeState;
};