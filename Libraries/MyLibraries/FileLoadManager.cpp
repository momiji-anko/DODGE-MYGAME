/*
* 2023/05/24
* FileLoadManager.cpp
* ファイルの読み込みするクラス
* 麻生　楓
*/
#include"pch.h"
#include"FileLoadManager.h"
#include<windows.h>

/// <summary>
/// ファイル読み込み
/// </summary>
/// <param name="fileName">読み込むファイルパス</param>
/// <returns>読み込んだファイル</returns>
std::vector<std::wstring> FileLoadManager::LoadFile(std::wstring fileName)
{
	if (m_fileNames[fileName].size() == 0)
	{
		//ファイル情報
		WIN32_FIND_DATA fileFindData;
		//アスタリスク
		std::wstring asterisk = L"*";

		//ファイル検索
		const HANDLE hFindFile = FindFirstFile((fileName + asterisk).c_str(), &fileFindData);

		//ファイルが検索できたか
		if (hFindFile != INVALID_HANDLE_VALUE)
		{

			do
			{
				//ファイルがディレクトリではない場合
				if (!(fileFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					//検索したファイルの名前
					std::wstring findFileName = fileFindData.cFileName;
					//ファイパスと検索したファイルを足す
					std::wstring jsonFilePath = fileName + findFileName;
					//ステージJsonに追加
					m_fileNames[fileName].push_back(jsonFilePath);
				}

				//次のファイルがあったらループ
			} while (FindNextFile(hFindFile, &fileFindData));

		}
	}

	//読み込んだファイルを返す
	return m_fileNames[fileName];
}