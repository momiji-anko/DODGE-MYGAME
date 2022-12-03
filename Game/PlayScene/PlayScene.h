/*
�v���C�V�[��
*/
#pragma once

#include <CommonStates.h>
#include <Model.h>
#include <GeometricPrimitive.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

#include "../IScene.h"
#include "Game/GameMain.h"
#include"Player/Player.h"
#include"Actor/Actor.h"
#include"Obstacle/ObstacleManeger.h"
#include"Stage/Stage.h"
#include"Item/ItemManeger.h"
#include"ShadowMap.h"
#include"Stage/StageManager.h"


#include"Libraries/AXD2/Adx2.h"
#include"../../Helpers/CueSheet_0.h"
#include"../../Helpers/DODGESound_acf.h"
#include"Objects.h"

class GridFloor;
class Camera;

class PlayScene : public IScene
{
private:
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>           m_commonState;
	std::unique_ptr<DirectX::Model>                  m_pModel;
	std::unique_ptr<DirectX::Model>                  m_pTenModel;

	
	std::unique_ptr<Player>                          m_actor;
	std::unique_ptr<ObstacleManeger>                 m_obstacleManeger;
	std::unique_ptr<ItemManeger>                     m_itemManeger;
	

	DebugCamera*                                     m_pDebugCamera;
	GridFloor*                                       m_pGridFloor;


	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch>            m_spriteBatch;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont>             m_spriteFont;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_numTexture;
	float                                            m_waitTime;


	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_blackTexture;
	float                                            m_fade;
	bool                                             m_flag;
	bool                                             m_flagFadeIn;
	float                                            m_timer;


	std::unique_ptr<ShadowMap>                       m_shadowMap;
	


	ADX2*                                            m_pAdx2;
	int                                              m_musicID;


	std::unique_ptr<StageManeger>                    m_stageManeger;

	std::unique_ptr<DirectX::Model>                  m_model[2];
	
	bool                                             m_stratFlag;
public:

	// �R���X�g���N�^
	PlayScene();

	// �f�X�g���N�^
	~PlayScene();

	// ������
	void Initialize() override;

	// �X�V
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

	// ���\�[�X�̓ǂݍ���
	void LoadResources() override;

	float GetTime() { return m_timer; }

	std::unique_ptr<Player>& GetPlayer() { return std::move(m_actor); }
	std::unique_ptr<ObstacleManeger>& GetObstacleManeger() { return std::move(m_obstacleManeger); }
	std::unique_ptr<StageManeger>& GetStageManeger() { return std::move(m_stageManeger); }
	std::unique_ptr<ItemManeger>& GetItemManeger() { return std::move(m_itemManeger); }


};