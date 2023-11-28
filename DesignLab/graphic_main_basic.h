#ifndef DESIGNLAB_GRAPHIC_MAIN_BASIC_H_
#define DESIGNLAB_GRAPHIC_MAIN_BASIC_H_


#include "interface_graphic_main.h"

#include <memory>
#include <vector>

#include "application_setting_recorder.h"
#include "camera_gui.h"
#include "display_node_switch_gui.h"
#include "gui_updater.h"
#include "graphic_const.h"
#include "graphic_data_broker.h"
#include "interface_hexapod_renderer.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_joint_calculator.h"
#include "interface_hexapod_vaild_checker.h"
#include "interpolated_node_creator.h"
#include "map_state.h"
#include "map_renderer.h"
#include "movement_locus_renderer.h"
#include "mouse.h"
#include "robot_state_node.h"
#include "node_display_gui.h"
#include "robot_graund_point_renderer.h"
#include "stability_margin_renderer.h"


class GraphicMainBasic final : public IGraphicMain
{
public:
	GraphicMainBasic(
		const std::shared_ptr<const GraphicDataBroker>& broker_ptr,
		const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
		const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
		const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr,
		const std::shared_ptr<const ApplicationSettingRecorder>& setting_ptr
	);
	~GraphicMainBasic() = default;

	bool Update() override;

	void Draw() const override;

private:

	const int kNodeGetCount;			//!< ノードを取得する間隔．

	const int kInterpolatedAnimeCount;	//!< 補間されたノードの表示を切り替える間隔．


	const std::shared_ptr<const GraphicDataBroker> broker_ptr_;		//!< データを受け取るクラス．


	std::shared_ptr<Mouse> mouse_ptr_;	//!< マウスの情報を取得するクラス．


	GuiUpdater gui_activator_;					//!< GUIをアクティブにするクラス．
	std::shared_ptr<DxlibCamera> camera_;			//!< カメラの管理を行うクラス．
	std::shared_ptr<CameraGui> camera_gui_;							//!< カメラの位置を制御するGUI
	std::shared_ptr<DisplayNodeSwitchGui> display_node_switch_gui_;	//!< ノードの表示を切り替えるGUI
	std::shared_ptr<NodeDisplayGui> node_display_gui_;				//!< ノードの表示を制御するGUI


	const std::unique_ptr<IHexapodRenderer> hexapod_renderer_;	//!< ロボットを表示するクラス．	

	MovementLocusRenderer movement_locus_renderer_;			//!< ロボットの動きの軌跡を表示するクラス．

	RobotGraundPointRenderer robot_graund_point_renderer_;	//!< ロボットの足先の位置を表示するクラス．

	StabilityMarginRenderer stability_margin_renderer_;		//!< ロボットの安定性マージンを表示するクラス．

	MapRenderer map_renderer_;								//!< マップを表示するクラス．

	InterpolatedNodeCreator interpolated_node_creator_;		//!< ノード間を補間するクラス．


	MapState map_state_;		//!< 表示するマップ．

	std::vector<RobotStateNode> graph_;	//!< ロボットの動きの遷移を記録するvector

	size_t display_node_index_;	//!< 描画しているノード

	int counter_;				//!< このクラスが実行されてから何回update関数が呼ばれたかカウントする．

	std::vector<RobotStateNode> interpolated_node_;	//!< 補間されたノードを記録するvector

	int interpolated_anime_start_count_;	//!< 補間されたノードを表示し始めるカウント


	bool is_displayed_movement_locus_;		//!< ロボットの動きの軌跡を表示するかどうか．

	bool is_displayed_robot_graund_point_;	//!< ロボットの足先の位置を表示するかどうか．


	int graph_update_count;	//!< 処理部でのグラフの更新回数．

	int map_update_count;	//!< 処理部でのマップの更新回数．
};


#endif // DESIGNLAB_GRAPHIC_MAIN_BASIC_H_