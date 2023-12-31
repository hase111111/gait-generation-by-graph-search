
//! @file      map_file_exporter.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_MAP_FILE_EXPORTER_H_
#define DESIGNLAB_MAP_FILE_EXPORTER_H_

#include <string>

#include "map_state.h"


namespace designlab
{

//! @class MapFileExporter
//! @brief マップを csv に出力するクラス．
//! @n マップには脚接地可能点が羅列されているので，1行ずつ出力する．
class MapFileExporter final
{
public:
    //! @brief マップを csv に出力する．
    //! @param[in] file_path 出力するファイルのパス．
    //! @param[in] map_state 出力するマップのデータ．
    //! @retval true 出力に成功した場合．
    //! @retval false 出力に失敗した場合．
    bool ExportMap(const std::string& file_path, const MapState& map_state) const noexcept;
};

}  // namespace designlab


#endif  // DESIGNLAB_MAP_FILE_EXPORTER_H_
