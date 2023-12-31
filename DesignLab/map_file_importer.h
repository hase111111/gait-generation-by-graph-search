
//! @file      map_file_importer.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_MAP_FILE_IMPORTER_H_
#define DESIGNLAB_MAP_FILE_IMPORTER_H_

#include <optional>
#include <string>

#include "map_state.h"


namespace designlab
{

//! @class MapFileImporter
//! @brief マップを csv に出力したものを読み込むクラス．
//! @n マップには脚接地可能点が羅列されているので，1行ずつ読み込み，それぞれマップに追加する．
class MapFileImporter final
{
public:
    //! @brief マップを csv に出力したものを読み込む．
    //! @param[in] file_path 読み込むファイルのパス．
    //! @return 読み込みに成功した場合，MapState を返す．
    //! @retval std::nullopt 読み込みに失敗した場合．
    [[nodiscard("読み込んだデータを破棄しています．")]]
    std::optional<MapState> ImportMap(const std::string& file_path) const noexcept;
};

}  // namespace designlab


#endif  // DESIGNLAB_MAP_FILE_IMPORTER_H_
