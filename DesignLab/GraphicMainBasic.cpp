#include "GraphicMainBasic.h"
#include "Dxlib3DFunction.h"
#include "DxLib.h"
#include "MapRenderer.h"
#include "HexapodRenderer.h"

GraphicMainBasic::GraphicMainBasic(const GraphicDataBroker* _broker)
    : IGraphicMain(_broker), m_Map(mp_Broker->getMapState())
{
    //3D系の処理行う前に初期化する．
    myDxlib3DFunc::initDxlib3D();

    m_node.clear();
}

bool GraphicMainBasic::update()
{
    if (m_counter % GET_NODE_COUNT == 0) 
    {
        //ノードを読み出す時間になったら，読み出す．
        mp_Broker->copyOnlyNewNode(m_node);
    }

    if (m_counter % CHANGE_NEXT_NODE == 0) 
    {
        //表示するノードを更新する時間になったら更新する．
        m_display_node++;

        //値が大きすぎたら丸める．
        if (m_display_node >= (int)m_node.size()) { m_display_node = (int)m_node.size() - 1; }
    }

    if (m_node.empty() == false) 
    {
        //ノードが存在しているならば，カメラの処理を行う．
        m_Camera.update();
        m_Camera.setTargetPos(myDxlib3DFunc::convertToDxVec(m_node.at(m_display_node).global_center_of_mass));

        //ロボットの状態を更新する．
        m_hexapod.setMyPosition(m_node.at(m_display_node).global_center_of_mass);
        m_hexapod.setLocalLegPos(m_node.at(m_display_node).Leg);
    }

    m_counter++;    //カウンタを進める．

    return false;
}

void GraphicMainBasic::draw() const
{
    //マップを描画する．
    MapRenderer _map_render;
    _map_render.draw(m_Map);

    if (m_node.empty() == false) 
    {
        //ノードが存在しているならば，ロボットを描画する．
        HexapodRenderer _hexapod_render;
        _hexapod_render.draw(m_hexapod, m_node.at(m_display_node).leg_state);
    }
}
