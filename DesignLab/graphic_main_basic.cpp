#include "graphic_main_basic.h"

#include <Dxlib.h>

#include "dxlib_util.h"
#include "keyboard.h"
#include "map_renderer.h"
#include "world_grid_renderer.h"


GraphicMainBasic::GraphicMainBasic(const std::shared_ptr<const GraphicDataBroker>& broker_ptr, const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr,
	const std::shared_ptr<const ApplicationSettingRecorder>& setting_ptr) :
	kNodeGetCount(setting_ptr ? setting_ptr->window_fps * 2 : 60),
	broker_ptr_(broker_ptr),
	node_display_gui_(setting_ptr ? setting_ptr->window_size_x - NodeDisplayGui::kWidth - 10 : 0, 10, calculator_ptr),
	display_node_switch_gui_(10, setting_ptr ? setting_ptr->window_size_y - DisplayNodeSwitchGui::GUI_HEIGHT - 10 : 0),
	hexapod_renderer_(calculator_ptr),
	robot_graund_point_renderer_(calculator_ptr),
	stability_margin_renderer_(calculator_ptr),
	map_state_(broker_ptr_ ? broker_ptr_->map_state.GetData() : MapState()),
	graph_({}),
	display_node_index_(0),
	counter_(0),
	is_displayed_movement_locus_(false),
	is_displayed_robot_graund_point_(false)
{
}


bool GraphicMainBasic::Update()
{
	if (map_update_count != broker_ptr_->map_state.GetUpdateCount())
	{
		map_update_count = broker_ptr_->map_state.GetUpdateCount();
		map_state_ = broker_ptr_->map_state.GetData();
	}

	//ノードを読み出す時間になったら，仲介人からデータを読み出す．
	if (counter_ % kNodeGetCount == 0 && graph_update_count != broker_ptr_->graph.GetUpdateCount())
	{
		//仲介人からデータを読み出す
		graph_ = broker_ptr_->graph.GetData();

		std::vector<size_t> simu_end_index;

		simu_end_index = broker_ptr_->simu_end_index.GetData();

		//ノードの情報を表示するGUIに情報を伝達する．
		display_node_switch_gui_.setGraphData(graph_.size(), simu_end_index);



		//移動軌跡を更新する．
		movement_locus_renderer_.SetMoveLocusPoint(graph_);

		movement_locus_renderer_.SetSimulationEndIndexes(simu_end_index);


		//ロボットの接地点を更新する．
		robot_graund_point_renderer_.SetNodeAndSimulationEndNodeIndex(graph_, simu_end_index);


		graph_update_count = broker_ptr_->graph.GetUpdateCount();
	}


	//ノードが存在しているのならば，各クラスに情報を伝達する
	if (!graph_.empty())
	{
		// 表示ノードが変更されたら，表示するノードを変更する．
		if (display_node_index_ != display_node_switch_gui_.getDisplayNodeNum())
		{
			display_node_index_ = display_node_switch_gui_.getDisplayNodeNum();	//表示するノードを取得する．

			hexapod_renderer_.SetDrawNode(graph_.at(display_node_index_));					//ロボットの状態を更新する．

			camera_gui_.SetHexapodPos(graph_.at(display_node_index_).global_center_of_mass);		//カメラの位置を更新する．

			node_display_gui_.SetDisplayNode(graph_.at(display_node_index_));			//ノードの情報を表示するGUIに情報を伝達する．
		}
	}


	++counter_;				//カウンタを進める．

	camera_gui_.Update();				//カメラのGUIを更新する．

	node_display_gui_.Update();			//ノードの情報を表示するGUIを更新する．

	display_node_switch_gui_.Update();	//ノードの情報を表示するGUIを更新する．


	// キー入力で表示を切り替える
	// TODO : あとでGUIに移行する
	if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_L) == 1)
	{
		is_displayed_movement_locus_ = !is_displayed_movement_locus_;
	}
	else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_G) == 1)
	{
		is_displayed_robot_graund_point_ = !is_displayed_robot_graund_point_;
	}


	return true;
}


void GraphicMainBasic::Draw() const
{
	// 3Dのオブジェクトの描画

	designlab::dxlib_util::SetZBufferEnable();		//Zバッファを有効にする．


	WorldGridRenderer grid_renderer;	//インスタンスを生成する．

	grid_renderer.Draw();				//グリッドを描画する．


	MapRenderer map_render;				//マップを描画する．

	map_render.Draw(map_state_);


	if (is_displayed_movement_locus_)
	{
		movement_locus_renderer_.Draw(display_node_switch_gui_.getSimulationNum());   //移動軌跡を描画する．
	}

	if (is_displayed_robot_graund_point_)
	{
		robot_graund_point_renderer_.Draw(display_node_switch_gui_.getSimulationNum());
	}


	if (!graph_.empty())
	{
		//ノードが存在しているならば，ロボットを描画する．
		hexapod_renderer_.Draw();

		stability_margin_renderer_.Draw(graph_.at(display_node_index_));
	}


	// 2DのGUIの描画

	camera_gui_.Draw();        //カメラのGUIを描画する．

	node_display_gui_.Draw();		//ノードの情報を表示するGUIを描画する．

	display_node_switch_gui_.Draw();	//表示するノードを切り替えるGUIを描画する．
}
