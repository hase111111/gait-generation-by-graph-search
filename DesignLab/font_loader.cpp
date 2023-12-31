
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "font_loader.h"

#include <DxLib.h>


namespace designlab
{

int FontLoader::GetFontHandle(const std::string& file_path)
{
    // すでに読み込み済みのフォントの場合は，ハンドル番号を返す．
    if (font_handle_map_.count(file_path) != 0)
    {
        return font_handle_map_[file_path];
    }

    // フォントを読み込む．
    int handle = CreateFontToHandle(file_path.c_str(), 16, 1);

    // 読み込みに失敗した場合は-1を返す．
    if (handle == -1)
    {
        return -1;
    }

    // 読み込みに成功した場合は，対応表に追加する．
    font_handle_map_[file_path] = handle;

    return handle;
}

}  // namespace designlab
