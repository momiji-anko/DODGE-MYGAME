/*
* 2023/04/11
* RotationCubeBehavior.h
* ��]����L���[�u
* �����@��
*/
#include"Game/PlayScene/IBehavior.h"


/// <summary>
/// ��]����L���[�u�̓���
/// </summary>
class RotationCubeBehavior : public IBehavior
{
public:
	RotationCubeBehavior() = default;
	~RotationCubeBehavior()override = default;

	/// <summary>
	/// ��]����L���[�u�̓���
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <param name="actor">�X�e�[�W�̃|�C���^�[</param>
	void Execute(const DX::StepTimer& timer, Actor* actor) override;
};