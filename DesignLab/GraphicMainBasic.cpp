#include "GraphicMainBasic.h"
#include "Dxlib3DFunction.h"

GraphicMainBasic::GraphicMainBasic(const GraphicDataBroker* _broker)
    : AbstractGraphicMain(_broker), m_Map(mp_Broker->getMapState())
{
    //3DŒn‚Ìˆ—s‚¤‘O‚É‰Šú‰»‚·‚éD
    myDxlib3DFunc::initDxlib3D();
}

bool GraphicMainBasic::update()
{
    return false;
}

void GraphicMainBasic::draw() const
{
}
