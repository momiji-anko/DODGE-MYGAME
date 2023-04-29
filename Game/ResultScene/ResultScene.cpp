/*
�^�C�g���V�[��
*/

#include "pch.h"

#include <WICTextureLoader.h>
#include <SimpleMath.h>

#include "../GameMain.h"
#include "DeviceResources.h"

#include "ResultScene.h"
#include<string>
#include"Game/PlayScene/AliveTimer.h"
#include"Libraries/MyLibraries/TextureManager.h"
#include"Game/PlayScene/GameContext/GameContext.h"

//�J�����̉�]���x
const float ResultScene::CAMERA_ROT_SPEED = 0.001f;
//�A���t�@�̍ő�l
const float ResultScene::ALPHA_MAX_NUM = 1.0f;
//�A���t�@�̍ŏ��l
const float ResultScene::ALPHA_MIN_NUM = 0.0f;

/*--------------------------------------------------
�R���X�g���N�^
--------------------------------------------------*/
ResultScene::ResultScene()
	:
	m_pAdx2(nullptr),
	m_musicID(0),
	m_commonState(nullptr),
	m_spriteBatch(nullptr),
	m_spriteFont(nullptr),
	m_fadeInOut(nullptr),
	m_numTexture{},
	m_pushTexture{},
	m_rankTexture{},
	m_aliveTimeTexture{},
	m_resultTexture{},
	m_rankStringTexture{},
	m_alpha(1.0f),
	m_alphaSpeed(-0.01f),
	m_aliveTime(0.0f),
	m_rank(RANK::NONE),
	m_stageManeger(nullptr),
	m_cameraRot(0.0f),
	m_stageNum(StageManager::StageSelect::Stage1)
{
}

/*--------------------------------------------------
�f�X�g���N�^
--------------------------------------------------*/
ResultScene::~ResultScene()
{
	//Adx�̏I������
	m_pAdx2->Finalize();
}

/*--------------------------------------------------
������
--------------------------------------------------*/
void ResultScene::Initialize()
{
	//Adx�擾
	m_pAdx2 = &ADX2::GetInstance();
	//Adx�̏�����
	m_pAdx2->Initialize("Resources/Sounds/DODGESound.acf",
		"Resources/Sounds/CueSheet_0.acb");
	//BGM�𗬂�
	m_musicID = m_pAdx2->Play(CRI_CUESHEET_0_RESULT);

	//�A���C�u�^�C���̎擾
	AliveTimer* aliveTime = &AliveTimer::GetInstance();
	//�^�C���̎擾
	m_aliveTime = aliveTime->GetTimer();

	//�^�C���ɂ���ă����N�����߂�
	m_rank = RankDecide(m_aliveTime);


	//�X�e�[�W�}�l�[�W���[�̍쐬
	m_stageManeger = std::make_unique<StageManager>();
	//������
	m_stageManeger->Initialize( m_stageNum);
	// �t�F�[�h�̍쐬
	m_fadeInOut = std::make_unique<Fade>();
	m_fadeInOut->Create();
	// ������
	m_fadeInOut->Initialize(DirectX::SimpleMath::Vector3::Zero);
	// �t�F�[�h�C���J�n
	m_fadeInOut->FadeIn();
	GameContext::GetInstance().SetCommonState(m_commonState.get());
}

/*--------------------------------------------------
�X�V
�߂�l	:���̃V�[���ԍ�
--------------------------------------------------*/
GAME_SCENE ResultScene::Update(const DX::StepTimer& timer)
{
	// �L�[���͏����擾����
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	//�J�����̉�]
	m_cameraRot += CAMERA_ROT_SPEED;

	//�A���t�@���A���t�@�X�s�[�h�ő���
	m_alpha += m_alphaSpeed;

	//�A���t�@���O���P�ɂȂ�����A���t�@�X�s�[�h�𔽓]������
	if (m_alpha < ALPHA_MIN_NUM || m_alpha > ALPHA_MAX_NUM)
	{
		//�A���t�@�X�s�[�h�𔽓]������
		m_alphaSpeed *= -1.0f;
	}


	//�t�F�[�h�C�����I�������ԂŃX�y�[�X�L�[����������t�F�[�h�A�E�g���^�C�g���ɑJ��
	if (keyState.Space && m_fadeInOut->ISOpen())
	{
		//�t�F�[�h�A�E�g�J�n
		m_fadeInOut->FadeOut();
		m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);
	}

	//�t�F�[�h�X�V
	m_fadeInOut->Update(timer);

	//�t�F�[�h�A�E�g���I�������^�C�g���ɑJ��
	if (m_fadeInOut->ISClose())
	{
		return GAME_SCENE::TITLE;
	}


	return GAME_SCENE::NONE;
}

