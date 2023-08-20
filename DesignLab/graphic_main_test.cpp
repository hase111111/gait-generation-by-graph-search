#include "graphic_main_test.h"

#include "designlab_dxlib.h"
#include "map_renderer.h"
#include "Keyboard.h"


GraphicMainTest::GraphicMainTest(const GraphicDataBroker* broker) : IGraphicMain(broker)
{
	m_node.init(false);

	m_camera_manager.setTargetPos(dl_dxlib::convertToDxVec(m_node.global_center_of_mass));

	m_map_state.init(EMapCreateMode::FLAT, MapCreator::OPTION_NONE, false);
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
				HexapodStateCalclator calclator;

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

	if (Keyboard::getIns()->getPressingCount(KEY_INPUT_Z) == 1)
	{
		m_camera_mode++;
		m_camera_mode %= 5;
		m_camera_manager.setCameraViewMode(static_cast<ECameraMode>(m_camera_mode));
	}

	m_hexapod_renderer.update(m_node);
	//m_gui_controller.update(m_camera_manager); //GUIÇçXêVÇ∑ÇÈÅD
	m_camera_manager.update();

	return true;
}


void GraphicMainTest::draw() const
{
	MapRenderer map_render;
	map_render.setNode(m_node);
	map_render.draw(m_map_state);

	m_hexapod_renderer.draw(m_node);

	m_gui_controller.draw(m_node);
}
