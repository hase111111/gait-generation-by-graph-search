
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#include "map_file_importer.h"

#include <fstream>
#include <sstream>
#include <vector>

#include "cmdio_util.h"


namespace designlab
{

std::optional<MapState> MapFileImporter::ImportMap(const std::string& file_path) const noexcept
{
    // ファイルを開く．
    std::ifstream ifs(file_path);

    // ファイルが開けないならば false を返す．
    if (!ifs.is_open())
    {
        CmdIOUtil::Output("ファイルを開けませんでした．", OutputDetail::kError);

        return std::nullopt;
    }

    // ファイルを1行ずつ読み込み，Mapに追加する．
    std::vector<Vector3> map_point;

    std::string line;

    while (std::getline(ifs, line))
    {
        std::istringstream iss(line);

        try
        {
            Vector3 point;
            iss >> point;

            map_point.push_back(point);
        }
        catch (...)
        {
            CmdIOUtil::Output("読み込むことができないデータがあったため無視します.",
                              OutputDetail::kWarning);
        }
    }

    return MapState(map_point);
}

}  // namespace designlab
