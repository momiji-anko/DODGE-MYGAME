#include"pch.h"
#include"StageSelect.h"
#include"Libraries/MyLibraries/TextureManager.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"DeviceResources.h"
#include"Game/PlayScene/GameContext/GameContext.h"
#include"Libraries/MyLibraries/MemoryLeakDetector.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="stageManager">ステージマネージャーの生ポインタ</param>
StageSelect::StageSelect(StageManager* stageManager)
	:
	m_stageTextTextureFilePath{},
	m_stageNumTextureFilePath{},
	m_stageArrowTextureFilePath{},
	m_texturePosition{DirectX::SimpleMath::Vector2::Zero},
	m_selectStageNum{0},
	m_stageManager{ stageManager },
	m_stageMaxNum{0},
	m_stageTextTexture{},
	m_stageNumTexture{},
	m_stageArrowTexture{}
{
}

/// <summary>
/// デストラクタ
/// </summary>
StageSelect::~StageSelect()
{
	m_stageTextTexture.Reset();
	m_stageNumTexture.Reset();
	m_stageArrowTexture.Reset();
}

/// <summary>
/// 初期化
/// </summary>
void StageSelect::Initialize()
{

	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// ウィンドウサイズの取得
	RECT size = pDR->GetOutputSize();

	//テクスチャファイルパスの初期化
	m_stageTextTextureFilePath = L"Resources/Textures/stageText.png";
	m_stageNumTextureFilePath = L"Resources/Textures/stageNum.png";
	m_stageArrowTextureFilePath = L"Resources/Textures/Arrow.png";

	//テクスチャマネージャーの取得
	TextureManager& textureManager = TextureManager::GetInstance();

	//テクスチャの読み込み
	m_stageTextTexture = textureManager.LoadTexture(m_stageTextTextureFilePath);
	m_stageNumTexture = textureManager.LoadTexture(m_stageNumTextureFilePath);
	m_stageArrowTexture = textureManager.LoadTexture(m_stageArrowTextureFilePath);


	//ステージ画像のY座標
	float stageSelectPosY = static_cast<float>(size.bottom / 2.0f + size.bottom / 4.0);
	//ステージ画像のX座標
	float stagePositionX = static_cast<float>(size.right / 2.0f);
	//ステージ画像の座標設定
	m_texturePosition = { stagePositionX,stageSelectPosY };

	//ファイルマネージャーの取得
	FileLoadManager& fileLoadManager = FileLoadManager::GetInstance();
	//ファイルの数取得
	m_stageMaxNum = fileLoadManager.LoadFile(L"Resources/StageData/").size();


}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
/// <returns>次の選択画面に行くか</returns>
bool StageSelect::Update(const DX::StepTimer& timer)
{
	//ゲームコンテキスト取得
	GameContext& gameContext = GameContext::GetInstance();
	//ゲームコンテキストからキーボードステートトラッカーの取得
	DirectX::Keyboard::KeyboardStateTracker* keyboard = gameContext.GetKeyboardStateTracker();

	//ボタンを押したか
	bool isKeyPush = false;

	//ステージ選択
	if (keyboard->IsKeyPressed(DirectX::Keyboard::Keys::Left))
	{
		//押したらtrueにする
		isKeyPush = true;

		//ステージの番号を減らす
		m_selectStageNum -= 1;

	}
	else if (keyboard->IsKeyPressed(DirectX::Keyboard::Keys::Right))
	{
		//押したらtrueにする
		isKeyPush = true;

		//ステージの番号を増やす
		m_selectStageNum += 1;
	}


	//押したらステージマネージャーのステージを変える
	if (isKeyPush)
	{
		//音を出す
		ADX2::GetInstance().Play(CRI_CUESHEET_0_BUTTON);

		//ステージ選択の値がステージの数より低くったらステージ１にする
		if (m_selectStageNum < static_cast<int>(StageManager::StageSelect::Stage1))
		{
			m_selectStageNum = static_cast<int>(StageManager::StageSelect::Stage1);
		}
		//ステージ選択の値がステージの数より大きくなったらステージ３にする
		else if (m_selectStageNum > m_stageMaxNum - 1)
		{
			m_selectStageNum = m_stageMaxNum - 1;
		}

		//背景のステージを変える
		m_stageManager->Initialize(static_cast<StageManager::StageSelect>(m_selectStageNum));

	}

	//スペースキーを押すと次の選択画面に行くに移動
	if (keyboard->IsKeyPressed(DirectX::Keyboard::Keys::Space))
	{
		//音を出す
		ADX2::GetInstance().Play(CRI_CUESHEET_0_BUTTON);

		return true;
	}

	return false;
}

