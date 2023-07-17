#include "GraphicMainGraphViewer.h"
#include "Dxlib3DFunction.h"
#include "MapRenderer.h"
#include "IGraphTreeCreator.h"
#include "GraphTreeCreatorHato.h"
#include <memory>
#include "Keyboard.h"

GraphicMainGraphViewer::GraphicMainGraphViewer(const GraphicDataBroker* _broker) : IGraphicMain(_broker), m_Map(_broker->getMapState())
{
	//3D系の処理行う前に初期化する．
	myDxlib3DFunc::initDxlib3D();

	m_display_node.init(false);
	m_HexapodRender.update(m_display_node);

	//カメラのターゲット座標をセットする
	m_Camera.setTargetPos(myDxlib3DFunc::convertToDxVec(m_display_node.global_center_of_mass));
}

bool GraphicMainGraphViewer::update()
{
	m_Camera.update();

	return true;
}

void GraphicMainGraphViewer::draw() const
{
	MapRenderer mapRenderer;
	mapRenderer.setNode(m_display_node);
	mapRenderer.draw(m_Map);

	m_HexapodRender.draw(m_display_node);

	m_GUIController.draw();
}