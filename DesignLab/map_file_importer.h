//! @file map_file_importer.h
//! @brief マップをcsvに出力したものを読み込むクラス．

#ifndef	DESIGNLAB_MAP_FILE_IMPORTER_H_
#define	DESIGNLAB_MAP_FILE_IMPORTER_H_

#include <optional>

#include "map_state.h"


namespace designlab
{

//! @class MapFileImporter
//! @brief マップをcsvに出力したものを読み込むクラス．
//! @n マップには脚接地可能点が羅列されているので，1行ずつ読み込み，それぞれMapに追加する．
class MapFileImporter final
{
public:

	//! @brief マップをcsvに出力したものを読み込む．
	//! @param [in] file_path 読み込むファイルのパス．
	//! @return std::optional<MapState> 読み込みに成功した場合，MapStateを返す．
	[[nodiscard]] std::optional<MapState> ImportMap(const std::string& file_path) const noexcept;
};

}	// namespace designlab


#endif	// DESIGNLAB_MAP_FILE_IMPORTER_H_