#include "graphic_main_test.h"

#include "designlab_dxlib.h"
#include "map_renderer.h"
#include "Keyboard.h"
#include "hexapod_state_calculator.h"

constexpr int temp_size = 50;
constexpr auto temp_ex = 10.8;
bool temp[HexapodConst::LEG_NUM][temp_size][temp_size][temp_size] = {};


GraphicMainTest::GraphicMainTest(const GraphicDataBroker* const  broker, std::shared_ptr<AbstractHexapodStateCalculator> calc, const SApplicationSettingRecorder* const setting) :
	AbstractGraphicMain(broker, calc, setting), m_node_display_gui(mp_setting->window_size_x - NodeDisplayGUI::BOX_SIZE_X - 10, 10, calc),
	m_hexapod_renderer(calc)
{
	m_node.init(false);

	m_map_state.init(EMapCreateMode::FLAT, MapCreator::OPTION_NONE, false);


	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		for (int x = 0; x < temp_size; x += 1)
		{
			for (int y = 0; y < temp_size; y += 1)
			{
				for (int z = 0; z < temp_size; z += 1)
				{
					dl_vec::SVector pos((float)x - temp_size / 2, (float)y - temp_size / 2, (float)z - temp_size / 2);
					pos *= (float)temp_ex;

					if (mp_calculator->isLegInRange(i, pos))
					{
						temp[i][x][y][z] = true;
					}
					else
					{
						temp[i][x][y][z] = false;
					}
				}
			}
		}
	}

}


bool GraphicMainTest::update()
{
	const float speed = 1;

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		if (Keyboard::getIns()->getPressingCount(KEY_INPUT_1 + i) > 0)
		{
			if (Keyboard::getIns()->getPressingCount(KEY_INPUT_Q) > 0) { m_node.leg_pos[i].z += speed; }
			else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_E) > 0) { m_node.leg_pos[i].z -= speed; }
			else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_A) > 0) { m_node.leg_pos[i].y += speed; }
			else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_D) > 0) { m_node.leg_pos[i].y -= speed; }
			else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_W) > 0) { m_node.leg_pos[i].x += speed; }
			else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_S) > 0) { m_node.leg_pos[i].x -= speed; }
			else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_M) == 1)
			{
				HexapodStateCalclator_Old calclator;

				dl_vec::SVector global = calclator.getGlobalLegBasePos(m_node, i, true);

				int _map_x = m_map_state.getDevideMapNumX(global.x);
				int _map_y = m_map_state.getDevideMapNumY(global.y);

				if (Keyboard::getIns()->getPressingCount(KEY_INPUT_UP) > 0) { _map_x++; }
				else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_DOWN) > 0) { _map_x--; }
				if (Keyboard::getIns()->getPressingCount(KEY_INPUT_LEFT) > 0) { _map_y++; }
				else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_RIGHT) > 0) { _map_y--; }

				dl_vec::SVector _map_pos = m_map_state.getPosFromDevideMap(_map_x, _map_y, m_map_index % m_map_state.getPointNumFromDevideMap(_map_x, _map_y));
				m_map_index++;

				m_node.leg_pos[i] = calclator.convertLocalLegPos(m_node, _map_pos, i, true);
			}
		}
	}

	if (Keyboard::getIns()->getPressingCount(KEY_INPUT_LSHIFT) > 0 || Keyboard::getIns()->getPressingCount(KEY_INPUT_RSHIFT) > 0)
	{
		const float com_speed = 1;

		if (Keyboard::getIns()->getPressingCount(KEY_INPUT_Q) > 0)
		{
			auto com = m_node.global_center_of_mass;
			com.z += com_speed;
			m_node.changeGlobalCenterOfMass(com, false);
		}
		else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_E) > 0)
		{
			auto com = m_node.global_center_of_mass;
			com.z -= com_speed;
			m_node.changeGlobalCenterOfMass(com, false);
		}
		else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_A) > 0)
		{
			auto com = m_node.global_center_of_mass;
			com.y += com_speed;
			m_node.changeGlobalCenterOfMass(com, false);
		}
		else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_D) > 0)
		{
			auto com = m_node.global_center_of_mass;
			com.y -= com_speed;
			m_node.changeGlobalCenterOfMass(com, false);
		}
		else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_W) > 0)
		{
			auto com = m_node.global_center_of_mass;
			com.x += com_speed;
			m_node.changeGlobalCenterOfMass(com, false);
		}
		else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_S) > 0)
		{
			auto com = m_node.global_center_of_mass;
			com.x -= com_speed;
			m_node.changeGlobalCenterOfMass(com, false);
		}
		else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_R) > 0)
		{
			auto rot = m_node.rot;
			rot.yaw += com_speed / 360.0f * 2 * dl_math::MY_FLT_PI;
			m_node.rot = rot;
		}
	}

	m_hexapod_renderer.setNode(m_node);

	m_node_display_gui.setDisplayNode(m_node);

	m_node_display_gui.update();

	m_camera_gui.setHexapodPos(m_node.global_center_of_mass);  //カメラの位置を更新する．

	m_camera_gui.update();      //カメラのGUIを更新する．

	return true;
}


void GraphicMainTest::draw() const
{
	dl_dxlib::setZBufferEnable();

	MapRenderer map_render;

	map_render.draw(m_map_state);


	m_hexapod_renderer.draw();


	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		for (int x = 0; x < temp_size; x += 1)
		{
			for (int y = 0; y < temp_size; y += 1)
			{
				for (int z = 0; z < temp_size; z += 1)
				{
					if (temp[i][x][y][z])
					{
						dl_vec::SVector pos(x - (float)temp_size / 2, y - (float)temp_size / 2, z - (float)temp_size / 2);
						pos *= (float)temp_ex;

						SetDrawBlendMode(DX_BLENDMODE_PMA_ALPHA, 255);
						unsigned int  all_color = GetColor(170, 64, 64);
						unsigned int color[6] = {
							all_color,all_color,all_color,
							all_color,all_color,all_color
							//GetColor(255, 128, 255),GetColor(255, 128, 128),GetColor(255, 255, 128),
							//GetColor(128, 255, 128),GetColor(128, 255, 255),GetColor(128, 128, 255)
						};

						dl_dxlib::drawCube3D(dl_dxlib::convertToDxVec(mp_calculator->getGlobalLegPosition(i, pos, m_node.global_center_of_mass, m_node.rot, true)), 10, color[i]);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
					}
					else
					{
						//
					}
				}
			}
		}
	}


	m_camera_gui.draw();        //カメラのGUIを描画する．

	m_node_display_gui.draw();	 //ノードの情報を表示するGUIを描画する．

}
