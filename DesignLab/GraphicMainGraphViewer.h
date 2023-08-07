#pragma once
#include "InterfaceGraphicMain.h"
#include "HexapodRenderer.h"
#include "map_state.h"
#include "CameraController.h"
#include "GraphViewerGUIController.h"
#include <memory>

class GraphicMainGraphViewer final : public IGraphicMain
{
public:
	GraphicMainGraphViewer(const GraphicDataBroker* _broker);
	~GraphicMainGraphViewer() = default;

	bool update() override;

	void draw() const override;

private:
	HexapodRenderer m_HexapodRender;
	MapState m_Map;
	CameraController m_Camera;
	std::unique_ptr<GraphViewerGUIController> mp_GUIController;

	std::vector<SNode> m_graph;
	size_t m_display_node_index = 0;

	void updateCameraState();
	int m_camera_mode = 0;
};