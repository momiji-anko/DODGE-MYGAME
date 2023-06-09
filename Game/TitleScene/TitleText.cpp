#include"pch.h"
#include"TitleText.h"
#include"DeviceResources.h"
#include"Libraries/MyLibraries/TextureManager.h"
#include"Game/PlayScene/GameContext/GameContext.h"

//タイトルの移動時間
const float TitleText::MOVE_TIME = 3.0f;
//タイトルの移動角度
const float TitleText::MOVE_RORATION = 0.1f;
//タイトルの初期角度
const float TitleText::RORATION_ZERO = 0.0f;
//タイトルの回転ルーチンのリセット
const int TitleText::ROTATION_RESET_ROUTINE = 2;
//カメラの回転速度
const float TitleText::CAMERA_ROT_SPEED = 0.001f;
//アルファの最大値
const float TitleText::MOVED_TEXTURE_ALPHA = 1.0f;
//アルファの最小値
const float TitleText::MOVEING_TEXTURE_ALPHA = 0.7f;

/// <summary>
/// コンストラクタ
/// </summary>
TitleText::TitleText()
	:
	m_titleTextureFilePath{L"Resources/Textures/title.png"},
	m_titileTexture{},
	m_titleTimer_s{0.0f},
	m_titleRoutine{0},
	m_titlePosition{DirectX::SimpleMath::Vector2::Zero,DirectX::SimpleMath::Vector2::Zero },
	m_titleRotetion{0.0f},
	m_titleAlpha{0.0f}
{
}

/// <summary>
/// デストラクタ
/// </summary>
TitleText::~TitleText()
{
	m_titileTexture.Reset();
}

/// <summary>
/// 初期化
/// </summary>
void TitleText::Initialize()
{
	//タイトル画像の読み込み
	m_titileTexture = TextureManager::GetInstance().LoadTexture(m_titleTextureFilePath.c_str());
	//タイトルアルファ値の初期化
	m_titleAlpha = MOVEING_TEXTURE_ALPHA;

}


/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void TitleText::Update(const DX::StepTimer& timer)
{

	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// ウィンドウサイズの取得
	RECT size = pDR->GetOutputSize();

	//経過時間
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	//タイトルタイマーを経過時間で足す
	m_titleTimer_s += elapsedTime_s;

	//タイトルの移動前の座標
	DirectX::SimpleMath::Vector2 titleStartPosition = DirectX::SimpleMath::Vector2(static_cast<float>(size.right), 0.0f);
	//タイトルの移動後の座標
	DirectX::SimpleMath::Vector2 titleMovedPosition = DirectX::SimpleMath::Vector2::Zero;


	switch (m_titleRoutine)
	{
	case 0:
		//ラープで移動
		m_titlePosition[0] = DirectX::SimpleMath::Vector2::Lerp(-titleStartPosition, titleMovedPosition, easeOutCubic(m_titleTimer_s / MOVE_TIME));
		m_titlePosition[1] = DirectX::SimpleMath::Vector2::Lerp(titleStartPosition, titleMovedPosition, easeOutCubic(m_titleTimer_s / MOVE_TIME));
		break;
	case 1:
		//タイマー初期化
		m_titleTimer_s = 0;
		m_titleRoutine++;
		//アルファ値を１にする
		m_titleAlpha = MOVED_TEXTURE_ALPHA;
	case 2:
		//タイトル文字の回転
		m_titleRotetion = Lerp(RORATION_ZERO, MOVE_RORATION, m_titleTimer_s / MOVE_TIME);
		break;
	case 3:
		//タイマー初期化
		m_titleTimer_s = 0;
		m_titleRoutine++;
	case 4:

		//タイトル文字の回転
		m_titleRotetion = Lerp(MOVE_RORATION, RORATION_ZERO, m_titleTimer_s / MOVE_TIME);

		break;
	case 5:
		//タイマー初期化
		m_titleTimer_s = 0;
		m_titleRoutine++;
	case 6:

		//タイトル文字の回転
		m_titleRotetion = Lerp(RORATION_ZERO, -MOVE_RORATION, m_titleTimer_s / MOVE_TIME);

		break;
	case 7:
		m_titleTimer_s = 0;
		m_titleRoutine++;
	case 8:

		//タイトル文字の回転
		m_titleRotetion = Lerp(-MOVE_RORATION, RORATION_ZERO, m_titleTimer_s / MOVE_TIME);

		break;
	case 9:
		m_titleTimer_s = 0;
		//回転を繰り返す
		m_titleRoutine = ROTATION_RESET_ROUTINE;
	default:
		break;
	}

	//移動し終わったら次に行く
	if (m_titleTimer_s > MOVE_TIME)
	{
		m_titleTimer_s = MOVE_TIME;
		m_titleRoutine++;
	}


}


/// <summary>
/// 描画
/// </summary>
void TitleText::Draw()
{
	//タイトル画面のサイズ取得
	DirectX::SimpleMath::Vector2 textureSize = TextureManager::GetInstance().GetTextureSize(m_titleTextureFilePath);

	//タイトル文字描画
	for (DirectX::SimpleMath::Vector2& position : m_titlePosition)
	{
		GameContext::GetInstance().GetSpriteBatcth()->Draw(m_titileTexture.Get(), position + textureSize / 2, nullptr, DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_titleAlpha), m_titleRotetion, textureSize / 2);
	}

}

/// <summary>
/// 終了処理
/// </summary>
void TitleText::Finalize()
{
}
