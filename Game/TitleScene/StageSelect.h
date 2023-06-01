#pragma once
#include"StepTimer.h"
#include<SimpleMath.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>
#include<Keyboard.h>
#include "../IScene.h"
#include "Game/GameMain.h"

#include"Libraries/AXD2/Adx2.h"
#include"../../Helpers/CueSheet_0.h"
#include"../../Helpers/DODGESound_acf.h"
#include"../PlayScene/Stage/Stage.h"
#include"Game/PlayScene/Stage/StageManager.h"

class StageSelect
{
public:
	void SetStageManager(StageManager* stageManager) { m_stageManager = stageManager; }

	StageSelect() = default;
	~StageSelect() = default;

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

	static const float MOVE_TIME;

	DirectX::SimpleMath::Vector2 m_nextPosition;
	
	DirectX::SimpleMath::Vector2 m_prePosition;

	std::wstring m_stageTextTextureFileName;
	
	std::wstring m_stageNumTextureFileName;

	std::wstring m_stageArrowTextureFileName;

	DirectX::SimpleMath::Vector2 m_nowPosition;

	std::vector<float> m_stageTextureScales;

	int m_selectStageNum;
	
	StageManager* m_stageManager;
};