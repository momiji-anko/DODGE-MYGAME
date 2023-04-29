#include "pch.h"
#include "Fade.h"
#include"BinaryFile.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include<Mouse.h>
#include<Keyboard.h>
#include"DeviceResources.h"



const std::vector<D3D11_INPUT_ELEMENT_DESC> Fade::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

Fade::Fade():
	m_CBuffer{},
	m_timer{},
	m_inputLayout{},
	m_batch{},
	m_states{},
	m_texture{},
	m_vertexShader{},
	m_pixelShader{},
	m_geometryShader{},
	m_centerPosition{},
	m_billboardTranslation{},
	m_view{},
	m_proj{},
	m_model{},
	m_capture{},
	m_rtv{},
	m_srv{},
	m_fadeTime_s{},
	m_fadeState{}
{
	
	
}

Fade::~Fade()
{
	Lost();
}
//-----------------------------------------------------------------------------------
//	Create()
//	�V�F�[�_���𐶐�
//	IN	:	�f�o�C�X			DX::DeviceResources* deviceResources
//	RE	:	void
//-----------------------------------------------------------------------------------
void Fade::Create()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	ID3D11Device1* device = pDR->GetD3DDevice();

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/FadeVS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/FadePS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/FadeGS.cso");

	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf()
	);
	

	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}

	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	const wchar_t* name = L"Resources\\Textures\\089.png";
	DirectX::CreateWICTextureFromFile(pDR->GetD3DDevice(), name, nullptr, m_texture.GetAddressOf());



	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	m_states = std::make_unique<DirectX::CommonStates>(device);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage		=	D3D11_USAGE_DEFAULT;
	bd.ByteWidth	=	sizeof(ConstBuffer);
	bd.BindFlags	=	D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;	
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);


	/*std::unique_ptr<EffectFactory> fxFactory = std::make_unique<EffectFactory>(device);
	fxFactory->SetDirectory(L"Resources/Models");

	m_model = Model::CreateFromCMO(device, L"Resources/Models/cup.cmo", *fxFactory);*/

}
//-----------------------------------------------------------------------------------
//	SetRenderState()
//	�J�����A�r���[�A�ˉe�̔��f
//	IN	:	�J����		DirectX::SimpleMath::Vector3 camera
//			�r���[�s��	DirectX::SimpleMath::Matrix view
//			�ˉe�s��	DirectX::SimpleMath::Matrix proj
//	RE	:	void
//-----------------------------------------------------------------------------------
void Fade::SetRenderState(const DirectX::SimpleMath::Vector3& camera, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	m_view = view;
	m_proj = proj;

	m_billboardTranslation = DirectX::SimpleMath::Matrix::CreateBillboard(m_centerPosition, camera, DirectX::SimpleMath::Vector3::UnitY);

	//�r���{�[�h�̌v�Z�ŗ��Ԃ�̂ŕ␳
	//Y����180�x��]����s��
	DirectX::SimpleMath::Matrix rev = DirectX::SimpleMath::Matrix::Identity;
	rev._11 = -1;
	rev._33 = -1;

	//�␳�s����ɂ����đ��ɉe�����Ȃ��悤�ɂ���
	m_billboardTranslation = rev * m_billboardTranslation;
}

void Fade::Initialize(const DirectX::SimpleMath::Vector3& pos, float fade)
{
	m_centerPosition = pos;
	m_fadeTime_s = fade;
	m_fadeState = State::FADE_NONE;
}

