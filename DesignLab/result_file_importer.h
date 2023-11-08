//! @file result_file_importer.h
//! @brief 結果をファイルから読み込むクラス．


#ifndef DESIGNLAB_RESULT_FILE_IMPORTER_H_
#define DESIGNLAB_RESULT_FILE_IMPORTER_H_


#include "result_file_exporter.h"


class ResultFileImporter final
{
public:

	//! @brief ノードリストとマップの状態をファイルから読み込む．
	//! @param [in] file_path ノードリストのパス
	//! @param [out] node_list ノードリスト
	//! @param [out] map_state マップの状態
	//! @return bool 読み込みが成功したかどうか
	bool ImportNodeListAndMapState(
		const std::string& file_path,
		std::vector<RobotStateNode>* node_list,
		MapState* map_state
	) const;


private:

	bool ImportNodeList(const std::string& file_path, std::vector<RobotStateNode>* node_list) const;

	bool ImportMapState(const std::string& file_path, MapState* map_state) const;
};


#endif