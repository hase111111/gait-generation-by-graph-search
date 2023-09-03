#pragma once

#include <vector>

#include "abstract_graphic_main.h"
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



//! @class GraphicMainBasic
//! @date 2023/08/09
//! @author 長谷川
//! @brief このプロジェクトにおける標準的なロボットの描画機能を持つクラス．
//! @details 波東さんのプログラムのロボット表示機能を書き直したもの．
//! 基本的な処理の内容は変化していないが，より表示するデータの内容が詳しくなっている．
//! また，UIによってランタイムで表示方法を制御することができるようになったため，よりロボットの状態を理解しやすくなっている．ﾀﾌﾞﾝﾈ
//! @note 処理を大きく書き換えたい場合はそもそも新しいクラスを書くようにするとよいと思う．
//! @n GraphicSampleを参考にして，作成するようにすると楽．
class GraphicMainBasic final : public AbstractGraphicMain
{
public:
	GraphicMainBasic(const GraphicDataBroker* const  broker, std::shared_ptr<AbstractHexapodStateCalculator> calc, const SApplicationSettingRecorder* const setting);
	~GraphicMainBasic() = default;

	bool update() override;

	void draw() const override;

private:

	CameraGUI m_camera_gui;							// カメラの位置を制御するGUI

	NodeDisplayGUI m_node_display_gui;				// ノードの表示を制御するGUI

	DisplayNodeSwitchGUI m_display_node_switch_gui;	// ノードの表示を切り替えるGUI


	HexapodRenderer m_hexapod_renderer;						//!< ロボットを表示するクラス．	

	MovementLocusRenderer m_movement_locus_renderer;		//!< ロボットの動きの軌跡を表示するクラス．

	RobotGraundPointRenderer m_robot_graund_point_renderer;	//!< ロボットの足先の位置を表示するクラス．

	StabilityMarginRenderer m_stability_margin_renderer;	//!< ロボットの安定性マージンを表示するクラス．


	std::vector<SNode> m_node;			//ロボットの動きの遷移を記録するvector

	int m_display_node = 0;				//描画しているノード

	MapState m_map_state;				//表示するマップ．

	int m_counter = 0;					//このクラスが実行されてから何回update関数が呼ばれたかカウントする．

	const int kNodeGetCount;			//2秒ごとに読み出す．
};


//! @file graphic_main_basic.h
//! @date 2023/08/09
//! @author 長谷川
//! @brief 基本的な描画クラス．
//! @n 行数 : @lineinfo
