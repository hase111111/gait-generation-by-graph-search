//! @file result_file_exporter.h
//! @brief 結果をファイルに出力するクラス．


#ifndef DESIGNLAB_RESULT_FILE_EXPORTER_H_
#define DESIGNLAB_RESULT_FILE_EXPORTER_H_


#include <string>
#include <vector>
#include <fstream>

#include "robot_state_node.h"
#include "simulation_result_recorder.h"


class ResultFileConst final
{
public:
	const static std::string kDirectoryPath;//!< 出力先ディレクトリ(フォルダ)名

	const static std::string kFileName;		//!< ファイル名 ( 人間が見る用 )

	const static std::string kNodeListName;	//!< ノードリストのファイル名( プログラムの読み込み用 )

	const static std::string kMapStateName;	//!< マップ状態のファイル名( プログラムの読み込み用 )
};


//! @class ResultFileExporter
//! @brief 結果をファイルに出力するクラス．
class ResultFileExporter final
{
public:

	ResultFileExporter();

	//! @brief resultフォルダがなければ作成する．また，フォルダ名を指定する．
	void Init();

	//! @brief シミュレーション結果を追加する．
	//! @param [in] simu_result シミュレーションの結果
	void PushSimulationResult(const SimulationResultRecorder& simu_result);

	//! @brief 出力を行うかどうかのフラグを設定する．
	//! @param [in] do_export 出力を行うかどうかのフラグ
	inline void SetDoExport(const bool do_export) { do_export_ = do_export; }


	//! @brief 最新のノードリストをファイルに出力する．
	//! @n 初期化ができていない場合は，なにも出力しない．また，出力フラグがfalseの場合もなにも出力しない．
	//! @n Init()が呼ばれていない場合は，なにも出力しないので，必ずInit()を呼び出してから呼び出すこと．
	void ExportLatestNodeList() const;

	//! @brief 最新のマップ状態をファイルに出力する．
	//! @n 初期化ができていない場合は，なにも出力しない．また，出力フラグがfalseの場合もなにも出力しない．
	//! @n Init()が呼ばれていない場合は，なにも出力しないので，必ずInit()を呼び出してから呼び出すこと．
	void ExportLatestMapState() const;

	//! @brief シミュレーション結果を全てファイルに出力する．
	//! @n 初期化ができていない場合は，なにも出力しない．また，出力フラグがfalseの場合もなにも出力しない．
	//! @n Init()が呼ばれていない場合は，なにも出力しないので，必ずInit()を呼び出してから呼び出すこと．
	void ExportResult() const;

private:

	//! @brief シミュレーション結果をファイルに出力する．
	//! @param [in] recoder シミュレーション結果
	//! @param [in] simu_index シミュレーション番号
	//! @return 出力に成功したか
	bool OutputResultDetail(const SimulationResultRecorder& recoder, int simu_index) const;


	std::string folder_name_;	//!< 出力先フォルダ名

	bool init_success_;	//!< 初期化成功フラグ

	bool do_export_;	//!< 出力を行うかどうかのフラグ


	std::vector<SimulationResultRecorder> result_list_;	//!< シミュレーション結果のリスト
};


#endif	//DESIGNLAB_RESULT_FILE_EXPORTER_H_