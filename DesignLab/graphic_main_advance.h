#ifndef DESIGNLAB_GRAPHIC_MAIN_ADVANCE_H_
#define DESIGNLAB_GRAPHIC_MAIN_ADVANCE_H_


#include "interface_graphic_main.h"

#include <memory>
#include <vector>

#include "abstract_hexapod_state_calculator.h"
#include "application_setting_recorder.h"
#include "camera_gui.h"
#include "display_node_switch_gui.h"
#include "graphic_const.h"
#include "graphic_data_broker.h"
#include "hexapod_renderer.h"
#include "interpolated_node_creator.h"
#include "map_state.h"
#include "movement_locus_renderer.h"
#include "node.h"
#include "node_display_gui.h"
#include "robot_graund_point_renderer.h"
#include "stability_margin_renderer.h"


class GraphicMainAdvance final : public IGraphicMain
{
public:
	GraphicMainAdvance(const std::shared_ptr<const GraphicDataBroker>& broker_ptr, const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr,
		const std::shared_ptr<const SApplicationSettingRecorder>& setting_ptr);
	~GraphicMainAdvance() = default;

	bool Update() override;

	void Draw() const override;

private:

	const int kNodeGetCount;			//!< ノードを取得する間隔．

	const int kInterpolatedAnimeCount;	//!< 補間されたノードの表示を切り替える間隔．


	const std::shared_ptr<const GraphicDataBroker> broker_ptr_;		//!< データを受け取るクラス．


	CameraGui camera_gui_;							//!< カメラの位置を制御するGUI

	DisplayNodeSwitchGUI display_node_switch_gui_;	//!< ノードの表示を切り替えるGUI

	NodeDisplayGui node_display_gui_;				//!< ノードの表示を制御するGUI


	HexapodRenderer hexapod_renderer_;						//!< ロボットを表示するクラス．	

	MovementLocusRenderer movement_locus_renderer_;			//!< ロボットの動きの軌跡を表示するクラス．

	RobotGraundPointRenderer robot_graund_point_renderer_;	//!< ロボットの足先の位置を表示するクラス．

	StabilityMarginRenderer stability_margin_renderer_;		//!< ロボットの安定性マージンを表示するクラス．

	InterpolatedNodeCreator interpolated_node_creator_;		//!< ノード間を補間するクラス．


	MapState map_state_;		//!< 表示するマップ．

	std::vector<SNode> graph_;	//!< ロボットの動きの遷移を記録するvector

	size_t display_node_index_;	//!< 描画しているノード

	int counter_;				//!< このクラスが実行されてから何回update関数が呼ばれたかカウントする．

	std::vector<SNode> interpolated_node_;	//!< 補間されたノードを記録するvector

	int interpolated_anime_start_count_;	//!< 補間されたノードを表示し始めるカウント


	bool is_displayed_movement_locus_;		//!< ロボットの動きの軌跡を表示するかどうか．

	bool is_displayed_robot_graund_point_;	//!< ロボットの足先の位置を表示するかどうか．


	int graph_update_count;	//!< 処理部でのグラフの更新回数．

	int map_update_count;	//!< 処理部でのマップの更新回数．
};


#endif // !DESIGNLAB_GRAPHIC_MAIN_ADVANCE_H_