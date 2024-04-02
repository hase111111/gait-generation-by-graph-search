
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#include "map_file_exporter.h"

#include <fstream>

#include "cmdio_util.h"


namespace designlab
{

bool MapFileExporter::ExportMap(const std::string& file_path,
                                const MapState& map_state) const noexcept
{
    // ファイルを開く．
    std::ofstream ofs(file_path);

    // ファイルが開けないならば false を返す．
    if (!ofs.is_open())
    {
        CmdIOUtil::Output("ファイルを開けませんでした．", OutputDetail::kError);

        return false;
    }

    // ファイルを1行ずつ書き込む．
    const size_t map_point_size = map_state.GetMapPointSize();

    for (size_t i = 0; i < map_point_size; ++i)
    {
        ofs << map_state.GetMapPoint(i) << std::endl;
    }

    return true;
}

}  // namespace designlab
