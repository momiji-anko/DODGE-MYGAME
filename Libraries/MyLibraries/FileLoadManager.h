/*
* 2023/05/24
* FileLoadManager.h
* ファイルの読み込みするクラス
* 麻生　楓
*/
#pragma once
#include<unordered_map>
#include"Singleton.h"

/// <summary>
/// ファイルの読み込みするクラス
/// </summary>
class FileLoadManager : public Singleton<FileLoadManager>
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	FileLoadManager() = default;
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~FileLoadManager() = default;

	/// <summary>
	/// ファイル読み込み
	/// </summary>
	/// <param name="fileName">読み込むファイルパス</param>
	/// <returns>読み込んだファイル</returns>
	std::vector<std::wstring> LoadFile(std::wstring fileName);

private:

	//読み込んだファイルを記憶用変数
	std::unordered_map<std::wstring, std::vector<std::wstring>> m_fileNames;

};