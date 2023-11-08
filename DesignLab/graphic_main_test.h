//! @file graphic_main_test.h
//! @brief GraphicMainTestクラス

#ifndef DESIGNLAB_GRAPHIC_MAIN_TEST_H_
#define DESIGNLAB_GRAPHIC_MAIN_TEST_H_

#include "interface_graphic_main.h"

#include <memory>

#include "abstract_hexapod_state_calculator.h"
#include "application_setting_recorder.h"
#include "camera_gui.h"
#include "devide_map_state.h"
#include "interface_hexapod_renderer.h"
#include "map_state.h"
#include "robot_state_node.h"
#include "node_display_gui.h"


//! @class GraphicMainTest
//! @brief MapStateやHexapodStateClaculatorが動作しているかテストを行うためのクラス．
class GraphicMainTest final : public IGraphicMain
{
public:
	GraphicMainTest(const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr,
		const std::shared_ptr<const ApplicationSettingRecorder>& setting_ptr);
	~GraphicMainTest() = default;

	bool Update() override;

	void Draw() const override;

private:

	void MoveBody();

	void MoveLeg();

	const std::shared_ptr<const AbstractHexapodStateCalculator> calculator_ptr_;	//!< ロボットの状態を計算するクラスのシェアードポインタ．


	CameraGui camera_gui_;				//!< カメラの位置を制御するGUI

	NodeDisplayGui node_display_gui_;	//!< ノードの表示を制御するGUI


	const std::unique_ptr<IHexapodRenderer> hexapod_renderer_;	//!< ロボットを表示するクラス．

	MapState map_state_;				//!< マップの状態を保持するクラス．

	DevideMapState devide_map_state_;	//!< マップを分割するクラス．

	RobotStateNode node_;						//!< ロボットの状態

	int map_index_ = 0;				//!< マップのインデックス
};


#endif // !DESIGNLAB_GRAPHIC_MAIN_TEST_H_