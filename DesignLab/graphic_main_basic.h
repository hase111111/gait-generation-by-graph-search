//! @file graphic_main_basic.h
//! @brief 基本的な描画クラス．

#ifndef DESIGNLAB_GRAPHIC_MAIN_BASIC_H_
#define DESIGNLAB_GRAPHIC_MAIN_BASIC_H_

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
#include "map_state.h"
#include "movement_locus_renderer.h"
#include "node.h"
#include "node_display_gui.h"
#include "robot_graund_point_renderer.h"
#include "stability_margin_renderer.h"


//! @class GraphicMainBasic
//! @brief このプロジェクトにおける標準的なロボットの描画機能を持つクラス．
//! @details 波東さんのプログラムのロボット表示機能を書き直したもの．
//! 基本的な処理の内容は変化していないが，より表示するデータの内容が詳しくなっている．
//! また，UIによってランタイムで表示方法を制御することができるようになったため，よりロボットの状態を理解しやすくなっている．ﾀﾌﾞﾝﾈ
//! @note 処理を大きく書き換えたい場合はそもそも新しいクラスを書くようにするとよいと思う．
//! @n GraphicSampleを参考にして，作成するようにすると楽．

class GraphicMainBasic final : public IGraphicMain
{
public:
	GraphicMainBasic(const std::shared_ptr<const GraphicDataBroker>& broker_ptr, const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr,
		const std::shared_ptr<const ApplicationSettingRecorder>& setting_ptr);
	~GraphicMainBasic() = default;

	bool Update() override;

	void Draw() const override;

private:

	const int kNodeGetCount;	//!< このカウントごとにデータをBrokerから読み出す


	const std::shared_ptr<const GraphicDataBroker> broker_ptr_;	//!< 画像表示を行うこのクラスと，データ処理を行う外部のクラスを繋ぐ仲介人クラスのポインタを受け取る．


	CameraGui camera_gui_;							//!< カメラの位置を制御するGUI

	NodeDisplayGui node_display_gui_;				//!< ノードの表示を制御するGUI

	DisplayNodeSwitchGui display_node_switch_gui_;	//!< ノードの表示を切り替えるGUI


	HexapodRenderer hexapod_renderer_;						//!< ロボットを表示するクラス．	

	MovementLocusRenderer movement_locus_renderer_;			//!< ロボットの動きの軌跡を表示するクラス．

	RobotGraundPointRenderer robot_graund_point_renderer_;	//!< ロボットの足先の位置を表示するクラス．

	StabilityMarginRenderer stability_margin_renderer_;		//!< ロボットの静的安定余裕を表示するクラス．


	MapState map_state_;		//!< 表示するマップ．

	std::vector<SNode> graph_;	//!< ロボットの動きの遷移を記録するvector

	size_t display_node_index_;	//!< 描画しているノード

	int counter_;				//!< このクラスが実行されてから何回update関数が呼ばれたかカウントする．


	bool is_displayed_movement_locus_;		//!< ロボットの動きの軌跡を表示するかどうか．

	bool is_displayed_robot_graund_point_;	//!< ロボットの足先の位置を表示するかどうか．


	int graph_update_count;	//!< 処理部でのグラフの更新回数．

	int map_update_count;	//!< 処理部でのマップの更新回数．
};


#endif // !DESIGNLAB_GRAPHIC_MAIN_BASIC_H_