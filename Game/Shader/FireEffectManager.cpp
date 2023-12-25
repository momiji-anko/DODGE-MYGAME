/*
* *2023/03/10
* FireEffectManager.h
* �t�@�C���[�V�F�[�_�[
* �����@��
*/
#include "pch.h"
#include "DeviceResources.h"
#include "FireEffectManager.h"

#include <SimpleMath.h> 
#include <Effects.h> 
#include <PrimitiveBatch.h> 
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include <CommonStates.h>
#include<time.h>
#include<random>


/// <summary>
/// �V�F�[�_�[�̍쐬
/// </summary>
void FireEffectManager::Create()
{
	//�f�o�C�X���\�[�X�擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//D3D�f�o�C�X�擾
	ID3D11Device1* device = pDR->GetD3DDevice();

	//�o�b�`�G�t�F�N�g�̍쐬
	m_batchEffect = std::make_unique<DirectX::AlphaTestEffect>(device);
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);

	// ���̓��C�A�E�g����
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf());
	
	//�v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(pDR->GetD3DDeviceContext());
	
	//�e�N�X�`���̃��[�h
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/shadow.png",
		nullptr,
		m_texture.GetAddressOf());
	
	//�΂̕��̐�
	int fireNum = 20;
	
	//�΂̕��̐�������
	for (int i = 0; i < fireNum; i++) 
	{
		//����
		std::unique_ptr<FireShader> effect = std::make_unique<FireShader>();
		//�G�t�F�N�g�쐬
		effect->Create();
		//�G�t�F�N�g���X�g�ɒǉ�
		m_effectList.push_back(std::move(effect));
	}

}

/// <summary>
/// ������
/// </summary>
/// <param name="life">����</param>
/// <param name="pos">���W</param>
/// <param name="velocity">�ړ���</param>
void FireEffectManager::Initialize(float life, DirectX::SimpleMath::Vector3 pos)
{
	UNREFERENCED_PARAMETER(pos);

	//�V�[�h�l�ݒ�
	srand((unsigned int)time(NULL));
	//life,pos,vel �̒l��m_effect������������
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		//�U�ꕝ
		int range = 200;
		//�ړ��ʂ������_���ŏo��
		DirectX::SimpleMath::Vector3 vel = DirectX::SimpleMath::Vector3(((rand() % (range * 2)) - range) / (float)range * 0.04f, ((rand() % (range * 2))) / (float)range * 0.01f, 0);
		//�ړ��ʂ����Ȃ��ꍇ��蒼��
		while (vel.Length() < 0.001f)
		{
			vel = DirectX::SimpleMath::Vector3(((rand() % (range * 2)) - range) / (float)range * 0.04f, ((rand() % (range * 2))) / (float)range * 0.01f, 0);
		}
		
		//�G�t�F�N�g������������
		(*ite)->Initialize(life, DirectX::SimpleMath::Vector3::Zero, vel);
	}

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�X�V</param>
void FireEffectManager::Update(const DX::StepTimer& timer)
{
	//�G�t�F�N�g�X�V
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->Update(timer);
	}
}

/// <summary>
/// �`��
/// </summary>
void FireEffectManager::Render()
{
	//�G�t�F�N�g��`�悷��
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->Render();
	}
}

/// <summary>
/// �`��ݒ�
/// </summary>
/// <param name="camera">�J�����̍��W</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�v���W�F�N�V�����s��</param>
void FireEffectManager::SetRenderState(const DirectX::SimpleMath::Vector3& camera, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	//camera,view,proj,�̒l���G�t�F�N�g�ɓn��
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->SetRenderState(camera,view,proj);
	}
}

/// <summary>
/// �������W
/// </summary>
/// <param name="pos">���W</param>
void FireEffectManager::SetStartPosition(const DirectX::SimpleMath::Vector3& pos)
{
	//�S�ẴG�t�F�N�g�ɏ������W��ݒ�
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->SetStartPos(DirectX::SimpleMath::Vector3(pos.x , pos.y, pos.z));
	}

}

/// <summary>
/// �I�t�Z�b�g���W�ݒ�
/// </summary>
/// <param name="pos">���W</param>
void FireEffectManager::SetOffsetPosition(const DirectX::SimpleMath::Vector3& pos)
{
	//�S�ẴG�t�F�N�g�ɃI�t�Z�b�g���W��ݒ�
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->SetOffsetPosition(DirectX::SimpleMath::Vector3(pos.x , pos.y, pos.z));
	}

}
