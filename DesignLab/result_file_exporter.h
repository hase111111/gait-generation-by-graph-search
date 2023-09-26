#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "node.h"
#include "simulation_result_recorder.h"
#include "graph_search_result.h"


//! @class ResultFileExporter
//! @date 2023/08/24
//! @brief 結果をファイルに出力するクラス．
class ResultFileExporter final
{
public:

	ResultFileExporter() = default;


	//! @brief resultフォルダがなければ作成する．また，フォルダ名を指定する．
	void init();


	//! @brief シミュレーション結果をファイルに出力する．
	//! @n 初期化ができていない場合は，なにも出力しない．また，出力フラグがfalseの場合もなにも出力しない．
	//! @n 出力されるファイルは sim_result_回数.csv という名前になる．
	//! @param [in] recoder シミュレーションの結果
	void exportResult(const SimulationResultRecorder& recoder);


	//! @brief 出力を行うかどうかのフラグを設定する．
	//! @param [in] do_export 出力を行うかどうかのフラグ
	void setDoExport(const bool do_export) { m_do_export = do_export; }


private:

	void outputResultDetail(const SimulationResultRecorder& recoder, std::ofstream& stream);


	const std::string RESULT_FOLDER_NAME = "result";	//resultフォルダ名

	const std::string FILE_NAME = "sim_result_";		//ファイル名


	std::string m_folder_name;		//フォルダ名

	bool m_init_success = false;	//初期化成功フラグ

	bool m_do_export = true;		//出力を行うかどうかのフラグ

	int m_export_count = 0;			//出力回数
};


//! @file result_file_exporter.h
//! @date 2023/08/24
//! @author 長谷川
//! @brief 結果をファイルに出力するクラス．
//! @n 行数 : @lineinfo
