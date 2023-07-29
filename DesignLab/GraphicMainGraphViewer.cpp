#include "GraphicMainGraphViewer.h"
#include "Dxlib3DFunction.h"
#include "MapRenderer.h"
#include "GraphTreeCreatorHato.h"
#include <memory>
#include "Keyboard.h"

GraphicMainGraphViewer::GraphicMainGraphViewer(const GraphicDataBroker* _broker) : IGraphicMain(_broker), m_Map(_broker->getMapState())
{
	//適当なノードを生成して，描画クラスを初期化する
	SNode _temp;
	_temp.init(false);

	m_HexapodRender.update(_temp);
	m_Camera.setTargetPos(myDxlib3DFunc::convertToDxVec(_temp.global_center_of_mass));

	// GUI にグラフのポインタを渡す.
	mp_GUIController = std::make_unique<GraphViewerGUIController>(&m_graph, &m_display_node_index);
}

bool GraphicMainGraphViewer::update()
{
	updateCameraState();

	mp_GUIController->update();

	//仲介人の持つグラフデータと自身の持っているグラフデータが一致していないならば
	if (mp_Broker->getNodeNum() != m_graph.size())
	{
		mp_Broker->copyAllNode(m_graph);	//データを更新する

		//グラフの中身が空でないならば，表示するノードを初期化する
		if (m_graph.size() > 0) { m_display_node_index = 0; }

		mp_GUIController->updateGraphNodeDepthData();

	}

	//HexapodReanderの更新
	if (m_display_node_index < m_graph.size() && m_graph.size() > 0)
	{
		m_HexapodRender.update(m_graph.at(m_display_node_index));
	}

	return true;
}

void GraphicMainGraphViewer::draw() const
{
	MapRenderer mapRenderer;
	mapRenderer.draw(m_Map);

	if (m_display_node_index < m_graph.size())
	{
		m_HexapodRender.draw(m_graph.at(m_display_node_index));
	}


	mp_GUIController->draw();
}

void GraphicMainGraphViewer::updateCameraState()
{
	m_Camera.update();

	if (Keyboard::getIns()->getPressingCount(KEY_INPUT_Z) == 1)
	{
		m_camera_mode++;
		m_camera_mode %= 5;
		m_Camera.setCameraMode(static_cast<ECameraMode>(m_camera_mode));
	}

	if (m_display_node_index < m_graph.size())
	{
		m_Camera.setTargetPos(myDxlib3DFunc::convertToDxVec(m_graph.at(m_display_node_index).global_center_of_mass));
	}
}
