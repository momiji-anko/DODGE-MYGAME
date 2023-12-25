/// <summary>
/// バイナリファイルを扱うクラス
/// </summary>
#pragma once

#include <memory>

class BinaryFile
{
public:

	/// <summary>
	/// バイナリファイル読み込み
	/// </summary>
	/// <param name="fileName">ファイルパス</param>
	/// <returns>バイナリファイル</returns>
	static BinaryFile LoadFile(const wchar_t* fileName);

	//コンストラクタ
	BinaryFile();

	// ムーブコンストラクタ
	BinaryFile(BinaryFile&& in);

	// アクセサ
	char* GetData() { return m_data.get(); }
	unsigned int GetSize() { return m_size; }
protected:

	// データ
	std::unique_ptr<char[]> m_data;

	// サイズ
	unsigned int m_size;
};
