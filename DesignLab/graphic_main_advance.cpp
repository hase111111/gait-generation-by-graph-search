#include "graphic_main_advance.h"

#include <Dxlib.h>

#include "dxlib_util.h"
#include "keyboard.h"
#include "map_renderer.h"
#include "world_grid_renderer.h"


GraphicMainAdvance::GraphicMainAdvance(const std::shared_ptr<const GraphicDataBroker>& broker_ptr, const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr,
	const std::shared_ptr<const SApplicationSettingRecorder>& setting_ptr) :
	kNodeGetCount(setting_ptr ? setting_ptr->window_fps * 2 : 60),
	kInterpolatedAnimeCount(15),
	broker_ptr_(broker_ptr),
	node_display_gui_(setting_ptr ? setting_ptr->window_size_x - NodeDisplayGui::kWidth - 10 : 10, 10, calculator_ptr),
	display_node_switch_gui_(10, setting_ptr ? setting_ptr->window_size_y - DisplayNodeSwitchGUI::GUI_HEIGHT - 10 : 10),
	hexapod_renderer_(calculator_ptr),
	map_state_(broker_ptr ? broker_ptr->map_state.data() : MapState()),
	graph_({}),
	display_node_index_(0),
	counter_(0),
	interpolated_anime_start_count_(kInterpolatedAnimeCount * -10),
	is_displayed_movement_locus_(true),
	is_displayed_robot_graund_point_(true)
{
}


bool GraphicMainAdvance::Update()
{
	if (map_update_count != broker_ptr_->map_state.update_count())
	{
		map_update_count = broker_ptr_->map_state.update_count();
		map_state_ = broker_ptr_->map_state.data();
	}


	//ノードを読み出す時間になったら，仲介人からデータを読み出す．
	if (counter_ % kNodeGetCount == 0)
	{
		//仲介人からデータを読み出す
		graph_ = broker_ptr_->graph.data();

		std::vector<size_t> simu_end_index;

		simu_end_index = broker_ptr_->simu_end_index.data();

		//ノードの情報を表示するGUIに情報を伝達する．
		display_node_switch_gui_.setGraphData(graph_.size(), simu_end_index);


		//ノードの情報を表示するGUIに情報を伝達する．
		display_node_switch_gui_.setGraphData(graph_.size(), simu_end_index);



		//移動軌跡を更新する．
		movement_locus_renderer_.set_move_locus_point(graph_);

		movement_locus_renderer_.set_simulation_end_indexes(simu_end_index);


		//ロボットの接地点を更新する．
		robot_graund_point_renderer_.SetNodeAndSimulationEndNodeIndex(graph_, simu_end_index);


		graph_update_count = broker_ptr_->graph.update_count();
	}


	//ノードが存在しているのならば，各クラスに情報を伝達する
	if (!graph_.empty())
	{
		// 表示ノードが変更されたら，表示するノードを変更する．
		if (display_node_index_ != display_node_switch_gui_.getDisplayNodeNum())
		{
			if (display_node_index_ > 0)
			{
				interpolated_anime_start_count_ = counter_;		//アニメーションを開始した時間を記録する．
				interpolated_node_creator_.createInterpolatedNode(graph_[display_node_index_], graph_[display_node_switch_gui_.getDisplayNodeNum()], &interpolated_node_);
			}


			display_node_index_ = display_node_switch_gui_.getDisplayNodeNum();				//表示するノードを取得する．

			hexapod_renderer_.set_draw_node(graph_.at(display_node_index_));							//ロボットの状態を更新する．

			camera_gui_.setHexapodPos(graph_.at(display_node_index_).global_center_of_mass);		//カメラの位置を更新する．

			node_display_gui_.SetDisplayNode(graph_.at(display_node_index_));						//ノードの情報を表示するGUIに情報を伝達する．
		}

		if (interpolated_anime_start_count_ <= counter_ && counter_ < interpolated_anime_start_count_ + kInterpolatedAnimeCount)
		{
			//アニメーション中は interpolated_node_ の補完されたノードを表示する
			size_t anime_index = interpolated_node_.size() * (static_cast<size_t>(counter_) - static_cast<size_t>(interpolated_anime_start_count_))
				/ static_cast<size_t>(kInterpolatedAnimeCount);

			hexapod_renderer_.set_draw_node(interpolated_node_[anime_index]);

			node_display_gui_.SetDisplayNode(interpolated_node_[anime_index]);
		}
		else if (counter_ == interpolated_anime_start_count_ + kInterpolatedAnimeCount)
		{
			//アニメーションが終了したら，元のノードを表示する
			hexapod_renderer_.set_draw_node(graph_.at(display_node_index_));

			node_display_gui_.SetDisplayNode(graph_.at(display_node_index_));
		}
	}


	counter_++;				//カウンタを進める．

	camera_gui_.Update();				//カメラのGUIを更新する．

	node_display_gui_.Update();			//ノードの情報を表示するGUIを更新する．

	display_node_switch_gui_.Update();	//ノードの情報を表示するGUIを更新する．


	//キー入力で表示を切り替える
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


void GraphicMainAdvance::Draw() const
{
	// 3Dのオブジェクトの描画

	designlab::dxlib_util::SetZBufferEnable();		//Zバッファを有効にする．


	WorldGridRenderer grid_renderer;	//インスタンスを生成する．

	grid_renderer.Draw();				//グリッドを描画する．


	MapRenderer map_render;				//マップを描画する．

	map_render.Draw(map_state_);


	if (is_displayed_movement_locus_)movement_locus_renderer_.Draw(display_node_switch_gui_.getSimulationNum());   //移動軌跡を描画する．

	if (is_displayed_robot_graund_point_)robot_graund_point_renderer_.Draw(display_node_switch_gui_.getSimulationNum());


	if (!graph_.empty())
	{
		//ノードが存在しているならば，ロボットを描画する．
		hexapod_renderer_.Draw();

		if (counter_ > interpolated_anime_start_count_ + kInterpolatedAnimeCount)
		{
			stability_margin_renderer_.Draw(graph_.at(display_node_index_));
		}
	}


	// 2DのGUIの描画

	camera_gui_.Draw();        //カメラのGUIを描画する．

	node_display_gui_.Draw();	 //ノードの情報を表示するGUIを描画する．

	display_node_switch_gui_.Draw();	//表示するノードを切り替えるGUIを描画する．
}