/*--------------------------------------------------
�`��
--------------------------------------------------*/
void ResultScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//	�E�B���h�E�T�C�Y�̎擾
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	// �r���[�s��
	DirectX::SimpleMath::Matrix view;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix projection;
	// �J�������W
	DirectX::SimpleMath::Vector3 eye = { cos(m_cameraRot) * 18,10,-sin(m_cameraRot) * 18 };
	// �����_
	DirectX::SimpleMath::Vector3 target = DirectX::SimpleMath::Vector3::Zero;
	// ������x�N�g��
	DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::UnitY;
	//�r���[�s��v�Z
	view = DirectX::SimpleMath::Matrix::CreateLookAt(eye, target, up);
	// �E�B���h�E�T�C�Y�̎擾
	RECT size = pDR->GetOutputSize();
	//�A�X�y�N�g��
	float aspectRatio = float(size.right) / float(size.bottom);

	//�J�����̌�����͈͂̐ݒ�
	float nearPlane = 1.0f;
	float farPlane = 100.0f;
	float fov = 45.0f;
	//����p�����W�A���Ŏ擾
	float fieldOfView = DirectX::XMConvertToRadians(fov);
	//�v���W�F�N�V�����s��v�Z
	projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
	//�J�����̍쐬
	Camera camera;
	//�v�Z�����v���W�F�N�V�����s���ݒ肷��
	camera.SetProjectionMatrix(projection);
	//�v�Z�����r���[�s���ݒ肷��
	camera.SetViewMatrix(view);

	//�w�i�̃X�e�[�W�`��
	RenderStage(&camera);


	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());


	//���U���g�����̕\�����W
	DirectX::SimpleMath::Vector2 resultPos{ width / 2 , 100};
	//���U���g�����̕\��
	m_spriteBatch->Draw(m_resultTexture.Get(), resultPos, nullptr, DirectX::Colors::White, 0.0f, { 129.0f,44.0f }, 2.0f, DirectX::SpriteEffects_None, 0);

	//�^�C���̕\��
	DrawAliveTime();

	//�����N��؂���ʒu
	RECT rectRank[] = {
	{450,0,600  ,150},//D
	{300,0,450	,150},//C	
	{150,0,300	,150},//B	
	{0	,0,150	,150},//A	
	{600,0,750  ,150},//S	
	};


	//�����N�����̕\�����W
	DirectX::SimpleMath::Vector2 rankStrPos{ width / 3 -30, height / 4  + 37.0f};
	//�����N�����摜�̒��S���W
	DirectX::SimpleMath::Vector2 rankStrCenterPos{ 97.0f, 37.0f };
	//�����N�̕\�����W
	DirectX::SimpleMath::Vector2 rankPos{ width / 2.0f + 60.0f,height / 4.0f };

	//�����N�����̃X�P�[��
	float rankStrTexScale = 2.0f;

	//�����N�����̕\��
	m_spriteBatch->Draw(m_rankStringTexture.Get(), rankStrPos + rankStrCenterPos, nullptr, DirectX::Colors::White, 0.0f, rankStrCenterPos, rankStrTexScale);
	//�����N�̕\��
	m_spriteBatch->Draw(m_rankTexture.Get(), rankPos, &rectRank[static_cast<int>(m_rank)], DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero);

	//PUSH_SPASE_KEY�̕\�����W
	DirectX::SimpleMath::Vector2 pushSpeseKeyPos = DirectX::SimpleMath::Vector2::Zero;
	//PUSH_SPASE_KEY�̐F
	DirectX::SimpleMath::Vector4 pushColor{ 1.0f,1.0f,1.0f,m_alpha };
	//PUSH_SPASE_KEY�̕\��
	m_spriteBatch->Draw(m_pushTexture.Get(), pushSpeseKeyPos, nullptr, pushColor, 0.0f, DirectX::SimpleMath::Vector2::Zero);


	m_spriteBatch->End();

	//�t�F�[�h�̕`��
	m_fadeInOut->Render();

}

/*--------------------------------------------------
�I������
--------------------------------------------------*/
void ResultScene::Finalize()
{
}
/*--------------------------------------------------
���\�[�X�̓ǂݍ���
--------------------------------------------------*/
void ResultScene::LoadResources()
{
	
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3D�f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// �R�����X�e�[�g::D3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// �X�v���C�g�o�b�`::�f�o�b�O���̕\���ɕK�v
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	TextureManager& textureManager = TextureManager::GetInstance();
	
	// �e�N�X�`���̓ǂݍ���
	// ����
	m_numTexture =textureManager.LoadTexture(L"Resources/Textures/num.png");
	// PUSH_SPASE_KEY
	m_pushTexture = textureManager.LoadTexture(L"Resources/Textures/PushSpaceKey.png");
	// �����N
	m_rankTexture = textureManager.LoadTexture(L"Resources/Textures/rank.png");
	// �A���C�u�^�C��
	m_aliveTimeTexture = textureManager.LoadTexture(L"Resources/Textures/time.png");
	// ���U���g����
	m_resultTexture = textureManager.LoadTexture(L"Resources/Textures/result.png");
	// �����N����
	m_rankStringTexture = textureManager.LoadTexture(L"Resources/Textures/rankString.png");

}

