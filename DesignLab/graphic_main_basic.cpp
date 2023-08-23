#include "graphic_main_basic.h"

#include "DxLib.h"

#include "designlab_dxlib.h"
#include "world_grid_renderer.h"
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

		m_movement_locus_renderer.setMovementLocus(m_node);   //移動軌跡を更新する．
	}

	m_gui_controller.update((int)m_node.size(), m_display_node, m_counter); //GUIを更新する．

	if (!m_node.empty())
	{
		m_camera_gui.setHexapodPos(m_node.at(m_display_node).global_center_of_mass);

		m_hexapod_renderer.update(m_node.at(m_display_node));      //ロボットの状態を更新する．
	}

	m_counter++;            //カウンタを進める．

	m_camera_gui.update();      //カメラのGUIを更新する．

	return true;
}


void GraphicMainBasic::draw() const
{
	dl_dxlib::setZBufferEnable();   //Zバッファを有効にする．


	WorldGridRenderer grid_renderer;	//インスタンスを生成する．

	grid_renderer.draw();    //グリッドを描画する．


	m_movement_locus_renderer.draw();   //移動軌跡を描画する．


	if (!m_node.empty())
	{
		//マップを描画する．
		MapRenderer map_render;
		map_render.setNode(m_node.at(m_display_node));
		map_render.draw(m_map_state);

		//ノードが存在しているならば，ロボットを描画する．
		m_hexapod_renderer.draw(m_node.at(m_display_node));

		//UIを表示する．
		m_gui_controller.draw(m_node.at(m_display_node));
	}

	m_camera_gui.draw();        //カメラのGUIを描画する．
}
