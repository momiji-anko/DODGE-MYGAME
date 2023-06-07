#include"pch.h"
#include"ModeSelect.h"
#include"Libraries/MyLibraries/TextureManager.h"
#include"Game/PlayScene/GameContext/GameContext.h"
#include"Libraries/AXD2/Adx2.h"
#include"DeviceResources.h"
#include"../../Helpers/CueSheet_0.h"
#include"../../Helpers/DODGESound_acf.h"

/// <summary>
/// コンストラクタ
/// </summary>
ModeSelect::ModeSelect()
	:
	m_modeFilePaths{},
	m_playerMode(GameMain::PlayerMode::Player1)
{
}

/// <summary>
/// デストラクタ
/// </summary>
ModeSelect::~ModeSelect()
{
}

/// <summary>
/// 初期化
/// </summary>
void ModeSelect::Initialize()
{
	//モードファイルパスの設定
	m_modeFilePaths.resize(static_cast<int>(GameMain::PlayerMode::Player2));
	m_modeFilePaths[0] = L"Resources/Textures/SOLO.png";
	m_modeFilePaths[1] = L"Resources/Textures/MARUTI.png";

	//テクスチャマネージャーの取得
	TextureManager& textureManager = TextureManager::GetInstance();

	//画像の読み込み
	for (int i = 0; i < m_modeFilePaths.size(); i++)
	{
		m_modeSelectTextures.push_back(textureManager.LoadTexture(m_modeFilePaths[i].c_str()));
	}

}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
/// <returns>次の選択画面に行くか</returns>
bool ModeSelect::Update(const DX::StepTimer& timer)
{
	//キーボードステートトラッカーの取得
	DirectX::Keyboard::KeyboardStateTracker* keybord = GameContext::GetInstance().GetKeyboardStateTracker();

	//モード選択
	if (keybord->IsKeyPressed(DirectX::Keyboard::Right))
	{
		m_playerMode = GameMain::PlayerMode::Player2;

		ADX2::GetInstance().Play(CRI_CUESHEET_0_BUTTON);
	}
	else if (keybord->IsKeyPressed(DirectX::Keyboard::Left))
	{
		m_playerMode = GameMain::PlayerMode::Player1;

		ADX2::GetInstance().Play(CRI_CUESHEET_0_BUTTON);
	}

	//スペースキーを押す次に移行する
	if (keybord->IsKeyPressed(DirectX::Keyboard::Space))
	{
		ADX2::GetInstance().Play(CRI_CUESHEET_0_BUTTON);

		return true;
	}

	return false;
}

/// <summary>
/// 描画
/// </summary>
void ModeSelect::Draw()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// ウィンドウサイズの取得
	RECT size = pDR->GetOutputSize();

	//windowサイズの4分の１の値
	float windowHeightQuarter = static_cast<float>(size.right / 4.0f);
	//windowサイズの半分の値
	float windowHeightHelf = static_cast<float>(size.right / 2.0f);
	//モード画像のY座標
	float modeSelectPosY = static_cast<float>(size.bottom / 2.0f + size.bottom / 4.0);

	//モードの画像座標
	DirectX::SimpleMath::Vector2 modeSelectPosition[2] =
	{
		//ソロモード
		DirectX::SimpleMath::Vector2{windowHeightHelf - windowHeightQuarter , modeSelectPosY },
		//マルチモード
		DirectX::SimpleMath::Vector2{windowHeightHelf + windowHeightQuarter , modeSelectPosY },

	};

	//画像の拡大率
	float scale = 2.0f;

	for (int i = 0; i < m_modeSelectTextures.size(); i++)
	{
		//カラー
		DirectX::SimpleMath::Vector4 color = DirectX::Colors::White;

		//画像の中心位置
		const DirectX::SimpleMath::Vector2& center = textureManager.GetTextureSize(m_modeFilePaths[i].c_str()) / 2.0f;

		//現在の選択しているモードを赤色で表示する
		if (i == static_cast<int>(m_playerMode) - 1)
		{
			color = DirectX::Colors::Red;
		}

		//画像描画
		GameContext::GetInstance().GetSpriteBatcth()->Draw(m_modeSelectTextures[i].Get(), modeSelectPosition[i], nullptr, color, 0.0f, center, scale);
	}


}

/// <summary>
/// 終了処理
/// </summary>
void ModeSelect::Finalize()
{
}
