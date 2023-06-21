/*
* 2023/06/12
* ModeSelect.h
* ���[�h�Z���N�g
* �����@��
*/
#pragma once
#include "Game/GameMain.h"
#include<string>
#include<vector>

/// <summary>
/// ���[�h�Z���N�g
/// </summary>
class ModeSelect
{
public:
	/// <summary>
	/// �v���C���[���[�h�̎擾
	/// </summary>
	/// <returns>�v���C���[���[�h</returns>
	GameMain::PlayerMode GetPlayerMode() { return m_playerMode; }
	
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ModeSelect();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ModeSelect();


	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <returns>���̑I����ʂɍs����</returns>
	bool  Update(const DX::StepTimer& timer);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();
private:
	//���[�h�t�@�C���p�X
	std::vector< std::wstring> m_modeFilePaths;
	
	//�I�������v���C���[���[�h
	GameMain::PlayerMode m_playerMode;

	//���[�h�Z���N�g�e�N�X�`��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_modeSelectTextures;

};