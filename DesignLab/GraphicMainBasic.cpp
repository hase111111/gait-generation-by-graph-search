#include "GraphicMainBasic.h"

GraphicMainBasic::GraphicMainBasic(const GraphicDataBroker* _broker)
    : AbstractGraphicMain(_broker), m_Map(mp_Broker->getMapState())
{
}

bool GraphicMainBasic::update()
{
    return false;
}

void GraphicMainBasic::draw() const
{
}
