#pragma once
#include"Player.h"
#include"Game/PlayScene/Obstacle/ObstacleManager.h"
#include"../Item/ItemManager.h"
#include"../Stage/StageManager.h"


/// <summary>
/// �v���C���[�}�l�[�W���[
/// </summary>
class PlayerManager
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="playerMode">�v���C���[���[�h</param>
	/// <param name="stageManager">�X�e�[�W�}�l�[�W���[</param>
	/// <param name="itemManager">�A�C�e���}�l�[�W���[</param>
	/// <param name="obstacleManager">��Q���}�l�[�W���[</param>
	PlayerManager(GameMain::PlayerMode playerMode,StageManager* stageManager, ItemManager* itemManager, ObstacleManager* obstacleManager);
	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PlayerManager();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	void Update(const DX::StepTimer& timer);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="camera">�J�����̐��|�C���^</param>
	void Draw(Camera* camera);

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	/// <summary>
	/// �P�����Q���̂ǂ��炩�̍��W���擾
	/// </summary>
	/// <returns>�v���C���[���W</returns>
	DirectX::SimpleMath::Vector3 GetRandomPlayerPosition();

	/// <summary>
	/// �v���C���[���S�����S���Ă���m�F
	/// </summary>
	/// <returns>true=�S�����S,false=�S�����S���Ă��Ȃ�</returns>
	bool GetPlayerIsAllDaed();
private:
	/// <summary>
	/// �v���C���[�쐬
	/// </summary>
	void CreatePlayer();


	/// <summary>
	/// �v���C���[�̂߂荞�ݏ���
	/// </summary>
	/// <param name="player">�v���C���[</param>
	/// <param name="cupseleToCupseVector">���������J�v�Z���ƃJ�v�Z���̃x�N�g���i�v���C���[�J�v�Z�������Q���J�v�Z���̃x�N�g���j</param>
	/// <param name="playerCapsleRadius">�v���C���[�̃J�v�Z���̔��a</param>
	/// <param name="obstacleCupsleRadius">�v���C���[�Q�̃J�v�Z���̔��a</param>
	void PlayerCapuslePenetration(Actor* player, const DirectX::SimpleMath::Vector3& cupseleToCupseVector, float playerCapsleRadius, float obstacleCupsleRadius);

private:
	//�v���C���[
	std::vector<std::unique_ptr<Player>> m_players;

	//�v���C���[���[�h
	GameMain::PlayerMode m_playerMode;
	
	//�X�e�[�W�}�l�[�W���[
	StageManager*    m_stageManager;
	//�A�C�e���}�l�[�W���[
	ItemManager*     m_itemManager;
	//��Q���}�l�[�W���[
	ObstacleManager* m_obstacleManager;

};