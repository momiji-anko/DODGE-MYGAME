/*
* 2023/04/10
* ModelManager.h 
* ���f���}�l�[�W���[
* �����@��
*/
#pragma once 
#include"Singleton.h"
#include <unordered_map>
#include<Model.h>

/// <summary>
/// ���f���}�l�[�W���[
/// </summary>
class ModelManager : public Singleton<ModelManager>
{
private:
	//���f���̔z��
	std::unordered_map<std::wstring, std::unique_ptr<DirectX::Model>> m_models;

public:
	/// <summary>
	/// ���f���ǂݍ���
	/// </summary>
	/// <param name="fileName">���f���̃t�@�C���p�X</param>
	/// <returns>���f���̐��|�C���^</returns>
	DirectX::Model* LoadModel(std::wstring fileName);

};