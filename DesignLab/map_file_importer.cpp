#include "map_file_importer.h"

#include <fstream>
#include <sstream>

#include "cmdio_util.h"

namespace dle = ::designlab::enums;
namespace dlio = ::designlab::cmdio;


std::optional<MapState> MapFileImporter::ImportMap(const std::string& file_path) const noexcept
{
	// ファイルを開く
	std::ifstream ifs(file_path);

	// ファイルが開けないならばfalseを返す．
	if (not ifs.is_open())
	{
		dlio::Output("ファイルを開けませんでした．", dle::OutputDetail::kError);

		return std::nullopt;
	}

	// ファイルを1行ずつ読み込み，Mapに追加する．
	std::vector<::designlab::Vector3> map_point;

	std::string line;

	while (std::getline(ifs, line))
	{
		std::istringstream iss(line);

		try
		{
			::designlab::Vector3 point;
			iss >> point;

			map_point.push_back(point);
		}
		catch (...)
		{
			dlio::Output("読み込むことができないデータがあったため無視します.", dle::OutputDetail::kWarning);
		}
	}

	return MapState(map_point);
}
