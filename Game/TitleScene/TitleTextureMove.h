#pragma once
#include"StepTimer.h"

class TitleTextureMove
{
public:
	TitleTextureMove();
	~TitleTextureMove();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <returns>���̃V�[��</returns>
	void  Update(const DX::StepTimer& timer);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();


private:
	


};