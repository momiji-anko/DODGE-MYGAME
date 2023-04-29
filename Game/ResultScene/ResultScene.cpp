/*
タイトルシーン
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

//カメラの回転速度
const float ResultScene::CAMERA_ROT_SPEED = 0.001f;
//アルファの最大値
const float ResultScene::ALPHA_MAX_NUM = 1.0f;
//アルファの最小値
const float ResultScene::ALPHA_MIN_NUM = 0.0f;

/*--------------------------------------------------
コンストラクタ
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
デストラクタ
--------------------------------------------------*/
ResultScene::~ResultScene()
{
	//Adxの終了処理
	m_pAdx2->Finalize();
}

/*--------------------------------------------------
初期化
--------------------------------------------------*/
void ResultScene::Initialize()
{
	//Adx取得
	m_pAdx2 = &ADX2::GetInstance();
	//Adxの初期化
	m_pAdx2->Initialize("Resources/Sounds/DODGESound.acf",
		"Resources/Sounds/CueSheet_0.acb");
	//BGMを流す
	m_musicID = m_pAdx2->Play(CRI_CUESHEET_0_RESULT);

	//アライブタイムの取得
	AliveTimer* aliveTime = &AliveTimer::GetInstance();
	//タイムの取得
	m_aliveTime = aliveTime->GetTimer();

	//タイムによってランクを決める
	m_rank = RankDecide(m_aliveTime);


	//ステージマネージャーの作成
	m_stageManeger = std::make_unique<StageManager>();
	//初期化
	m_stageManeger->Initialize( m_stageNum);
	// フェードの作成
	m_fadeInOut = std::make_unique<Fade>();
	m_fadeInOut->Create();
	// 初期化
	m_fadeInOut->Initialize(DirectX::SimpleMath::Vector3::Zero);
	// フェードイン開始
	m_fadeInOut->FadeIn();
	GameContext::GetInstance().SetCommonState(m_commonState.get());
}

/*--------------------------------------------------
更新
戻り値	:次のシーン番号
--------------------------------------------------*/
GAME_SCENE ResultScene::Update(const DX::StepTimer& timer)
{
	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	//カメラの回転
	m_cameraRot += CAMERA_ROT_SPEED;

	//アルファをアルファスピードで足す
	m_alpha += m_alphaSpeed;

	//アルファが０か１になったらアルファスピードを反転させる
	if (m_alpha < ALPHA_MIN_NUM || m_alpha > ALPHA_MAX_NUM)
	{
		//アルファスピードを反転させる
		m_alphaSpeed *= -1.0f;
	}


	//フェードインが終わった状態でスペースキーを押したらフェードアウトしタイトルに遷移
	if (keyState.Space && m_fadeInOut->ISOpen())
	{
		//フェードアウト開始
		m_fadeInOut->FadeOut();
		m_pAdx2->Play(CRI_CUESHEET_0_BUTTON);
	}

	//フェード更新
	m_fadeInOut->Update(timer);

	//フェードアウトし終わったらタイトルに遷移
	if (m_fadeInOut->ISClose())
	{
		return GAME_SCENE::TITLE;
	}


	return GAME_SCENE::NONE;
}

