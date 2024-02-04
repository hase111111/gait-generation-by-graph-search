
//! @file      result_file_exporter.h
//! @author    Hasegawa
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

    static const std::string kLegDirectoryName;  //!< 出力先ディレクトリ(フォルダ)名．

    static const std::string kSuccessfulCount;  //!< シミュレーションの成功回数をまとめたファイルの名前．

    static const std::string kDetailFileName;  //!< ファイル名 ( 人間が見る用 )

    //! ノードリストのファイル名 (プログラムの読み込み用)
    static const std::string kNodeListName;

    //! マップ状態のファイル名 (プログラムの読み込み用)
    static const std::string kMapStateName;
};


//! @class ResultFileExporter
//! @brief 結果をファイルに出力するクラス．
//! @n シミュレーション結果をファイルに出力する処理を全てこのクラスにまとめた．
class ResultFileExporter final
{
public:
    ResultFileExporter() = default;

    //! @brief resultフォルダがなければ作成する．また，フォルダ名を指定する．
    void CreateRootDirectory();

    //! @brief シミュレーション結果を追加する．
    //! @param[in] simulation_result シミュレーションの結果
    void PushSimulationResult(const SimulationResultRecord& simulation_result);

    //! @brief 結果をファイルに出力する．
    void Export() const;

private:
    //! @brief 出力先ディレクトリの名前を受け付け，そのディレクトリを作成する．
    //! @return 作成したディレクトリのパス．
    std::string MakeOutputDirectory() const;

    //! @brief ノードリストをファイルに出力する．
    //! @param[in] path 出力先のファイルパス．
    void ExportEachNodeList(const std::string& path) const;

    //! @brief マップ状態をファイルに出力する．
    //! @param[in] path 出力先のファイルパス．
    void ExportEachMapState(const std::string& path) const;

    //! @brief シミュレーション結果をファイルに出力する．
    //! @param[in] path 出力先のファイルパス．
    void ExportEachSimulationDetail(const std::string& path) const;

    //! @brief 全てのシミュレーション結果をまとめてファイルに出力する．人間が見る用．
    //! @n 初期化ができていない場合は，なにも出力しない．
    //! また，出力フラグが false の場合もなにも出力しない．
    //! @n Init()が呼ばれていない場合は，なにも出力しないので，
    //! 必ずInit()を呼び出してから呼び出すこと．
    void ExportSuccessfulCount(const std::string& path) const;

    void ExportEachLegPos(const std::string& path) const;

    std::vector<SimulationResultRecord> result_list_;  //!< シミュレーション結果のリスト．
};

}  // namespace designlab


#endif  // DESIGNLAB_RESULT_FILE_EXPORTER_H_
