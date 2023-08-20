#include "graphic_main_basic.h"

#include "DxLib.h"

#include "designlab_dxlib.h"
#include "map_renderer.h"


GraphicMainBasic::GraphicMainBasic(const GraphicDataBroker* broker) : IGraphicMain(broker), m_map_state(mp_broker->getMapState())
{
	m_node.clear();
}


bool GraphicMainBasic::update()
{
	if (m_counter % kNodeGetCount == 0)
	{
		//ノードを読み出す時間になったら，読み出す．
		mp_broker->copyOnlyNewNode(&m_node);
	}

	m_gui_controller.update(m_camera_controller, (int)m_node.size(), m_display_node, m_counter); //GUIを更新する．

	if (!m_node.empty())
	{
		m_camera_controller.setTargetPos(dl_dxlib::convertToDxVec(m_node.at(m_display_node).global_center_of_mass));      //ノードが存在しているならば，カメラの処理を行う．

		m_hexapod_renderer.update(m_node.at(m_display_node));      //ロボットの状態を更新する．
	}

	m_counter++;            //カウンタを進める．

	m_camera_controller.update();      //カメラを更新する

	return true;
}


void GraphicMainBasic::draw() const
{
	if (!m_node.empty())
	{
		dl_dxlib::setZBufferEnable();

		//マップを描画する．
		MapRenderer map_render;
		map_render.setNode(m_node.at(m_display_node));
		map_render.draw(m_map_state);

		//ノードが存在しているならば，ロボットを描画する．
		m_hexapod_renderer.draw(m_node.at(m_display_node));

		//UIを表示する．
		m_gui_controller.draw(m_node.at(m_display_node));
	}
}