/// <summary>
/// 描画
/// </summary>
void StageSelect::Draw()
{
	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//テクスチャマネージャーの取得
	TextureManager& textureManager = TextureManager::GetInstance();

	//ステージ番号テクスチャの画像サイズを取得
	DirectX::SimpleMath::Vector2 stageNumTextureSize = textureManager.GetTextureSize(m_stageNumTextureFilePath);
	//ステージテキストテクスチャの画像サイズを取得
	DirectX::SimpleMath::Vector2 stageTextTextureSize = textureManager.GetTextureSize(m_stageTextTextureFilePath);
	//アローテクスチャの画像サイズを取得
	DirectX::SimpleMath::Vector2 stageArrowTextureSize = textureManager.GetTextureSize(m_stageArrowTextureFilePath);

	//ゲームコンテキスト取得
	GameContext& gameContext = GameContext::GetInstance();
	//ゲームコンテキストからスプライトバッチ取得
	DirectX::SpriteBatch* spriteBatch= gameContext.GetSpriteBatcth();

	//数字の一マスのサイズ
	float stageNumSize = (stageNumTextureSize.x) / 10;

	//切り取り位置
	RECT texRect ={

		stageNumSize * m_selectStageNum + 1,
		0,
		stageNumSize * (m_selectStageNum + 1) - (stageNumSize / 5),
		stageNumTextureSize.y
	};

	//拡大率
	float textureScale = 1.5f;

	//カラー
	DirectX::SimpleMath::Vector4 color = DirectX::Colors::Red;

	//ステージ文字の描画
	spriteBatch->Draw(m_stageTextTexture.Get(), m_texturePosition, nullptr, color, 0.0f, stageTextTextureSize / 2.0f, textureScale);

	//数字の位置
	DirectX::SimpleMath::Vector2 numPosition = m_texturePosition + DirectX::SimpleMath::Vector2(stageTextTextureSize.x + stageNumTextureSize.x / 10, 0);

	//数字の描画
	spriteBatch->Draw(m_stageNumTexture.Get(), numPosition, &texRect, color, 0.0f, DirectX::SimpleMath::Vector2(stageNumSize, stageNumTextureSize.y / 2.0f), textureScale);

	//選択しているステージ番号が１より高ければ左矢印表示
	if (m_selectStageNum > 0)
	{
		spriteBatch->Draw(m_stageArrowTexture.Get(), m_texturePosition - DirectX::SimpleMath::Vector2(stageArrowTextureSize.x + stageTextTextureSize.x, 0), nullptr, DirectX::Colors::White, DirectX::XM_PI, stageArrowTextureSize / 2, textureScale);
	}
	
	//選択しているステージ番号がステージの最大数より低くくれば
	if (m_selectStageNum < m_stageMaxNum - 1)
	{
		spriteBatch->Draw(m_stageArrowTexture.Get(), numPosition + DirectX::SimpleMath::Vector2(stageArrowTextureSize.x, 0), nullptr, DirectX::Colors::White, 0.0f, stageArrowTextureSize / 2, textureScale);
	}
	
}

/// <summary>
/// 終了処理
/// </summary>
void StageSelect::Finalize()
{

}
