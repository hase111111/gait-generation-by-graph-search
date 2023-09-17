#pragma once

#include <memory>

#include "abstract_graphic_main.h"
#include "hexapod_renderer.h"
#include "map_state.h"
#include "graph_viewer_gui_controller.h"
#include "camera_gui.h"
#include "node_display_gui.h"


//! @class GraphicMainGraphViewer
//! @date 2023/08/09
//! @author 長谷川
//! @brief GraphViewerの画面を表示するクラス
class GraphicMainGraphViewer final : public AbstractGraphicMain
{
public:
	GraphicMainGraphViewer(const GraphicDataBroker* const  broker, std::shared_ptr<AbstractHexapodStateCalculator> calc, const SApplicationSettingRecorder* const setting);
	~GraphicMainGraphViewer() = default;

	bool Update() override;

	void Draw() const override;

private:

	HexapodRenderer m_hexapod_renderer;

	MapState m_map_state;

	CameraGUI m_camera_gui;

	NodeDisplayGui m_node_display_gui;

	std::unique_ptr<GraphViewerGUIController> mp_gui_controller;


	std::vector<SNode> m_graph;

	size_t m_display_node_index = 0;

	int m_camera_mode = 0;
};


//! @file graphic_main_graph_viewer.h
//! @date 2023/08/09
//! @author 長谷川
//! @brief GraphicMainGraphViewerクラス
//! @n 行数 : @lineinfo
