/*
* 2023/04/10
* TextureManager.cpp
* �e�N�X�`����ǂݍ���&�Ǘ�����
* �����@��
*/
#include"pch.h"
#include"TextureManager.h"
#include"DeviceResources.h"

/// <summary>
/// �e�N�X�`����ǂݍ���
/// </summary>
/// <param name="fileName">�e�N�X�`���̃t�@�C���p�X</param>
/// <returns>�e�N�X�`��</returns>
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureManager::LoadTexture(std::wstring fileName)
{
	if (m_textures[fileName] == nullptr)
	{
		//DeviceResources����f�o�C�X�̎擾
		ID3D11Device1* device = DX::DeviceResources::GetInstance()->GetD3DDevice();

		//�e�N�X�`���̓ǂݍ���
		DirectX::CreateWICTextureFromFile(
			device,
			fileName.c_str(),
			nullptr,
			m_textures[fileName].ReleaseAndGetAddressOf()
		);
	}

	return m_textures[fileName].Get();
}
