/*
* 2023/03/23
* Blink.h
* �����@��
*/
#pragma once
#include"StepTimer.h"

/// <summary>
/// �_��
/// </summary>
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

	//�����u�����N�^�C��
	float m_startBlinkTime_s;
	//�����u�����N�X�s�[�h
	float m_startBlinkSpeed;

	int m_blinkCount;

	int m_startBlinkCount;

	int m_blinkedCount;
	float m_blinkTimeFast;
	bool m_isDraw;
public:
	/// <summary>
	/// �_�ł��邩�ݒ�
	/// </summary>
	/// <param name="flag">true = �_�ł���@false = �_�ł��Ȃ�</param>
	void SetBlinkFlag(bool flag) { m_isBlink = flag; }
	
	/// <summary>
	/// �_�ŏ�Ԏ擾
	/// </summary>
	/// <returns>true = �����Ă��Ȃ��@false = �����Ă���</returns>
	bool IsBlink() { return m_isDraw; }

	/// <summary>
	/// �_�ŊJ�n
	/// </summary>
	void Start() { m_isBlink = true; }

	/// <summary>
	/// �_�ł��~�߂�
	/// </summary>
	void End() { m_isBlink = false; }

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
	/// <param name="blinkTime">�_�Ő؂�ւ����ԁi�b�w��j</param>
	/// <param name="blinkCount">�_�ł����</param>
	/// <param name="isGetiingFaster">�_�ł����񂾂񑁂����邩</param>
	/// <param name="blinkSpeed">�_�ł��鑬��</param>
	void Initialize(float blinkTime, int blinkCount = 10,  bool isGetiingFaster = false , float blinkSpeed = 0.5f);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	void Update(const DX::StepTimer& timer);

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void ReSet();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

private:
	/// <summary>
	/// �_�ł̂����u�Ԃ̏���
	/// </summary>
	void BlinkedCheck();
};
