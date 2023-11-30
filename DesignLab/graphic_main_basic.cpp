#include "graphic_main_basic.h"

#include <Dxlib.h>

#include "camera_dragger.h"
#include "camera_gui.h"
#include "dxlib_util.h"
#include "hexapod_renderer_builder.h"
#include "keyboard.h"
#include "node_display_gui.h"
#include "map_renderer.h"
#include "world_grid_renderer.h"


GraphicMainBasic::GraphicMainBasic(
	const std::shared_ptr<const GraphicDataBroker>& broker_ptr,
	const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
	const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
	const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr,
	const std::shared_ptr<const ApplicationSettingRecorder>& setting_ptr
) :
	kNodeGetCount(setting_ptr ? setting_ptr->window_fps * 2 : 60),
	kInterpolatedAnimeCount(30),
	broker_ptr_(broker_ptr),
	mouse_ptr_(std::make_shared<Mouse>()),
	display_node_switch_gui_(std::make_shared<DxlibGuiDisplayNodeSwitcher>()),
	map_renderer_ptr_(std::make_shared<MapRenderer>()),
	movement_locus_renderer_{},
	interpolated_node_creator_{ converter_ptr },
	robot_graund_point_renderer_{ converter_ptr },
	map_state_(broker_ptr ? broker_ptr->map_state.GetData() : MapState()),
	graph_({}),
	display_node_index_(0),
	counter_(0),
	interpolated_anime_start_count_(kInterpolatedAnimeCount * -10)
{
	if (setting_ptr->gui_display_quality == DisplayQuality::kHigh)
	{
		movement_locus_renderer_.SetIsHighQuality(true);
	}

	const auto camera = std::make_shared<DxlibCamera>();
	const auto camera_gui = std::make_shared<CameraGui>(camera);
	display_node_switch_gui_->SetPos(10, setting_ptr ? setting_ptr->window_size_y - 10 : 10, designlab::kOptionLeftBottom);
	const auto node_display_gui = std::make_shared<NodeDisplayGui>(converter_ptr, calculator_ptr, checker_ptr);
	node_display_gui->SetPos(setting_ptr ? setting_ptr->window_size_x - 10 : 10, 10, designlab::kOptionRightTop);
	const auto camera_dragger = std::make_shared<CameraDragger>(camera);

	const auto [hexapod_renderer, hexapod_node_setter] =
		HexapodRendererBuilder::Build(converter_ptr, calculator_ptr, setting_ptr ? setting_ptr->gui_display_quality : DisplayQuality::kMedium);
	const auto stability_margin_renderer = std::make_shared<StabilityMarginRenderer>(converter_ptr);
	const auto world_grid_renderer = std::make_shared<WorldGridRenderer>();

	gui_activator_.Register(static_cast<std::shared_ptr<IDxlibGui>>(display_node_switch_gui_), 1);
	gui_activator_.Register(static_cast<std::shared_ptr<IDxlibGui>>(camera_gui), 1);
	gui_activator_.Register(static_cast<std::shared_ptr<IDxlibGui>>(node_display_gui), 1);
	gui_activator_.Register(static_cast<std::shared_ptr<IDxlibDraggable>>(camera_dragger), 0);

	render_group_.Register(map_renderer_ptr_);
	render_group_.Register(hexapod_renderer);
	render_group_.Register(stability_margin_renderer);
	render_group_.Register(world_grid_renderer);

	node_setter_group_.Register(map_renderer_ptr_);
	node_setter_group_.Register(camera_gui);
	node_setter_group_.Register(hexapod_node_setter);
	node_setter_group_.Register(node_display_gui);
	node_setter_group_.Register(stability_margin_renderer);
}


bool GraphicMainBasic::Update()
{
	mouse_ptr_->Update();

	if (map_update_count != broker_ptr_->map_state.GetUpdateCount())
	{
		map_update_count = broker_ptr_->map_state.GetUpdateCount();
		map_state_ = broker_ptr_->map_state.GetData();

		map_renderer_ptr_->SetMapState(map_state_);
	}


	//ノードを読み出す時間になったら，仲介人からデータを読み出す．
	if (counter_ % kNodeGetCount == 0 && graph_update_count != broker_ptr_->graph.GetUpdateCount())
	{
		//仲介人からデータを読み出す
		graph_ = broker_ptr_->graph.GetData();

		std::vector<size_t> simu_end_index;

		simu_end_index = broker_ptr_->simu_end_index.GetData();

		//ノードの情報を表示するGUIに情報を伝達する．
		display_node_switch_gui_->SetGraphData(graph_.size(), simu_end_index);


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
		// 表示ノードが更新されたら，表示するノードを変更する．
		if (display_node_index_ != display_node_switch_gui_->GetDisplayNodeNum())
		{
			if (display_node_index_ > 0)
			{
				interpolated_anime_start_count_ = counter_;		//アニメーションを開始した時間を記録する．

				interpolated_node_.clear();

				interpolated_node_ = interpolated_node_creator_.CreateInterpolatedNode(
					graph_[display_node_index_],
					graph_[display_node_switch_gui_->GetDisplayNodeNum()]
				);
			}


			display_node_index_ = display_node_switch_gui_->GetDisplayNodeNum();					//表示するノードを取得する．

			node_setter_group_.SetNode(graph_.at(display_node_index_));						//ノードの情報を設定するGUIに情報を伝達する．
		}

		if (interpolated_anime_start_count_ <= counter_ && counter_ < interpolated_anime_start_count_ + kInterpolatedAnimeCount)
		{
			//アニメーション中は interpolated_node_ の補完されたノードを表示する
			const size_t anime_index = interpolated_node_.size() * (static_cast<size_t>(counter_) - static_cast<size_t>(interpolated_anime_start_count_))
				/ static_cast<size_t>(kInterpolatedAnimeCount);

			node_setter_group_.SetNode(interpolated_node_[anime_index]);
		}
		else if (counter_ == interpolated_anime_start_count_ + kInterpolatedAnimeCount)
		{
			node_setter_group_.SetNode(graph_.at(display_node_index_));						//ノードの情報を設定するGUIに情報を伝達する．
		}
	}

	counter_++;				//カウンタを進める．

	gui_activator_.Activate(mouse_ptr_);	//GUIをアクティブにする．

	return true;
}


void GraphicMainBasic::Draw() const
{
	render_group_.Draw();

	movement_locus_renderer_.Draw(display_node_switch_gui_->GetSimulationNum());   //移動軌跡を描画する．

	robot_graund_point_renderer_.Draw(display_node_switch_gui_->GetSimulationNum());

	// 2DのGUIの描画
	gui_activator_.Draw();
}