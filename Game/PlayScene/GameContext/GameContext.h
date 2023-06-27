/*
* 2023/04/28
* GameContext.h
* ゲームコンテキスト
* 麻生楓
*/
#include"Libraries/MyLibraries/Singleton.h"
#include<CommonStates.h>
#include<SpriteBatch.h>
#include<Keyboard.h>


/// <summary>
/// ゲームコンテキスト
/// </summary>
class GameContext:public Singleton<GameContext>
{
public:
	/// <summary>
	/// コモンステートの設定
	/// </summary>
	/// <param name="commonState">コモンステートの生ポインタ</param>
	void SetCommonState(DirectX::CommonStates* commonState) { m_commonState = commonState; }
	/// <summary>
	/// コモンステートの取得
	/// </summary>
	/// <returns>コモンステートの生ポインタ</returns>
	DirectX::CommonStates* GetCommonState()const { return m_commonState; }

	/// <summary>
	/// スプライトバッチの設定
	/// </summary>
	/// <param name="spriteBatcth">スプライトバッチの生ポインタ</param>
	void SetSpriteBatcth(DirectX::SpriteBatch* spriteBatcth) { m_spriteBatcth = spriteBatcth;}
	/// <summary>
	/// スプライトバッチの取得
	/// </summary>
	/// <returns>スプライトバッチの生ポインタ</returns>
	DirectX::SpriteBatch* GetSpriteBatcth()const { return m_spriteBatcth; }
	
	/// <summary>
	/// キーボードステートトラッカーの設定
	/// </summary>
	/// <param name="keyboad">キーボードステートトラッカー</param>
	void SetKeyboardStateTracker(DirectX::Keyboard::KeyboardStateTracker* keyboad) { m_keyBoardStateTracker = keyboad; }
	/// <summary>
	/// キーボードステートトラッカーの取得
	/// </summary>
	/// <returns>キーボードステートトラッカー</returns>
	DirectX::Keyboard::KeyboardStateTracker* GetKeyboardStateTracker() const{ return m_keyBoardStateTracker; }
		
	/// <summary>
	/// プレイヤーが死亡しているか取得
	/// </summary>
	/// <returns>true = 死亡していいる、false = 死亡していない</returns>
	bool GetIsPlayerDeath() { return m_isPlayerDeath; }

	/// <summary>
	/// プレイヤーが死亡しているか設定
	/// </summary>
	/// <param name="flag">true = 死亡していいる、false = 死亡していない</param>
	void SetIsPlayerDeath(bool flag) { m_isPlayerDeath = flag; }


public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameContext();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameContext();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();


private:
	//コモンステート
	DirectX::CommonStates* m_commonState;
	//スプライトバッチ
	DirectX::SpriteBatch* m_spriteBatcth;
	//キーボードステートトラッカー
	DirectX::Keyboard::KeyboardStateTracker* m_keyBoardStateTracker;
	//プレイシーンストップ
	bool m_isPlayerDeath;
};
