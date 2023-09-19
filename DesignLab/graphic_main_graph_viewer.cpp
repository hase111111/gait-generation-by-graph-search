#include "graphic_main_graph_viewer.h"

#include <memory>

#include "dxlib_util.h"
#include "graph_tree_creator_hato.h"
#include "keyboard.h"
#include "map_renderer.h"

namespace dldu = designlab::dxlib_util;


GraphicMainGraphViewer::GraphicMainGraphViewer(const std::shared_ptr<const GraphicDataBroker>& broker_ptr,
	const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr, const std::shared_ptr<const SApplicationSettingRecorder>& setting_ptr) :
	broker_ptr_(broker_ptr),
	calculator_ptr_(calculator_ptr),
	camera_gui_(10, setting_ptr ? setting_ptr->window_size_y - CameraGui::GUI_SIZE_Y - 10 : 0),
	node_display_gui_(setting_ptr ? setting_ptr->window_size_x - NodeDisplayGui::kWidth - 10 : 0, 10, calculator_ptr),
	map_state_(broker_ptr ? broker_ptr->map_state() : MapState_Old()),
	hexapod_renderer_(calculator_ptr),
	graph_({}),
	display_node_index_(0)
{
	//適当なノードを生成して，描画クラスを初期化する
	SNode init_node;
	init_node.init(false);

	hexapod_renderer_.set_draw_node(init_node);

	// GUI にグラフのポインタを渡す.
	gui_controller_ptr_ = std::make_unique<GraphViewerGUIController>(&graph_, &display_node_index_, setting_ptr);
}


bool GraphicMainGraphViewer::Update()
{
	gui_controller_ptr_->Update();

	//仲介人の持つグラフデータと自身の持っているグラフデータが一致していないならば
	if (broker_ptr_->GetNodeNum() != graph_.size())
	{
		graph_.clear();	//グラフを初期化する

		broker_ptr_->CopyAllNode(&graph_);	//データを更新する

		//グラフの中身が空でないならば，表示するノードを初期化する
		if (!graph_.empty()) { display_node_index_ = 0; }

		gui_controller_ptr_->updateGraphNodeDepthData();

	}

	//HexapodReanderの更新
	if (display_node_index_ < graph_.size() && 0 != graph_.size())
	{
		hexapod_renderer_.set_draw_node(graph_.at(display_node_index_));

		camera_gui_.setHexapodPos(graph_.at(display_node_index_).global_center_of_mass);

		node_display_gui_.SetDisplayNode(graph_.at(display_node_index_));
	}

	camera_gui_.Update();

	node_display_gui_.Update();

	return true;
}


void GraphicMainGraphViewer::Draw() const
{
	dldu::SetZBufferEnable();


	MapRenderer map_renderer;

	map_renderer.Draw(map_state_);


	if (display_node_index_ < graph_.size())
	{
		hexapod_renderer_.Draw();
	}


	gui_controller_ptr_->Draw();

	camera_gui_.Draw();

	node_display_gui_.Draw();
}
