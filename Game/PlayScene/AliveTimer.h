/*
* 2023/04/26
* AliveTimer.h
* �A���C�u�^�C��
* �����@��
*/
#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>
#include"../../Libraries/MyLibraries/Camera.h"
#include"StepTimer.h"
#include"Libraries/MyLibraries/Singleton.h"
#include<vector>

class AliveTimer : public Singleton< AliveTimer>
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	AliveTimer();
	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~AliveTimer();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer"></param>
	void Update(const DX::StepTimer& timer);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	/// <summary>
	/// �^�C�}�[�̎擾
	/// </summary>
	/// <returns>�^�C�}�[</returns>
	float GetTimer() { return m_timer_s; }


	void SetTimerStop(bool isStop) { m_isTimerStop = isStop; }

private:
	//�^�C�}�[
	float m_timer_s;

	// �X�v���C�g�o�b�`
	DirectX::SpriteBatch* m_spriteBatch;

	//�����e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_numTexture;

	bool m_isTimerStop;

};