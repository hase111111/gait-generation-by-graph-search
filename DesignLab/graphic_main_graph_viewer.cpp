#include "graphic_main_graph_viewer.h"

#include <memory>

#include "dxlib_util.h"
#include "graph_tree_creator_hato.h"
#include "hexapod_renderer_builder.h"
#include "keyboard.h"
#include "map_renderer.h"
#include "node_initializer.h"

namespace dldu = designlab::dxlib_util;


GraphicMainGraphViewer::GraphicMainGraphViewer(
	const std::shared_ptr<const GraphicDataBroker>& broker_ptr,
	const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
	const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
	const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr,
	const std::shared_ptr<const ApplicationSettingRecorder>& setting_ptr
) :
	broker_ptr_(broker_ptr),
	camera_gui_{ 10, setting_ptr ? setting_ptr->window_size_y - 10 : 0 ,CameraGui::kOptionLeftBottom },
	node_display_gui_{ setting_ptr ? setting_ptr->window_size_x - NodeDisplayGui::kWidth - 10 : 0, 10, calculator_ptr, checker_ptr },
	map_state_(broker_ptr ? broker_ptr->map_state.GetData() : MapState{}),
	hexapod_renderer_(HexapodRendererBuilder::Build(converter_ptr, calculator_ptr, setting_ptr->gui_display_quality)),
	graph_({}),
	display_node_index_(0),
	map_update_count_(0),
	graph_update_count_(0),
	gui_controller_ptr_(std::make_unique<GraphViewerGUIController>(&graph_, &display_node_index_, setting_ptr))
{
	//適当なノードを生成して，描画クラスを初期化する
	NodeInitializer node_initializer;
	RobotStateNode init_node = node_initializer.InitNode();

	hexapod_renderer_->SetDrawNode(init_node);
}


bool GraphicMainGraphViewer::Update()
{
	gui_controller_ptr_->Update();

	//仲介人の持つデータと自身の持っているグラフデータが一致していないならば更新する
	if (map_update_count_ != broker_ptr_->map_state.GetUpdateCount())
	{
		map_state_ = broker_ptr_->map_state.GetData();
		map_update_count_ = broker_ptr_->map_state.GetUpdateCount();
	}

	if (graph_update_count_ != broker_ptr_->graph.GetUpdateCount())
	{
		graph_.clear();	//グラフを初期化する

		graph_ = broker_ptr_->graph.GetData();	//データを更新する

		//グラフの中身が空でないならば，表示するノードを初期化する
		if (!graph_.empty()) { display_node_index_ = 0; }

		gui_controller_ptr_->UpdateGraphNodeDepthData();

		graph_update_count_ = broker_ptr_->graph.GetUpdateCount();
	}

	//HexapodReanderの更新
	if (display_node_index_ < graph_.size() && graph_.size() != 0)
	{
		hexapod_renderer_->SetDrawNode(graph_.at(display_node_index_));

		camera_gui_.SetHexapodPos(graph_.at(display_node_index_).global_center_of_mass);

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

	//map_renderer.Draw(map_state_);


	if (display_node_index_ < graph_.size())
	{
		hexapod_renderer_->Draw();
	}


	gui_controller_ptr_->Draw();

	camera_gui_.Draw();

	node_display_gui_.Draw();
}
