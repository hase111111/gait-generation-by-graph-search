#include "map_creator_by_csv.h"

#include <filesystem>
#include <fstream>

#include "cassert_define.h"
#include "cmdio_util.h"
#include "designlab_string_util.h"


namespace dlio = ::designlab::cmdio;
namespace dlsu = ::designlab::string_util;


MapCreatorByCsv::MapCreatorByCsv(const std::string& map_file_path) : map_file_path_(map_file_path)
{
	// ファイルが存在するか確認し，なければassert．
	const bool is_exist = std::filesystem::exists(map_file_path_);

	if (!is_exist)
	{
		dlio::Output("map fileが存在しません．ファイルのパスは" + map_file_path_ + "です．(" +
			typeid(MapCreatorByCsv).name() + "のコンストラクタ)", OutputDetail::kError);

		assert(false);
	}

}

MapState MapCreatorByCsv::InitMap()
{
	// ファイルを読み込む
	std::ifstream ifs(map_file_path_);

	// ファイルが開けなかったらassert．
	if (!ifs)
	{
		dlio::Output("map fileが開けません．ファイルのパスは" + map_file_path_ + "です．(" +
			typeid(MapCreatorByCsv).name() + "のInitMap関数)", OutputDetail::kError);

		assert(false);

		return MapState();
	}

	std::string str;
	std::vector<std::string> str_vec;
	std::vector<std::vector<std::string>> str_vec_vec;

	MapState map_state;

	while (std::getline(ifs, str))
	{
		str_vec = dlsu::Split(str, ",");
		str_vec_vec.push_back(str_vec);

		map_state.AddMapPoint(designlab::Vector3{ std::stof(str_vec[0]), std::stof(str_vec[1]), std::stof(str_vec[2]) });
	}

	return map_state;
}

void MapCreatorByCsv::UpdateMap([[maybe_unused]] MapState* current_map)
{
	// 何もしない
}
