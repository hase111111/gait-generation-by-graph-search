
//! @file      graph_viewer_gui_controller.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_GRAPH_VIEWER_GUI_CONTROLLER_H_
#define DESIGNLAB_GRAPH_VIEWER_GUI_CONTROLLER_H_

#include <memory>
#include <utility>
#include <vector>

#include "application_setting_record.h"
#include "robot_state_node.h"

//! @todo 仮置きのはずが，本実装になっているので，後で直す

namespace designlab
{

//! @class GraphViewerGUIController
//! @brief グラフのノードのデータを表示するGUIのコントローラークラス．
class GraphViewerGUIController final
{
public:
    GraphViewerGUIController(
        const std::vector<RobotStateNode>* const graph_ptr,
        size_t* const display_node_index_ptr,
        const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr);

    ~GraphViewerGUIController() = default;

    void Update();

    void Draw() const;

    void UpdateGraphNodeDepthData();

private:
    void DrawGraphData() const;
    void DrawNodeControlPanel() const;
    void DrawNodeData(const RobotStateNode& node) const;

    void InputNumber();
    void ChangeDisplayNodeIndex();
    void UpdateChildrenList();

    const std::vector<RobotStateNode>* const graph_ptr_;

    const std::shared_ptr<const ApplicationSettingRecord> setting_ptr_;


    size_t* const display_node_index_ptr_;

    //! 子ノードのリスト．
    std::pair<int, std::vector<int>> children_list_ = { -1, {} };
    int display_children_list_index_ = 0;  //!< 表示する子ノードのリストのインデックス．

    std::vector<int> graph_node_depth_data_;  //!< 各深さごとのノード数のデータ．
    int input_number_ = -1;  //!< 入力された数値．
};

}  // namespace designlab


#endif  // DESIGNLAB_GRAPH_VIEWER_GUI_CONTROLLER_H_
