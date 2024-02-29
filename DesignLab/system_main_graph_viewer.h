
//! @file      system_main_graph_viewer.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#ifndef DESIGNLAB_SYSTEM_MAIN_GRAPH_VIEWER_H_
#define DESIGNLAB_SYSTEM_MAIN_GRAPH_VIEWER_H_

#include <memory>
#include <vector>

#include "application_setting_record.h"
#include "graphic_data_broker.h"
#include "graph_tree_creator.h"
#include "interface_map_creator.h"
#include "interface_system_main.h"
#include "map_state.h"
#include "stopwatch.h"


namespace designlab
{

//! @class SystemMainGraphViewer
//! @brief グラフを表示するシステムのメインクラス．
//! @details
//! この研究の手法では木構造のグラフを作成する．
//! どのようなグラフが作成されるかを確認するために，
//! このグラフを表示するプログラムを作成した．
class SystemMainGraphViewer final : public ISystemMain
{
public:
    SystemMainGraphViewer(
      std::unique_ptr<GraphTreeCreator>&& graph_tree_creator,
      std::unique_ptr<IMapCreator>&& map_creator,
      const std::shared_ptr<GraphicDataBroker>& broker_ptr,
      const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr);

    void Main() override;

private:
    //! @brief グラフを作成する．
    //! @param[in] parent 親ノード．
    //! @param[out] graph 作成したグラフ．
    void CreateGraph(const RobotStateNode parent,
                     std::vector<RobotStateNode>* graph);


    //! @brief グラフのステータスを表示する．
    //! @n 全ノード数，木の深さ，各深さごとのノード数を表示する．
    //! @param[in] graph グラフ
    void OutputGraphStatus(const std::vector<RobotStateNode>& graph) const;



    //! @brief グラフの中から1つのノードを選択する．
    //! グラフが空の場合は，初期状態のノードを返す．
    //! @param[in] graph グラフ．
    //! @return 選択されたノード．
    RobotStateNode SelectNodeByInput(const std::vector<RobotStateNode>& graph) const;


    const std::unique_ptr<GraphTreeCreator> graph_tree_creator_ptr_;

    const std::unique_ptr<IMapCreator> map_creator_ptr_;

    const std::shared_ptr<GraphicDataBroker> broker_ptr_;

    const std::shared_ptr<const ApplicationSettingRecord> setting_ptr_;

    MapState map_state_;

    Stopwatch stopwatch_;
};

}  // namespace designlab


#endif  // DESIGNLAB_SYSTEM_MAIN_GRAPH_VIEWER_H_
