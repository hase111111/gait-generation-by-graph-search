
//! @file      result_file_exporter.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#ifndef DESIGNLAB_RESULT_FILE_EXPORTER_H_
#define DESIGNLAB_RESULT_FILE_EXPORTER_H_

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "interface_hexapod_joint_calculator.h"
#include "robot_state_node.h"
#include "simulation_result_record.h"


namespace designlab
{

class ResultFileConst final
{
public:
    //! 出力先ディレクトリ(フォルダ)名．
    static const std::string kDirectoryPath;

    //! 出力先ディレクトリ(フォルダ)名．
    static const std::string kLegDirectoryName;

    //! シミュレーションの成功回数をまとめたファイルの名前．
    static const std::string kSuccessfulCount;

    //! ファイル名 ( 人間が見る用 )
    static const std::string kDetailFileName;

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
    ResultFileExporter(
        const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr);

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

    //! @brief 各脚ごとの脚先座標をファイルに出力する．
    //! @param[in] path 出力先のファイルパス．
    void ExportEachLegPos(const std::string& path) const;

    //! @brief 全脚の脚先座標をまとめてファイルに出力する．
    void ExportAllLegPos(const std::string& path) const;

    //! @brief 成功したシミュレーションのみの脚先座標をファイルに出力する．
    //! @param[in] path 出力先のファイルパス．
    void ExportEachLegPosAllSuccessfulSimulation(const std::string& path) const;

    void ExportAllLegPosAllSuccessfulSimulation(const std::string& path) const;

    std::string GetHeader() const;

    std::string GetLegChangeStatus(
        const std::optional<Vector3>& past, const Vector3& current) const;

    //! シミュレーション結果のリスト．
    std::vector<SimulationResultRecord> result_list_;

    //! 計算機のポインタ．
    const std::shared_ptr<const IHexapodJointCalculator> calculator_ptr_;
};

}  // namespace designlab


#endif  // DESIGNLAB_RESULT_FILE_EXPORTER_H_
