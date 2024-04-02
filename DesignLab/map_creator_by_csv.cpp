
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#include "map_creator_by_csv.h"

#include <format>
#include <filesystem>
#include <fstream>

#include "cassert_define.h"
#include "cmdio_util.h"
#include "map_file_importer.h"
#include "string_util.h"



namespace designlab
{

MapCreatorByCsv::MapCreatorByCsv(const std::string& map_file_path) : map_file_path_(map_file_path)
{
    // ファイルが存在するか確認し，なければアサート．
    const bool is_exist = std::filesystem::exists(map_file_path_);

    if (!is_exist)
    {
        std::string error_mes =
            std::format("map fileが存在しません．ファイルのパスは{}です．({}のコンストラクタ)",
                        map_file_path_, string_util::GetTypeName(*this));

        CmdIOUtil::Output(error_mes, OutputDetail::kError);

        assert(false);
    }
}

MapState MapCreatorByCsv::InitMap()
{
    std::optional<MapState> map_state;
    MapFileImporter map_file_importer;

    map_state = map_file_importer.ImportMap(map_file_path_);

    return map_state.value_or(MapState());
}

void MapCreatorByCsv::UpdateMap([[maybe_unused]] MapState* current_map)
{
    // 何もしない．
}

}  // namespace designlab
