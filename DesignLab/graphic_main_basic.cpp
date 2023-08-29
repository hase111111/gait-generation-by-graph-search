#include "graphic_main_basic.h"

#include "DxLib.h"

#include "designlab_dxlib.h"
#include "world_grid_renderer.h"
#include "map_renderer.h"


GraphicMainBasic::GraphicMainBasic(const GraphicDataBroker* const  broker, const SApplicationSettingRecorder* const setting)
	: AbstractGraphicMain(broker, setting), m_map_state(mp_broker->getMapState()), kNodeGetCount(setting->window_fps * 2), m_gui_controller(mp_setting),
	m_node_display_gui(mp_setting->window_size_x - NodeDisplayGUI::BOX_SIZE_X - 10, 10)
{
	m_node.clear();
}


bool GraphicMainBasic::update()
{

	//ノードを読み出す時間になったら，仲介人からデータを読み出す．
	if (m_counter % kNodeGetCount == 0)
	{
		mp_broker->copyOnlyNewNode(&m_node);

		m_movement_locus_renderer.setMovementLocus(m_node);   //移動軌跡を更新する．

		m_robot_graund_point_renderer.setNode(m_node);        //ロボットの接地点を更新する．


		std::vector<size_t> simu_end_index;

		mp_broker->copySimuEndIndex(&simu_end_index);

		m_movement_locus_renderer.setSimuEndIndex(simu_end_index);
	}


	m_gui_controller.update((int)m_node.size(), m_display_node, m_counter); //GUIを更新する．


	//ノードが存在しているのならば，各クラスに情報を伝達する
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
	// 3Dのオブジェクトの描画

	dl_dxlib::setZBufferEnable();		//Zバッファを有効にする．


	WorldGridRenderer grid_renderer;	//インスタンスを生成する．

	grid_renderer.draw();				//グリッドを描画する．


	MapRenderer map_render;				//マップを描画する．

	map_render.draw(m_map_state);


	m_movement_locus_renderer.draw();   //移動軌跡を描画する．

	m_robot_graund_point_renderer.draw(-1, true);

	if (!m_node.empty())
	{
		//ノードが存在しているならば，ロボットを描画する．
		m_hexapod_renderer.draw(m_node.at(m_display_node));
	}


	// 2DのGUIの描画

	m_camera_gui.draw();        //カメラのGUIを描画する．

	m_node_display_gui.draw();	 //ノードの情報を表示するGUIを描画する．
}
