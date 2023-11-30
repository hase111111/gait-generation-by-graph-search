//! @file graphic_main_test.h
//! @brief GraphicMainTestクラス

#ifndef DESIGNLAB_GRAPHIC_MAIN_TEST_H_
#define DESIGNLAB_GRAPHIC_MAIN_TEST_H_

#include "interface_graphic_main.h"

#include <memory>

#include "application_setting_recorder.h"
#include "devide_map_state.h"
#include "dxlib_3d_renderer_group.h"
#include "dxlib_node_setter_group.h"
#include "dxlib_gui_updater.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_joint_calculator.h"
#include "interface_hexapod_vaild_checker.h"
#include "map_state.h"
#include "robot_state_node.h"


//! @class GraphicMainTest
//! @brief MapStateやHexapodStateClaculatorが動作しているかテストを行うためのクラス．
class GraphicMainTest final : public IGraphicMain
{
public:
	GraphicMainTest(
		const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
		const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
		const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr,
		const std::shared_ptr<const ApplicationSettingRecorder>& setting_ptr
	);
	~GraphicMainTest() = default;

	bool Update() override;

	void Draw() const override;

private:

	void MoveBody();

	void MoveLeg();

	std::shared_ptr<Mouse> mouse_ptr_;			//!< マウスの位置を制御するクラス．

	DxlibGuiUpdater gui_updater_;				//!< GUIの更新を行うクラス
	DxlibNodeSetterGroup node_setter_group_;	//!< ノードの設定を行うクラス
	Dxlib3dRendererGroup render_group_;			//!< 描画を行うクラス

	const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
	const std::shared_ptr<const IHexapodJointCalculator> calculator_ptr_;

	MapState map_state_;				//!< マップの状態を保持するクラス．
	DevideMapState devide_map_state_;	//!< 分割したマップの状態を保持するクラス．
	RobotStateNode robot_;				//!< ロボットの状態．

	int devide_map_tile_index_{ 0 };		//!< 脚を接地しているデバイドマップのインデックス．
};


#endif // DESIGNLAB_GRAPHIC_MAIN_TEST_H_