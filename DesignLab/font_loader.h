//! @file font_loader.h
//! @brief Dxlibのフォントを読み込むクラス

#ifndef DESIGNLAB_FONT_LOADER_H_
#define DESIGNLAB_FONT_LOADER_H_


#include <string>
#include <map>

#include "singleton.h"


//! @class FontLoader
//! @brief Dxlibのフォントを読み込むクラス．シングルトンであり，FontLoader::GetIns()でインスタンスを取得する．
//! @n 呼び出すときはFontLoader::GetIns()->GetFontHandle(file_path)とする．
class FontLoader final : public Singleton<FontLoader>
{
public:

	//! @brief Dxlibでは特定のフォントで描画する際に，フォントのハンドルを指定する．
	//! @n この関数では，フォントのファイルパスを指定すると，フォントのハンドル番号を返す．
	//! @n フォントがまだ読み込まれていない場合は，フォントを読み込んでから，ハンドル番号を返す．
	//! @n すでに読み込みずみのフォントを読み込んだ場合は，ハンドル番号を返す．
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