//-----------------------------------------------------------------------------------
//	Update()
//	�X�V����
//	IN	:	�^�C�}�[		DX::StepTimer timer
//	RE	:	void
//-----------------------------------------------------------------------------------
void Fade::Update(DX::StepTimer timer)
{
	m_timer = timer;
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	switch (m_fadeState)
	{
	case Fade::State::FADE_IN:
		m_fadeTime_s -= elapsedTime;

		if (m_fadeTime_s <= 0)
		{
			m_fadeState = State::FADE_NONE;
		}

		break;
	case Fade::State::FADE_OUT:
		m_fadeTime_s += elapsedTime;

		if (m_fadeTime_s >= 1)
		{
			m_fadeState = State::FADE_NONE;
		}
		break;
	case Fade::State::FADE_NONE:
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------------
//	Render()
//	�`�揈��(���C���ȊO�̊O���v���j
//	IN	:	void
//	RE	:	void
//-----------------------------------------------------------------------------------
void Fade::Render()
{
	//��]���郂�f����\��
	static float rot = 0.0f;
	rot += 0.1f;
	//m_model->Draw(pDR->GetD3DDeviceContext(),*m_states, Matrix::CreateRotationZ(rot), m_view, m_proj);

	//�S��ʃG�t�F�N�g
	DirectX::SimpleMath::Matrix  mat = DirectX::SimpleMath::Matrix::Identity;
	Draw(mat, mat, mat);

	//�|���S���G�t�F�N�g
	//Draw(m_billboardTranslation, m_view, m_proj);
}
//-----------------------------------------------------------------------------------
//	Draw()
//	���C���`�揈��
//	IN	:	void
//	RE	:	void
//-----------------------------------------------------------------------------------
void Fade::Draw(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// ���_���(�|���S���̒��_�̍��W���j
	DirectX::VertexPositionColorTexture vertex[1] =
	{
		DirectX::VertexPositionColorTexture(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),DirectX::SimpleMath::Vector4::One, DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
	};

	//�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = world.Transpose();
	//Time		x:�o�ߎ���(�g�[�^���b)	y:1F�̌o�ߎ���(�b�j	z:�����i�T�C���J�[�u�j w:���g�p�i�b��łP�j
	
	//cbuff.Time = Vector4((float)m_timer.GetTotalSeconds(), (float)m_timer.GetElapsedSeconds(), Mouse::Get().GetState().x / 800.0f, Mouse::Get().GetState().y / 600.0f);
	
	cbuff.Time = DirectX::SimpleMath::Vector4((float)m_timer.GetTotalSeconds(), m_fadeTime_s, DirectX::Mouse::Get().GetState().x / 800.0f, DirectX::Mouse::Get().GetState().y / 600.0f);
	
	
	//�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);

	//�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//�������`��w��
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	// �J�����O�͔����v���
	context->RSSetState(m_states->CullCounterClockwise());

	//�V�F�[�_���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);

	//�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());


	//�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	// �|���S����`��
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();

	//�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);

}

void Fade::Lost()
{
	m_CBuffer.Reset();
	m_inputLayout.Reset();
	m_batch.reset();
	m_states.reset();
	m_texture.Reset();
	m_vertexShader.Reset();
	m_pixelShader.Reset();
}

//���f�������e�N�X�`��(m_srv)�ɕ`�悷�鏈��
void Fade::RenderModel()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	D3D11_TEXTURE2D_DESC texDesc;
	pDR->GetRenderTarget()->GetDesc(&texDesc);
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	pDR->GetD3DDevice()->CreateTexture2D(&texDesc, NULL, m_capture.ReleaseAndGetAddressOf());

	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	// �����_�[�^�[�Q�b�g�r���[�̐���
	pDR->GetD3DDevice()->CreateRenderTargetView(m_capture.Get(), &rtvDesc, &m_rtv);

	//�V�F�[�_���\�[�X�r���[�̐ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	//�����_�[�^�[�Q�b�g�r���[,�[�x�r���[���擾�i��Ō��ɖ߂����߁j
	ID3D11RenderTargetView* defaultRTV = pDR->GetRenderTargetView();
	ID3D11DepthStencilView* pDSV = pDR->GetDepthStencilView();

	//�w�i�F�̐ݒ�i�A���t�@���O�ɂ���ƃI�u�W�F�N�g�̂ݕ\���j
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 0.0f };

	//�����_�[�^�[�Q�b�g�r���[���Z�b�g���A����������
	context->OMSetRenderTargets(1, m_rtv.GetAddressOf(), pDSV);
	context->ClearRenderTargetView(m_rtv.Get(), clearColor);
	context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//----------------------------------------------------------------------------
	//�Ƃ肠�����̓����̂��߂ɉ�]
	static float rot = 0.0f;
	rot += 0.1f;

	//���f����`��
	m_model->Draw(pDR->GetD3DDeviceContext(), *m_states, DirectX::SimpleMath::Matrix::CreateRotationZ(rot), m_view, m_proj);

	

	//------------------------------------------------------------------
	//�ݒ�����Ƃɖ߂�
	clearColor[0] = 0.3f;
	clearColor[1] = 0.3f;
	context->OMSetRenderTargets(1, &defaultRTV, pDSV);
	context->ClearRenderTargetView(defaultRTV, clearColor);
	context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

