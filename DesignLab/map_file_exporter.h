//! @file map_file_exporter.h
//! @brief マップをcsvに出力するクラス．

#ifndef	DESIGNLAB_MAP_FILE_EXPORTER_H_
#define	DESIGNLAB_MAP_FILE_EXPORTER_H_

#include <string>

#include "map_state.h"


namespace designlab
{

//! @class MapFileExporter
//! @brief マップをcsvに出力するクラス．
//! @n マップには脚接地可能点が羅列されているので，1行ずつ出力する．
class MapFileExporter final
{
public:

	//! @brief マップをcsvに出力する．
	//! @param [in] file_path 出力するファイルのパス．
	//! @param [in] map_state 出力するマップのデータ．
	//! @return bool 出力に成功した場合，trueを返す．
	bool ExportMap(const std::string& file_path, const MapState& map_state) const noexcept;
};

}	// namespace designlab


#endif	// DESIGNLAB_MAP_FILE_EXPORTER_H_