/*--------------------------------------------------
描画
--------------------------------------------------*/
void ResultScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//	ウィンドウサイズの取得
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	// ビュー行列
	DirectX::SimpleMath::Matrix view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix projection;
	// カメラ座標
	DirectX::SimpleMath::Vector3 eye = { cos(m_cameraRot) * 18,10,-sin(m_cameraRot) * 18 };
	// 注視点
	DirectX::SimpleMath::Vector3 target = DirectX::SimpleMath::Vector3::Zero;
	// 上向きベクトル
	DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::UnitY;
	//ビュー行列計算
	view = DirectX::SimpleMath::Matrix::CreateLookAt(eye, target, up);
	// ウィンドウサイズの取得
	RECT size = pDR->GetOutputSize();
	//アスペクト比
	float aspectRatio = float(size.right) / float(size.bottom);

	//カメラの見える範囲の設定
	float nearPlane = 1.0f;
	float farPlane = 100.0f;
	float fov = 45.0f;
	//視野角をラジアンで取得
	float fieldOfView = DirectX::XMConvertToRadians(fov);
	//プロジェクション行列計算
	projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
	//カメラの作成
	Camera camera;
	//計算したプロジェクション行列を設定する
	camera.SetProjectionMatrix(projection);
	//計算したビュー行列を設定する
	camera.SetViewMatrix(view);

	//背景のステージ描画
	RenderStage(&camera);


	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());


	//リザルト文字の表示座標
	DirectX::SimpleMath::Vector2 resultPos{ width / 2 , 100};
	//リザルト文字の表示
	m_spriteBatch->Draw(m_resultTexture.Get(), resultPos, nullptr, DirectX::Colors::White, 0.0f, { 129.0f,44.0f }, 2.0f, DirectX::SpriteEffects_None, 0);

	//タイムの表示
	DrawAliveTime();

	//ランクを切り取り位置
	RECT rectRank[] = {
	{450,0,600  ,150},//D
	{300,0,450	,150},//C	
	{150,0,300	,150},//B	
	{0	,0,150	,150},//A	
	{600,0,750  ,150},//S	
	};


	//ランク文字の表示座標
	DirectX::SimpleMath::Vector2 rankStrPos{ width / 3 -30, height / 4  + 37.0f};
	//ランク文字画像の中心座標
	DirectX::SimpleMath::Vector2 rankStrCenterPos{ 97.0f, 37.0f };
	//ランクの表示座標
	DirectX::SimpleMath::Vector2 rankPos{ width / 2.0f + 60.0f,height / 4.0f };

	//ランク文字のスケール
	float rankStrTexScale = 2.0f;

	//ランク文字の表示
	m_spriteBatch->Draw(m_rankStringTexture.Get(), rankStrPos + rankStrCenterPos, nullptr, DirectX::Colors::White, 0.0f, rankStrCenterPos, rankStrTexScale);
	//ランクの表示
	m_spriteBatch->Draw(m_rankTexture.Get(), rankPos, &rectRank[static_cast<int>(m_rank)], DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero);

	//PUSH_SPASE_KEYの表示座標
	DirectX::SimpleMath::Vector2 pushSpeseKeyPos = DirectX::SimpleMath::Vector2::Zero;
	//PUSH_SPASE_KEYの色
	DirectX::SimpleMath::Vector4 pushColor{ 1.0f,1.0f,1.0f,m_alpha };
	//PUSH_SPASE_KEYの表示
	m_spriteBatch->Draw(m_pushTexture.Get(), pushSpeseKeyPos, nullptr, pushColor, 0.0f, DirectX::SimpleMath::Vector2::Zero);


	m_spriteBatch->End();

	//フェードの描画
	m_fadeInOut->Render();

}

/*--------------------------------------------------
終了処理
--------------------------------------------------*/
void ResultScene::Finalize()
{
}
/*--------------------------------------------------
リソースの読み込み
--------------------------------------------------*/
void ResultScene::LoadResources()
{
	
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3Dデバイスとデバイスコンテキストの取得
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// コモンステート::D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// スプライトバッチ::デバッグ情報の表示に必要
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	TextureManager& textureManager = TextureManager::GetInstance();
	
	// テクスチャの読み込み
	// 数字
	m_numTexture =textureManager.LoadTexture(L"Resources/Textures/num.png");
	// PUSH_SPASE_KEY
	m_pushTexture = textureManager.LoadTexture(L"Resources/Textures/PushSpaceKey.png");
	// ランク
	m_rankTexture = textureManager.LoadTexture(L"Resources/Textures/rank.png");
	// アライブタイム
	m_aliveTimeTexture = textureManager.LoadTexture(L"Resources/Textures/time.png");
	// リザルト文字
	m_resultTexture = textureManager.LoadTexture(L"Resources/Textures/result.png");
	// ランク文字
	m_rankStringTexture = textureManager.LoadTexture(L"Resources/Textures/rankString.png");

}

