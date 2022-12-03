#include "pch.h"
#include "ShaderSample2.h"
#include "BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include"PlayScene/MyRandom.h"
using namespace DX;
using namespace DirectX;
using namespace DirectX::SimpleMath;

const std::vector<D3D11_INPUT_ELEMENT_DESC> ShaderSample2::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) + sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

ShaderSample2::ShaderSample2()
	:
	m_scale(1.0f),
	m_offSetPos(0.f, 0.f, 0.f)
{
}

ShaderSample2::~ShaderSample2()
{
	Lost();
}
//-----------------------------------------------------------------------------------
//	Create()
//	�V�F�[�_���𐶐�
//	IN	:	�f�o�C�X			DX::DeviceResources* deviceResources
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderSample2::Create()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	auto device = pDR->GetD3DDevice();

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");

	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}

	const wchar_t* name = L"Resources\\Textures\\10.bmp";
	DirectX::CreateWICTextureFromFile(pDR->GetD3DDevice(), name, nullptr, m_texture.GetAddressOf());

	const wchar_t* name2 = L"Resources\\Textures\\shadow.png";
	DirectX::CreateWICTextureFromFile(pDR->GetD3DDevice(), name2, nullptr, m_texture2.GetAddressOf());

	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	m_states = std::make_unique<CommonStates>(device);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

	m_world = Matrix::Identity;


	m_angle = 0.0f;

	int count = 8;

	for (int i = 0; i < count; i++)
	{
		m_positionList.push_back(DirectX::SimpleMath::Vector3(1.0f, 0.0f, 1.0f));
	}

}
//-----------------------------------------------------------------------------------
//	SetRenderState()
//	�J�����A�r���[�A�ˉe�̔��f
//	IN	:	�J����		DirectX::SimpleMath::Vector3 camera
//			�r���[�s��	DirectX::SimpleMath::Matrix view
//			�ˉe�s��	DirectX::SimpleMath::Matrix proj
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderSample2::SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_camera = camera;
	m_view = view;
	m_proj = proj;
}

void ShaderSample2::Initialize(float life, Vector3 pos, Vector3 velocity)
{
	m_startPosition = pos;
	m_startVelocity = velocity;
	m_startLife = life;

	//int seed = MyRandom::GetIntRange(0, 1);
	//
	//if (seed == 0)
	//{
	//	m_wait = 0.5f;

	//}
	//else
	//{
	//	m_wait = 1.0f;
	//}


	Reset();
	m_rutin = 0;
	if (m_velocity.x > 0)
	{

		m_gravity = Vector3(0, -0.001f, 0);
	}
	else if (m_velocity.x < 0)
	{

		m_gravity = Vector3(0, 0.001f, 0);
	}


	m_isGravity = false;

}

void ShaderSample2::Reset()
{
	m_position = m_startPosition;
	m_velocity = m_startVelocity;
	m_life = m_startLife;
	m_scale = 0.2f;
	m_rutin = 0;
	m_time = 0.0f;
}
//-----------------------------------------------------------------------------------
//	Update()
//	�X�V����
//	IN	:	�^�C�}�[		DX::StepTimer timer
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderSample2::Update(StepTimer timer)
{
	//�����Ɏg���b���v(1�b��1.0f)���擾����B
	float time = timer.GetElapsedSeconds();

	//m_wait -= time;

	//if (m_wait > 0)
	//{
	//	return;
	//}

	Vector3 vel = Vector3(0.002f, 0, 0);

	//m_velocity += g;


	//for (auto& position : m_positionList)
	//{
	//	position += m_velocity;
	//}

	if (m_velocity.x > 0)
	{
		m_velocity.x -= vel.x;
	}
	else if (m_velocity.x < 0)
	{
		m_velocity.x += vel.x;
	}




	const float MOVE_TIME = 0.5;
	m_position += m_velocity;
	switch (m_rutin)
	{
	case 0:
		m_time += time;
		m_scale = Lerp(0.4f, 0.6f, m_time / MOVE_TIME);

		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_rutin++;
		}
		break;

	case 1:
		m_time = 0.0f;
		m_rutin ++;
	case 2:
		m_time += time;
		
		m_scale = Lerp(0.6f, 0.0f, m_time / MOVE_TIME);

		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_rutin++;
		}
		break;

	case 3:
		m_time = 0.0f;
		m_rutin++;
	case 4:
		m_time += time;

		m_scale = Lerp(0, 0.0f, m_time / MOVE_TIME);

		if (m_time > MOVE_TIME)
		{
			m_time = MOVE_TIME;
			m_rutin++;
		}
		break;

	


	
	default:
		break;
	}


	

	//if (m_life > m_startLife / 2)
	//{
	//	m_scale -= 0.1f;
	//}
	//else
	//{
	//	m_scale += 0.1f;
	//}


