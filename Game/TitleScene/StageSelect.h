/*
* 2023/06/12
* StageSelect.h
* �X�e�[�W�Z���N�g
* �����@��
*/
#pragma once
#include"StepTimer.h"
#include<SimpleMath.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>
#include<Keyboard.h>
#include "../IScene.h"
#include "Game/GameMain.h"

#include"Libraries/ADX2/Adx2.h"
#include"../../Helpers/CueSheet_0.h"
#include"../../Helpers/DODGESound_acf.h"
#include"../PlayScene/Stage/Stage.h"
#include"Game/PlayScene/Stage/StageManager.h"

/// <summary>
/// �X�e�[�W�Z���N�g
/// </summary>
class StageSelect
{
public:

	/// <summary>
	/// �I�������X�e�[�W�ԍ��̎擾
	/// </summary>
	/// <returns>�I�������X�e�[�W�ԍ�</returns>
	int  GetSelectStageNum() { return m_selectStageNum; }

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="stageManager">�X�e�[�W�}�l�[�W���[�̐��|�C���^</param>
	StageSelect(StageManager* stageManager);
	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~StageSelect();

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
	//�X�e�[�W�e�L�X�g�e�N�X�`���̃t�@�C���p�X
	std::wstring m_stageTextTextureFilePath;
	//�X�e�[�W�ԍ��e�N�X�`���̃t�@�C���p�X
	std::wstring m_stageNumTextureFilePath;
	//�X�e�[�W���e�N�X�`���̃t�@�C���p�X
	std::wstring m_stageArrowTextureFilePath;

	//���W
	DirectX::SimpleMath::Vector2 m_texturePosition;
	
	//�I�������X�e�[�W�̔ԍ�
	int m_selectStageNum;
	//�X�e�[�W�̍ő吔
	int m_stageMaxNum;

	//�X�e�[�W�}�l�[�W���[
	StageManager* m_stageManager;

	//�X�e�[�W�Z���N�g�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stageTextTexture;
	
	//�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stageNumTexture;
	
	//�X�e�[�W�Z���N�g�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stageArrowTexture;

};