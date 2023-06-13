#include "GraphicMainBasic.h"
#include "Dxlib3DFunction.h"
#include "DxLib.h"
#include "MapRenderer.h"

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

    m_GUI.update(m_Camera, m_node.size(), m_display_node, m_counter); //GUIを更新する．

    if (m_node.empty() == false) 
    {
        m_Camera.setTargetPos(myDxlib3DFunc::convertToDxVec(m_node.at(m_display_node).global_center_of_mass));      //ノードが存在しているならば，カメラの処理を行う．

        m_HexapodRender.update(m_node.at(m_display_node));      //ロボットの状態を更新する．
    }

    m_counter++;            //カウンタを進める．

    m_Camera.update();      //カメラを更新する

    return false;
}

void GraphicMainBasic::draw() const
{
    if (m_node.empty() == false) 
    {
        //マップを描画する．
        MapRenderer _map_render;
        _map_render.setNode(m_node.at(m_display_node));
        _map_render.draw(m_Map);

        //ノードが存在しているならば，ロボットを描画する．
        m_HexapodRender.draw(m_node.at(m_display_node));

        //UIを表示する．
        m_GUI.draw(m_node.at(m_display_node));
    }
}