ResultScene::RANK ResultScene::RankDecide(float aliveTime_s)
{
	//�����ND�̃{�[�_�[���C���^�C��
	int rankDBorderLineTime_s = 35;
	//�����NC�̃{�[�_�[���C���^�C��
	int rankCBorderLineTime_s = 60;
	//�����NB�̃{�[�_�[���C���^�C��
	int rankBBorderLineTime_s = 90;
	//�����NA�̃{�[�_�[���C���^�C��
	int rankABorderLineTime_s = 150;

	//�����N
	ResultScene::RANK rank = RANK::NONE;

	//�^�C���ɂ���ă����N��ς���
	//�R�T�ȉ���D
	if (aliveTime_s <= rankDBorderLineTime_s)
	{
		rank = RANK::D;
	}
	//�R�U�ȏ�U�O�ȉ���C
	else if (aliveTime_s <= rankCBorderLineTime_s)
	{
		rank = RANK::C;
	}
	//�U�P�ȏ�X�O�ȉ���B
	else if (aliveTime_s <= rankBBorderLineTime_s)
	{
		rank = RANK::B;
	}
	//�X�P�ȏ�P�T�O�ȉ���A
	else if (aliveTime_s <= rankABorderLineTime_s)
	{
		rank = RANK::A;
	}
	//�P�T�P�ȏ��B
	else
	{
		rank = RANK::S;
	}

	//�����N��Ԃ�
	return rank;
}

void ResultScene::RenderStage(Camera* camera)
{
	m_stageManeger->Draw(camera);
	
}

/// <summary>
/// �A���C�u�^�C���̕\��
/// </summary>
void ResultScene::DrawAliveTime()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//	�E�B���h�E�T�C�Y�̎擾
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	//�A���C�u�^�C���\������
	//��̈ʂ̍��W
	DirectX::SimpleMath::Vector2 onePos{ width / 2 +150 , height / 2 };
	//�\�̈ʂ̍��W
	DirectX::SimpleMath::Vector2 tenPos{ onePos.x - 85, onePos.y };
	//�S�̈ʂ̍��W
	DirectX::SimpleMath::Vector2 hundredPos{ onePos.x - 170, onePos.y };
	//�A���C�u�^�C���̍��W
	DirectX::SimpleMath::Vector2 timePos{ width / 3 - 50,height / 2 + 20 };

	//�����̐؂���ʒu
	RECT rectNum[10] = {
	{0	,0,52 ,64},//0
	{52	,0,97 ,64},//1
	{96	,0,140,64},//2
	{140,0,190,64},//3
	{190,0,243,64},//4
	{243,0,293,64},//5
	{293,0,341,64},//6
	{341,0,388,64},//7
	{388,0,436,64},//8
	{436,0,483,64} //9
	};

	//�摜�̃X�P�[��
	float texScale = 2.0f;

	//aliveTimeTexture�̒��S���W
	DirectX::SimpleMath::Vector2 aliveTimeTextureCenterPos{ 83.0f, 38.0f };

	//��̈ʂ̐����̌v�Z
	int oneNum = static_cast<int>(m_aliveTime) % 10;
	//�\�̈ʂ̐����̌v�Z
	int tenNum = static_cast<int>(m_aliveTime / 10) % 10;
	//�S�̈ʂ̐����̌v�Z
	int hundredNum = static_cast<int>(m_aliveTime / 100) % 10;

	//�A���C�u�^�C���̕\��
	m_spriteBatch->Draw(m_aliveTimeTexture.Get(), timePos + aliveTimeTextureCenterPos, nullptr, DirectX::Colors::White, 0.0f, aliveTimeTextureCenterPos, texScale, DirectX::SpriteEffects_None, 0);
	//��̈ʂ̕\��
	m_spriteBatch->Draw(m_numTexture.Get(), onePos, &rectNum[oneNum], DirectX::Colors::White, 0.0f, { 0.0f,0.0f }, texScale, DirectX::SpriteEffects_None, 0);
	//�\�̈ʂ̕\��
	m_spriteBatch->Draw(m_numTexture.Get(), tenPos, &rectNum[tenNum], DirectX::Colors::White, 0.0f, { 0.0f,0.0f }, texScale, DirectX::SpriteEffects_None, 0);
	//�S�̈ʂ̕\��
	m_spriteBatch->Draw(m_numTexture.Get(), hundredPos, &rectNum[hundredNum], DirectX::Colors::White, 0.0f, { 0.0f,0.0f }, texScale, DirectX::SpriteEffects_None, 0);
	

}
