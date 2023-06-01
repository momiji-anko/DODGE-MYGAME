/*
* 2023/05/24
* FileLoadManager.h
* �t�@�C���̓ǂݍ��݂���N���X
* �����@��
*/
#pragma once
#include<unordered_map>

/// <summary>
/// �t�@�C���̓ǂݍ��݂���N���X
/// </summary>
class FileLoadManager
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	FileLoadManager() = default;
	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~FileLoadManager() = default;

	/// <summary>
	/// �t�@�C���ǂݍ���
	/// </summary>
	/// <param name="fileName">�ǂݍ��ރt�@�C���p�X</param>
	/// <returns>�ǂݍ��񂾃t�@�C��</returns>
	std::vector<std::wstring> LoadFile(std::wstring fileName);

private:

	//�ǂݍ��񂾃t�@�C�����L���p�ϐ�
	std::unordered_map<std::wstring, std::vector<std::wstring>> m_fileNames;
	//�ǂݍ��񂾃t�@�C���̐�
	std::unordered_map< std::wstring, int > m_fileNum;

};