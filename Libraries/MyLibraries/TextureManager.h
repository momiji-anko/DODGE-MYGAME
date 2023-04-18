/*
* 2023/04/10
* TextureManager.h
* �e�N�X�`����ǂݍ��݁��Ǘ�����N���X
* �����@��
*/
#pragma once 
#include"Singleton.h"
#include<WICTextureLoader.h>
#include<unordered_map>

/// <summary>
/// �e�N�X�`���}�l�[�W���[
/// </summary>
class TextureManager :public Singleton<TextureManager>
{
private:
	//�e�N�X�`��
	std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;
public:
	/// <summary>
	/// �e�N�X�`����ǂݍ���
	/// </summary>
	/// <param name="fileName">�e�N�X�`���̃t�@�C���p�X</param>
	/// <returns>�e�N�X�`��</returns>
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> LoadTexture(std::wstring fileName);
};