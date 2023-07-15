#pragma once
#include "MapState.h"
#include "GraphicSystem.h"
#include "GraphicDataBroker.h"

class GraphViewerSystemMain final
{
public:

	GraphViewerSystemMain();

	void main();

private:
	MapState m_MapState;
	GraphicDataBroker m_GraphicDataBroker;
	GraphicSystem m_GraphicSystem;
};
