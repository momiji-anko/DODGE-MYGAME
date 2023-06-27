/*
* 2023/04/28
* GameContext.h
* �Q�[���R���e�L�X�g
* ������
*/
#include"Libraries/MyLibraries/Singleton.h"
#include<CommonStates.h>
#include<SpriteBatch.h>
#include<Keyboard.h>


/// <summary>
/// �Q�[���R���e�L�X�g
/// </summary>
class GameContext:public Singleton<GameContext>
{
public:
	/// <summary>
	/// �R�����X�e�[�g�̐ݒ�
	/// </summary>
	/// <param name="commonState">�R�����X�e�[�g�̐��|�C���^</param>
	void SetCommonState(DirectX::CommonStates* commonState) { m_commonState = commonState; }
	/// <summary>
	/// �R�����X�e�[�g�̎擾
	/// </summary>
	/// <returns>�R�����X�e�[�g�̐��|�C���^</returns>
	DirectX::CommonStates* GetCommonState()const { return m_commonState; }

	/// <summary>
	/// �X�v���C�g�o�b�`�̐ݒ�
	/// </summary>
	/// <param name="spriteBatcth">�X�v���C�g�o�b�`�̐��|�C���^</param>
	void SetSpriteBatcth(DirectX::SpriteBatch* spriteBatcth) { m_spriteBatcth = spriteBatcth;}
	/// <summary>
	/// �X�v���C�g�o�b�`�̎擾
	/// </summary>
	/// <returns>�X�v���C�g�o�b�`�̐��|�C���^</returns>
	DirectX::SpriteBatch* GetSpriteBatcth()const { return m_spriteBatcth; }
	
	/// <summary>
	/// �L�[�{�[�h�X�e�[�g�g���b�J�[�̐ݒ�
	/// </summary>
	/// <param name="keyboad">�L�[�{�[�h�X�e�[�g�g���b�J�[</param>
	void SetKeyboardStateTracker(DirectX::Keyboard::KeyboardStateTracker* keyboad) { m_keyBoardStateTracker = keyboad; }
	/// <summary>
	/// �L�[�{�[�h�X�e�[�g�g���b�J�[�̎擾
	/// </summary>
	/// <returns>�L�[�{�[�h�X�e�[�g�g���b�J�[</returns>
	DirectX::Keyboard::KeyboardStateTracker* GetKeyboardStateTracker() const{ return m_keyBoardStateTracker; }
		
	/// <summary>
	/// �v���C���[�����S���Ă��邩�擾
	/// </summary>
	/// <returns>true = ���S���Ă�����Afalse = ���S���Ă��Ȃ�</returns>
	bool GetIsPlayerDeath() { return m_isPlayerDeath; }

	/// <summary>
	/// �v���C���[�����S���Ă��邩�ݒ�
	/// </summary>
	/// <param name="flag">true = ���S���Ă�����Afalse = ���S���Ă��Ȃ�</param>
	void SetIsPlayerDeath(bool flag) { m_isPlayerDeath = flag; }


public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	GameContext();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameContext();

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();


private:
	//�R�����X�e�[�g
	DirectX::CommonStates* m_commonState;
	//�X�v���C�g�o�b�`
	DirectX::SpriteBatch* m_spriteBatcth;
	//�L�[�{�[�h�X�e�[�g�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker* m_keyBoardStateTracker;
	//�v���C�V�[���X�g�b�v
	bool m_isPlayerDeath;
};
