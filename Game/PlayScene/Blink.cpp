/*
* 2023/03/23
* Blink.cpp
* �����@��
* �_�ł���
*/
#include"pch.h"
#include"Blink.h"
#include"DeviceResources.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
Blink::Blink()
	:
	m_blinkTime_s(0),
	m_blinkSpeed(0),
	m_isBlink(false),
	m_isGettingFaster(false),
	m_startBlinkSpeed(0),
	m_startBlinkTime_s(0),
	m_blinkCount(0),
	m_startBlinkCount(0),
	m_blinkTimeFast(0),
	m_blinkedCount(0),
	m_isDraw(false)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Blink::~Blink()
{

}

/// <summary>
/// ������
/// </summary>
/// <param name="blinkTime">�_�ł��鎞�ԁi�b�w��j</param>
/// <param name="blinkCount">�_�ł����</param>
/// <param name="isGetiingFaster">�_�ł����񂾂񑁂����邩</param>
/// <param name="blinkSpeed">�_�ł��鑬��</param>
void Blink::Initialize(float blinkTime, int blinkCount, bool isGetiingFaster, float blinkSpeed )
{
	//�p�����[�^������
	
	m_isGettingFaster = isGetiingFaster;
	
	//�����͓_�ł��Ă��Ȃ���Ԃɂ���
	m_isBlink = false;
	
	//�u�����N�^�C��
	m_startBlinkTime_s = blinkTime;
	m_blinkTime_s = m_startBlinkTime_s;
	m_blinkTimeFast = m_startBlinkTime_s;
	
	//�u�����N�X�s�[�h
	m_startBlinkSpeed = blinkSpeed;
	m_blinkSpeed = m_startBlinkSpeed;
	
	//�u�����N��
	m_startBlinkCount = blinkCount;
	m_blinkCount = m_startBlinkCount;

	//�\��
	m_blinkedCount = 0;
	m_isDraw = true;

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void Blink::Update(const DX::StepTimer& timer)
{
	//�o�ߎ���
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	//m_isBlink��true & �_�ŉ񐔂��O��荂����Γ_�ł���
	if (m_isBlink && m_blinkCount > 0)
	{
		//�u�����N�^�C�����o�ߎ��Ԃň���
		m_blinkTime_s -= elapsedTime;

		//�u�����N�^�C�����O�ȉ��ɂȂ�����_�ŏ�Ԃ̕ύX
		if (m_blinkTime_s <= 0)
		{
			//�_�ŏ�Ԃ̕ύX
			m_isDraw = !m_isDraw;

			//�_�ł��Ă����u�Ԃ̏���
			BlinkedCheck();
			
		}
	}
	else
	{
		//�_�ł��Ă��Ȃ��̂ł����ReSet�������Ă���
		ReSet();
	}

}

/// <summary>
/// ���Z�b�g
/// </summary>
void Blink::ReSet()
{
	//�p�����[�^������

	//�����͓_�ł��Ă��Ȃ���Ԃɂ���
	m_isBlink = false;
	
	//�u�����N�^�C��
	m_blinkTime_s = m_startBlinkTime_s;
	m_blinkTimeFast = m_startBlinkTime_s;

	//�u�����N�X�s�[�h
	m_blinkSpeed = m_startBlinkSpeed;
	
	//�u�����N��
	m_blinkCount = m_startBlinkCount;

	//�\��
	m_blinkedCount = 0;
	m_isDraw = true;
}

/// <summary>
/// ���Z�b�g
/// </summary>
void Blink::Finalize()
{
}

/// <summary>
/// �_�ł̂����u�Ԃ̏���
/// </summary>
void Blink::BlinkedCheck()
{
	//���Ă����Ԃ�
	if (m_isDraw)
	{
		//�_�ŉ񐔂����炷
		m_blinkCount--;
		//�_�ł����񐔂𑝂₷
		m_blinkedCount++;

		//���񂾂񑁂����邩
		if (m_isGettingFaster)
		{
			//�^�C�������炷
			m_blinkTimeFast -= m_blinkSpeed;

			//�u�����N�^�C�������񂾂񑁂�����
			m_blinkTime_s = m_blinkTimeFast;

		}
		else
		{
			//�u�����N�^�C����������
			m_blinkTime_s = m_startBlinkTime_s;
		}
	}
}
