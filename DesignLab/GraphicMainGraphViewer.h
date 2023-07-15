#pragma once
#include "InterfaceGraphicMain.h"
#include "HexapodRenderer.h"
#include "Node.h"
#include "MapState.h"
#include "CameraController.h"
#include "GraphViewerGUIController.h"
#include <memory>

enum class EGraphViewerGUIState
{
	Idle,
	ViewNodeData,
	SelectNodebyGraph,
	SelectObstacle,
	SelectRoute,
};

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

	SNode m_node;
	SNode m_parent_node;
	std::vector<SNode> m_graph;

	void makeGraph(SNode _parent);
};