
//! @file      result_file_exporter.h
//! @author    hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_RESULT_FILE_EXPORTER_H_
#define DESIGNLAB_RESULT_FILE_EXPORTER_H_

#include <fstream>
#include <string>
#include <vector>

#include "robot_state_node.h"
#include "simulation_result_record.h"


namespace designlab
{

class ResultFileConst final
{
public:
    static const std::string kDirectoryPath;    //!< 出力先ディレクトリ(フォルダ)名．

    static const std::string kFileName;         //!< ファイル名 ( 人間が見る用 )

    static const std::string kDetailFileName;   //!< ファイル名 ( プログラムの読み込み用 )

    //! ノードリストのファイル名 (プログラムの読み込み用)
    static const std::string kNodeListName;

    //! マップ状態のファイル名 (プログラムの読み込み用)
    static const std::string kMapStateName;
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
    //! @param[in] simulation_result シミュレーションの結果
    void PushSimulationResult(const SimulationResultRecord& simulation_result);

    //! @brief 出力を行うかどうかのフラグを設定する．
    //! @param[in] do_export 出力を行うかどうかのフラグ
    inline void SetDoExport(const bool do_export) { do_export_ = do_export; }


    //! @brief 最新のノードリストをファイルに出力する．result_viewerで使用する．
    //! @n 初期化ができていない場合は，なにも出力しない．
    //! また，出力フラグが false の場合もなにも出力しない．
    //! @n Init()が呼ばれていない場合は，なにも出力しないので，
    //! 必ずInit()を呼び出してから呼び出すこと．
    void ExportLatestNodeList() const;

    //! @brief 最新のマップ状態をファイルに出力する．result_viewerで使用する．
    //! @n 初期化ができていない場合は，なにも出力しない．
    //! また，出力フラグが false の場合もなにも出力しない．
    //! @n Init()が呼ばれていない場合は，なにも出力しないので，
    //! 必ずInit()を呼び出してから呼び出すこと．
    void ExportLatestMapState() const;

    //! @brief シミュレーション結果をファイルに出力する．人間が見る用．
    //! @n 初期化ができていない場合は，なにも出力しない．
    //! また，出力フラグが false の場合もなにも出力しない．
    //! @n Init()が呼ばれていない場合は，なにも出力しないので，
    //! 必ずInit()を呼び出してから呼び出すこと．
    void ExportResult() const;

    //! @brief 全てのシミュレーション結果をまとめてファイルに出力する．人間が見る用．
    //! @n 初期化ができていない場合は，なにも出力しない．
    //! また，出力フラグが false の場合もなにも出力しない．
    //! @n Init()が呼ばれていない場合は，なにも出力しないので，
    //! 必ずInit()を呼び出してから呼び出すこと．
    void ExportAllResultDetail() const;

private:
    //! @brief シミュレーション結果をファイルに出力する．
    //! @param[in] recorder シミュレーション結果．
    //! @param[in] simulation_index シミュレーション番号．
    //! @return 出力に成功したか．
    bool OutputResultDetail(const SimulationResultRecord& recorder, int simulation_index) const;


    std::string folder_name_;  //!< 出力先フォルダ名．

    bool init_success_;  //!< 初期化成功フラグ．

    bool do_export_;  //!< 出力を行うかどうかのフラグ．


    std::vector<SimulationResultRecord> result_list_;  //!< シミュレーション結果のリスト．
};

}  // namespace designlab


#endif  // DESIGNLAB_RESULT_FILE_EXPORTER_H_