//	m_angle = Lerp(0.0f, 3.140f, 1.f - m_life / m_startLife);

	m_life -= time;

	if (m_life < 0.0f)
	{
		Reset();
	}


}

//-----------------------------------------------------------------------------------
//	Render()
//	�`�揈��(���C���ȊO�̊O���v���j
//	IN	:	void
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderSample2::Render()
{
	////�r���{�[�h��
	//m_world = Matrix::CreateBillboard(m_position, m_camera, Vector3::UnitY);
	////m_world *= Matrix::CreateScale(m_scale);
	////�V�F�[�_�[�𔽓]������
	//Matrix rotmat;
	//rotmat._11 = -1;
	//rotmat._33 = -1;
	//Matrix scaleMat, rotMat;
	//scaleMat = Matrix::CreateScale(m_scale);
	//
	////m_world = rotmat * m_world;
	//m_world = scaleMat  * rotmat * m_world;
	m_world = Matrix::Identity;

	Matrix wrold = Matrix::Identity;
	wrold = Matrix::CreateBillboard(m_position, m_camera, Vector3::UnitY);

	Matrix revMat = Matrix::Identity;
	revMat._11 = -1.0f;
	revMat._33 = -1.0f;

	Matrix scaleMat, rotMat;
	scaleMat = Matrix::CreateScale(m_scale);
	rotMat = Matrix::CreateRotationZ(m_angle);

	m_world = scaleMat * rotMat * revMat * wrold;

	ShaderDraw();
}
//-----------------------------------------------------------------------------------
//	ShaderDraw()
//	���C���`�揈��
//	IN	:	void
//	RE	:	void
//-----------------------------------------------------------------------------------
void ShaderSample2::ShaderDraw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	auto context = pDR->GetD3DDeviceContext();

	// ���_���(�|���S���̂S���_�̍��W���j
	VertexPositionColorTexture vertex[] =
	{
		VertexPositionColorTexture(Vector3::Zero,Vector4::One, Vector2(0.0f, 0.0f)),
		VertexPositionColorTexture(Vector3(0.0f,0.3f,0.0f),Vector4::One, Vector2(0.0f, 0.0f))
	};


	//�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.matView = m_view.Transpose();
	cbuff.matProj = m_proj.Transpose();
	cbuff.matWorld = m_world.Transpose();
	cbuff.Diffuse = Vector4(1.0, 0.3, 0.1, 1);
	/*Vector3 pos = vx[0].position;
	cbuff.Diffuse = Vector4(pos.x, pos.y, pos.z, 1);*/

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
	//ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	ID3D11BlendState* blendstate = m_states->Additive();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);

	// �J�����O�͍�����
	context->RSSetState(m_states->CullNone());

	//�V�F�[�_���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);

	//�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	context->PSSetShaderResources(1, 1, m_texture2.GetAddressOf());


	//�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());



	// �|���S����`��
	m_batch->Begin();
	//m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 2);
	m_batch->End();

	//�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);

}

void ShaderSample2::Draw()
{
}

void ShaderSample2::Lost()
{
	m_CBuffer.Reset();
	m_inputLayout.Reset();
	m_batch.reset();
	m_states.reset();
	m_texture.Reset();
	m_texture2.Reset();
	m_vertexShader.Reset();
	m_pixelShader.Reset();
}
void ShaderSample2::SetGravity(bool gravity)
{
	m_isGravity = gravity;

}



