//! @file font_loader.h
//! @brief Dxlibのフォントを読み込むクラス．

#ifndef DESIGNLAB_FONT_LOADER_H_
#define DESIGNLAB_FONT_LOADER_H_


#include <string>
#include <map>

#include "singleton.h"


//! @class FontLoader
//! @brief Dxlibのフォントを読み込むクラス．
//! @details シングルトンクラスであり，FontLoader::GetIns()でインスタンスを取得する．
//! @n 呼び出すときは以下のようにする．
//! @code 
//! //例
//! int font_handle = FontLoader::GetIns()->GetFontHandle("font/consola.ttf");
//! 
//! DrawStringToHandle(0, 0, "Hello World", GetColor(255, 255, 255), font_handle);
//! @endcode
//! @see Singleton
class FontLoader final : public Singleton<FontLoader>
{
public:

	//! @brief Dxlibでは特定のフォントで描画する際に，フォントのハンドルを指定する．
	//! @n この関数では，フォントのファイルパスを指定すると，フォントのハンドル番号を返す．
	//! @n フォントがまだ読み込まれていない場合は，フォントを読み込んでから，ハンドル番号を返す．
	//! @n すでに読み込みずみのフォントを読み込んだ場合は，ハンドル番号を返す．
	//! @attention コンストラクタでフォントの読み込みを行うと，Dxlibの初期化前に呼び出されるため，失敗する可能性がある．
	//! そのため，この関数を呼び出すときは，Dxlibの初期化後に呼び出すこと．
	//! @param [in] file_path フォントのファイルパス.
	//! @return int フォントのハンドル番号．失敗したら-1を返す．成功値は正の値．
	[[nodiscard]] int GetFontHandle(const std::string& file_path);

private:

	friend Singleton<FontLoader>;
	FontLoader() = default;
	~FontLoader() = default;
	FontLoader(const FontLoader& r) = default;
	FontLoader& operator=(const FontLoader& r) = default;

	std::map<std::string, int> font_handle_map_;	//!< 読み込み済みのモデルのファイルパスとハンドル番号の対応表
};


#endif // DESIGNLAB_FONT_LOADER_H_