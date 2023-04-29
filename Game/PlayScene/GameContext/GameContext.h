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
	DirectX::CommonStates* GetCommonState() { return m_commonState; }

	/// <summary>
	/// スプライトバッチの設定
	/// </summary>
	/// <param name="spriteBatcth">スプライトバッチの生ポインタ</param>
	void SetSpriteBatcth(DirectX::SpriteBatch* spriteBatcth) { m_spriteBatcth = spriteBatcth;}
	/// <summary>
	/// スプライトバッチの取得
	/// </summary>
	/// <returns>スプライトバッチの生ポインタ</returns>
	DirectX::SpriteBatch* GetSpriteBatcth() { return m_spriteBatcth; }
	
	/// <summary>
	/// キーボードステートトラッカーの設定
	/// </summary>
	/// <param name="keyboad">キーボードステートトラッカー</param>
	void SetKeyboardStateTracker(DirectX::Keyboard::KeyboardStateTracker* keyboad) { m_keyBoardStateTracker = keyboad; }
	/// <summary>
	/// キーボードステートトラッカーの取得
	/// </summary>
	/// <returns>キーボードステートトラッカー</returns>
	DirectX::Keyboard::KeyboardStateTracker* GetKeyboardStateTracker() { return m_keyBoardStateTracker; }
		



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


};
