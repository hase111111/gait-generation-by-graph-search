#pragma once

#include <memory>

#include "abstract_graphic_main.h"
#include "hexapod_renderer.h"
#include "map_state.h"
#include "camera_manager.h"
#include "graph_viewer_gui_controller.h"


//! @class GraphicMainGraphViewer
//! @date 2023/08/09
//! @author 長谷川
//! @brief GraphViewerの画面を表示するクラス
class GraphicMainGraphViewer final : public AbstractGraphicMain
{
public:
	GraphicMainGraphViewer(const GraphicDataBroker* broker);
	~GraphicMainGraphViewer() = default;

	bool update() override;

	void draw() const override;

private:

	void updateCameraState();


	HexapodRenderer m_hexapod_renderer;

	MapState m_map_state;

	CameraManager m_camera_manager;

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
