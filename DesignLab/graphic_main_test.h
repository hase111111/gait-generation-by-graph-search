#pragma once

#include "interface_graphic_main.h"
#include "camera_controller.h"
#include "hexapod_renderer.h"
#include "gui_controller.h"
#include "node.h"
#include "map_state.h"


//! @class GraphicMainTest
//! @date 2023/08/09
//! @author 長谷川
//! @brief MapStateやHexapodStateClaculatorが動作しているか視覚的に分かりやすくするためのテストシーン
class GraphicMainTest final : public IGraphicMain
{
public:
	GraphicMainTest(const GraphicDataBroker* broker);
	~GraphicMainTest() = default;

	bool update() override;

	void draw() const override;

private:
	SNode m_node;

	CameraController m_camera_controller;	//カメラ制御クラス．
	HexapodRenderer m_hexapod_renderer;
	MapState m_map_state;
	GUIController m_gui_controller;		// GUI (ロボットの状態とか表示する窓) を制御するクラス．

	int m_camera_mode = 0;		//カメラのモード．

	int m_map_index = 0;
};


//! @file graphic_main_test.h
//! @date 2023/08/09
//! @author 長谷川
//! @brief GraphicMainTestクラス
//! @n 行数 : @lineinfo
