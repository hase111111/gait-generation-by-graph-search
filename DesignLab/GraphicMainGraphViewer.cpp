#include "GraphicMainGraphViewer.h"
#include "Dxlib3DFunction.h"
#include "MapRenderer.h"
#include "IGraphTreeCreator.h"
#include "GraphTreeCreatorHato.h"
#include <memory>
#include "Keyboard.h"

GraphicMainGraphViewer::GraphicMainGraphViewer(const GraphicDataBroker* _broker) : IGraphicMain(_broker)
{
	//3DŒn‚Ìˆ—s‚¤‘O‚É‰Šú‰»‚·‚éD
	myDxlib3DFunc::initDxlib3D();

	m_node.init(true);
	m_parent_node.init(false);

	m_Camera.setTargetPos(myDxlib3DFunc::convertToDxVec(m_node.global_center_of_mass));

	m_Map.init(EMapCreateMode::Flat, MapCreator::OPTION_NONE, false);
}

bool GraphicMainGraphViewer::update()
{
	m_HexapodRender.update(m_node);
	m_HexapodRender.update(m_parent_node);

	m_Camera.update();

	if (Keyboard::getIns()->getPressingCount(KEY_INPUT_Z) == 1)
	{
		makeGraph(m_node);
	}

	return true;
}

void GraphicMainGraphViewer::draw() const
{
	MapRenderer mapRenderer;
	mapRenderer.setNode(m_node);
	mapRenderer.draw(m_Map);

	m_HexapodRender.draw(m_node);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	m_HexapodRender.draw(m_parent_node);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	m_GUIController.draw();
}

void GraphicMainGraphViewer::makeGraph(SNode _parent)
{
	m_graph.clear();

	_parent.changeParentNode();
	m_graph.push_back(_parent);

	std::unique_ptr<IGraphTreeCreator> graphCreator = std::make_unique<GraphTreeCreatorHato>();

	graphCreator->createGraphTree(_parent, &m_Map, m_graph);
}
