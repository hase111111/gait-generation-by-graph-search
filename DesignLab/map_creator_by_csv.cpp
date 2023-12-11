#include "map_creator_by_csv.h"

#include <filesystem>
#include <fstream>

#include "cassert_define.h"
#include "cmdio_util.h"
#include "designlab_string_util.h"
#include "map_file_importer.h"


namespace dle = ::designlab::enums;
namespace dlio = ::designlab::cmdio;
namespace dlsu = ::designlab::string_util;


MapCreatorByCsv::MapCreatorByCsv(const std::string& map_file_path) : map_file_path_(map_file_path)
{
	// ファイルが存在するか確認し，なければassert．
	const bool is_exist = std::filesystem::exists(map_file_path_);

	if (!is_exist)
	{
		dlio::Output("map fileが存在しません．ファイルのパスは" + map_file_path_ + "です．(" +
			typeid(MapCreatorByCsv).name() + "のコンストラクタ)", dle::OutputDetail::kError);

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
	// 何もしない
}
