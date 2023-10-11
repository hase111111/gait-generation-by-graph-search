//! @file graph_viewer_gui_controller.h
//! @brief グラフのノードのデータを表示するGUIのコントローラークラス

#ifndef DESIGNLAB_GRAPH_VIEWER_GUI_CONTROLLER_H_
#define DESIGNLAB_GRAPH_VIEWER_GUI_CONTROLLER_H_

#include <memory>
#include <vector>

#include "application_setting_recorder.h"
#include "robot_state_node.h"


//! @class GraphViewerGUIController
//! @brief グラフのノードのデータを表示するGUIのコントローラークラス
class GraphViewerGUIController final
{
public:
	GraphViewerGUIController(const std::vector<RobotStateNode>* const graph_ptr, size_t* const display_node_index_ptr,
		const std::shared_ptr<const ApplicationSettingRecorder>& setting_ptr);
	~GraphViewerGUIController() = default;

	void Update();

	void Draw() const;

	void updateGraphNodeDepthData();

private:

	void drawGraphData() const;
	void drawNodeControllPanel() const;
	void drawNodeData(const RobotStateNode& node) const;

	void inputNumber();
	void changeDisplayNodeIndex();
	void updateChildrenList();

	const std::vector<RobotStateNode>* const mp_graph;

	const std::shared_ptr<const ApplicationSettingRecorder> setting_ptr_;


	size_t* const mp_display_node_index;
	std::pair<int, std::vector<int>> m_childen_list = std::make_pair<int, std::vector<int>>(-1, {});	//子ノードのリスト
	int m_display_children_list_index = 0;	//表示する子ノードのリストのインデックス

	std::vector<int> m_graph_node_depth_data;	//各深さごとのノード数のデータ
	int m_input_number = -1;	//入力された数値
};


#endif // !DESIGNLAB_GRAPH_VIEWER_GUI_CONTROLLER_H_