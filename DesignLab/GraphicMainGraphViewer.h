#pragma once
#include "InterfaceGraphicMain.h"
#include "HexapodRenderer.h"
#include "MapState.h"
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
	GraphViewerGUIController m_GUIController;
	SNode m_display_node;
};