ResultScene::RANK ResultScene::RankDecide(float aliveTime_s)
{
	//ランクDのボーダーラインタイム
	int rankDBorderLineTime_s = 35;
	//ランクCのボーダーラインタイム
	int rankCBorderLineTime_s = 60;
	//ランクBのボーダーラインタイム
	int rankBBorderLineTime_s = 90;
	//ランクAのボーダーラインタイム
	int rankABorderLineTime_s = 150;

	//ランク
	ResultScene::RANK rank = RANK::NONE;

	//タイムによってランクを変える
	//３５以下はD
	if (aliveTime_s <= rankDBorderLineTime_s)
	{
		rank = RANK::D;
	}
	//３６以上６０以下はC
	else if (aliveTime_s <= rankCBorderLineTime_s)
	{
		rank = RANK::C;
	}
	//６１以上９０以下はB
	else if (aliveTime_s <= rankBBorderLineTime_s)
	{
		rank = RANK::B;
	}
	//９１以上１５０以下はA
	else if (aliveTime_s <= rankABorderLineTime_s)
	{
		rank = RANK::A;
	}
	//１５１以上はB
	else
	{
		rank = RANK::S;
	}

	//ランクを返す
	return rank;
}

void ResultScene::RenderStage(Camera* camera)
{
	m_stageManeger->Draw(camera);
	
}

/// <summary>
/// アライブタイムの表示
/// </summary>
void ResultScene::DrawAliveTime()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//	ウィンドウサイズの取得
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	//アライブタイム表示する
	//一の位の座標
	DirectX::SimpleMath::Vector2 onePos{ width / 2 +150 , height / 2 };
	//十の位の座標
	DirectX::SimpleMath::Vector2 tenPos{ onePos.x - 85, onePos.y };
	//百の位の座標
	DirectX::SimpleMath::Vector2 hundredPos{ onePos.x - 170, onePos.y };
	//アライブタイムの座標
	DirectX::SimpleMath::Vector2 timePos{ width / 3 - 50,height / 2 + 20 };

	//数字の切り取り位置
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

	//画像のスケール
	float texScale = 2.0f;

	//aliveTimeTextureの中心座標
	DirectX::SimpleMath::Vector2 aliveTimeTextureCenterPos{ 83.0f, 38.0f };

	//一の位の数字の計算
	int oneNum = static_cast<int>(m_aliveTime) % 10;
	//十の位の数字の計算
	int tenNum = static_cast<int>(m_aliveTime / 10) % 10;
	//百の位の数字の計算
	int hundredNum = static_cast<int>(m_aliveTime / 100) % 10;

	//アライブタイムの表示
	m_spriteBatch->Draw(m_aliveTimeTexture.Get(), timePos + aliveTimeTextureCenterPos, nullptr, DirectX::Colors::White, 0.0f, aliveTimeTextureCenterPos, texScale, DirectX::SpriteEffects_None, 0);
	//一の位の表示
	m_spriteBatch->Draw(m_numTexture.Get(), onePos, &rectNum[oneNum], DirectX::Colors::White, 0.0f, { 0.0f,0.0f }, texScale, DirectX::SpriteEffects_None, 0);
	//十の位の表示
	m_spriteBatch->Draw(m_numTexture.Get(), tenPos, &rectNum[tenNum], DirectX::Colors::White, 0.0f, { 0.0f,0.0f }, texScale, DirectX::SpriteEffects_None, 0);
	//百の位の表示
	m_spriteBatch->Draw(m_numTexture.Get(), hundredPos, &rectNum[hundredNum], DirectX::Colors::White, 0.0f, { 0.0f,0.0f }, texScale, DirectX::SpriteEffects_None, 0);
	

}
