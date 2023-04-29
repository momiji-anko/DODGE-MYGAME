/*
* 2023/04/26
* StageManager.h
* �X�e�[�W�}�l�[�W���[
* �����@��
*/
#pragma once
#include"Stage.h"
#include"Game/GameMain.h"
#include <CommonStates.h>
#include<SimpleMath.h>
#include<Model.h>
#include <string>

#include"../IBehavior.h"

#include<GeometricPrimitive.h>

#include"Libraries/Json/json.hpp"


/// <summary>
/// �X�e�[�W�}�l�[�W���[
/// </summary>
class StageManager
{
public:
	//�O���錾
	enum class StageSelect;

	/// <summary>
	/// �X�e�[�W�ɃV���h�E�}�b�v��n��
	/// </summary>
	/// <param name="shadow">�V���h�E�}�b�v�̐��|�C���^</param>
	void SetShadowMap(ShadowMap* shadow);
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	StageManager();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~StageManager();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="stage">�X�e�[�W�I��</param>
	void Initialize( StageSelect stage = StageSelect::Stage1);

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
	/// �X�e�[�W�ƃA�N�^�[�̓����蔻��
	/// </summary>
	/// <param name="actor">�A�N�^�[</param>
	/// <returns>true=�������Ă���@false=�����Ă��Ȃ�</returns>
	bool StageToActorHitCheck(Actor* actor);


	/// <summary>
	/// �����Ɣ|���S���̓����蔻��
	/// </summary>
	/// <param name="vertices">���_���W</param>
	/// <param name="linePos">�����̗��[���W</param>
	/// <param name="normalVector">�@���x�N�g���̃|�C���^</param>
	/// <returns>true=�������Ă���@false=�����Ă��Ȃ�</returns>
	bool StageHitCheck(std::vector<DirectX::SimpleMath::Vector3> vertices, std::vector<DirectX::SimpleMath::Vector3> linePos, DirectX::SimpleMath::Vector3* normalVector);
private:

	/// <summary>
	/// �r�w�C�r�A�[�쐬
	/// </summary>
	void CreateBehavior();


	/// <summary>
	/// �X�e�[�Wjson��ǂݍ���
	/// </summary>
	/// <param name="fileName">json�̃p�X</param>
	void LoadStageJson(const std::wstring& fileName);

	/// <summary>
	/// string��wstring�ɕϊ�����
	/// </summary>
	/// <param name="str">�}���`�o�C�g������</param>
	/// <returns>���C�h������</returns>
	std::wstring ConvertWString(const std::string& str);


	/// <summary>
	/// �X�e�[�W���s�����I�����Ă��邩�m�F
	/// </summary>
	void CheckStageMoveEnd();

	/// <summary>
	/// �X�e�[�W�̃|���S���ƃA�N�^�[�̂߂荞�ݏ���
	/// </summary>
	/// <param name="actor">�A�N�^�[/param>
	/// <param name="polygonVertexPos">�|���S���̒��_���W</param>
	/// <param name="normalVec">�@���x�N�g��</param>
	void ActorPolygonPenetration(Actor* actor, std::vector<DirectX::SimpleMath::Vector3> polygonVertexPos, DirectX::SimpleMath::Vector3 normalVec);


	/// <summary>
	/// �A�N�^�[�̃X���C�h�x�N�g�����v�Z����
	/// </summary>
	/// <param name="normalVec">�|���S���̖@���x�N�g��</param>
	/// <param name="actorVel">�A�N�^�[�̈ړ���</param>
	/// <returns>�X���C�h�x�N�g��</returns>
	DirectX::SimpleMath::Vector3 SlideVecCalculation(DirectX::SimpleMath::Vector3 normalVec, DirectX::SimpleMath::Vector3 actorVel);

public:
	//�X�e�[�W�I��
	enum class StageSelect
	{
		//�X�e�[�W�P
		Stage1,
		//�X�e�[�W�Q
		Stage2,
		//�X�e�[�W�R
		Stage3
	};

private:
	//�X�e�[�W�z��
	std::vector<std::unique_ptr<Stage>>         m_stage;
	//�X�e�[�W�r�w�C�r�A�[
	std::vector<std::unique_ptr<IBehavior>>     m_behavior;


};