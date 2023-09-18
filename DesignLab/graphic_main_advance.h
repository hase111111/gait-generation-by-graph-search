#pragma once

#include <vector>

#include "interface_graphic_main.h"
#include "map_state.h"
#include "node.h"
#include "graphic_const.h"
#include "camera_gui.h"
#include "display_node_switch_gui.h"
#include "node_display_gui.h"
#include "hexapod_renderer.h"
#include "movement_locus_renderer.h"
#include "robot_graund_point_renderer.h"
#include "stability_margin_renderer.h"
#include "interpolated_node_creator.h"




class GraphicMainAdvance final : public IGraphicMain
{
public:
	GraphicMainAdvance(const GraphicDataBroker* const  broker, std::shared_ptr<AbstractHexapodStateCalculator> calc, const SApplicationSettingRecorder* const setting);
	~GraphicMainAdvance() = default;

	bool Update() override;

	void Draw() const override;

private:

	const int kNodeGetCount;			//2秒ごとに読み出す．

	const int kInterpolatedAnimeCount = 15;


	CameraGui m_camera_gui;							// カメラの位置を制御するGUI

	NodeDisplayGui m_node_display_gui;				// ノードの表示を制御するGUI

	DisplayNodeSwitchGUI m_display_node_switch_gui;	// ノードの表示を切り替えるGUI


	HexapodRenderer m_hexapod_renderer;						//!< ロボットを表示するクラス．	

	MovementLocusRenderer m_movement_locus_renderer;		//!< ロボットの動きの軌跡を表示するクラス．

	RobotGraundPointRenderer m_robot_graund_point_renderer;	//!< ロボットの足先の位置を表示するクラス．

	StabilityMarginRenderer m_stability_margin_renderer;	//!< ロボットの安定性マージンを表示するクラス．


	InterpolatedNodeCreator m_interpolated_node_creator;	//!< ノード間を補間するクラス．


	std::vector<SNode> m_node;			//ロボットの動きの遷移を記録するvector

	int m_display_node_index = -1;		//描画しているノード

	MapState m_map_state;				//表示するマップ．

	int m_counter = 0;					//このクラスが実行されてから何回update関数が呼ばれたかカウントする．

	std::vector<SNode> m_interpolated_node;	//補間されたノードを記録するvector

	int m_interpolated_anime_start_count = -100;	//補間されたノードを表示し始めるカウント


	bool m_is_display_movement_locus = true;		//ロボットの動きの軌跡を表示するかどうか．

	bool m_is_display_robot_graund_point = true;	//ロボットの足先の位置を表示するかどうか．
};

