#include "map_creator_by_csv.h"

#include <filesystem>
#include <fstream>

#include "cassert_define.h"
#include "designlab_string_util.h"


namespace dlsu = ::designlab::string_util;


MapCreatorByCsv::MapCreatorByCsv(const std::string& map_file_path) : map_file_path_(map_file_path)
{
	// ファイルが存在するか確認し，なければassert
	assert(std::filesystem::exists(map_file_path_));
}

MapState MapCreatorByCsv::InitMap()
{
	// ファイルを読み込む
	std::ifstream ifs(map_file_path_);
	std::string str;
	std::vector<std::string> str_vec;
	std::vector<std::vector<std::string>> str_vec_vec;

	MapState map_state;

	while (std::getline(ifs, str))
	{
		str_vec = dlsu::Split(str, ',');
		str_vec_vec.push_back(str_vec);

		map_state.AddMapPoint(designlab::Vector3{ std::stof(str_vec[0]), std::stof(str_vec[1]), std::stof(str_vec[2]) });
	}

	return map_state;
}

void MapCreatorByCsv::UpdateMap([[maybe_unused]] MapState* current_map)
{
}
