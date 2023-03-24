/*
* 2023/03/23
* Blink.h
* �����@��
*/
#pragma once
#include"StepTimer.h"

class Blink
{
private:
	//�u�����N�^�C��
	float m_blinkTime_s;
	//�_�ł��鑬��
	float m_blinkSpeed;

	//�_�ő��x�����񂾂񑁂����邩
	bool m_isGettingFaster;

	//�_�ł��邩�ǂ���
	bool m_isBlink;

public:
	/// <summary>
	/// �_�ł��邩�ݒ�
	/// </summary>
	/// <param name="flag">true = �_�ł���@false = �_�ł��Ȃ�</param>
	void SetBlinkFlag(bool flag) { m_isBlink = flag; }
	
	/// <summary>
	/// �_�ŏ�Ԏ擾
	/// </summary>
	/// <returns>true = �_�ł��Ă���@false = �_�ł��Ă��Ȃ�</returns>
	bool IsBlink() { return m_isBlink; }

	
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Blink();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Blink();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="blinkTime">�_�ł��鎞�ԁi�b�w��j</param>
	/// <param name="blinkSpeed">�_�ł��鑬��</param>
	/// <param name="isGetiingFaster">�_�ł����񂾂񑁂����邩</param>
	void Initialize(float blinkTime ,float blinkSpeed = 0.5f ,bool isGetiingFaster = false);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer"></param>
	void Update(const DX::StepTimer& timer);

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void ReSet();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();


};
