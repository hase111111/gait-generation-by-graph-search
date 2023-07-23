#include "GraphicMainTest.h"
#include "Dxlib3DFunction.h"
#include "MapRenderer.h"
#include "Keyboard.h"

GraphicMainTest::GraphicMainTest(const GraphicDataBroker* _broker) : IGraphicMain(_broker)
{
	//3D系の処理行う前に初期化する．
	myDxlib3DFunc::initDxlib3D();

	m_node.init(false);

	m_Camera.setTargetPos(myDxlib3DFunc::convertToDxVec(m_node.global_center_of_mass));

	m_Map.init(EMapCreateMode::Flat, MapCreator::OPTION_NONE, false);
}

bool GraphicMainTest::update()
{
	const float _s = 1;

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		if (Keyboard::getIns()->getPressingCount(KEY_INPUT_1 + i) > 0)
		{
			if (Keyboard::getIns()->getPressingCount(KEY_INPUT_Q) > 0) { m_node.leg_pos[i].z += _s; }
			else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_E) > 0) { m_node.leg_pos[i].z -= _s; }
			else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_A) > 0) { m_node.leg_pos[i].y += _s; }
			else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_D) > 0) { m_node.leg_pos[i].y -= _s; }
			else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_W) > 0) { m_node.leg_pos[i].x += _s; }
			else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_S) > 0) { m_node.leg_pos[i].x -= _s; }
			else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_M) == 1)
			{
				HexapodStateCalclator _calclator;

				my_vec::SVector _global = _calclator.getGlobalLegBasePos(m_node, i, true);

				int _map_x = m_Map.getDevideMapNumX(_global.x);
				int _map_y = m_Map.getDevideMapNumY(_global.y);

				if (Keyboard::getIns()->getPressingCount(KEY_INPUT_UP) > 0) { _map_x++; }
				else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_DOWN) > 0) { _map_x--; }
				if (Keyboard::getIns()->getPressingCount(KEY_INPUT_LEFT) > 0) { _map_y++; }
				else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_RIGHT) > 0) { _map_y--; }

				my_vec::SVector _map_pos = m_Map.getPosFromDevideMap(_map_x, _map_y, m_map_index % m_Map.getPointNumFromDevideMap(_map_x, _map_y));
				m_map_index++;

				m_node.leg_pos[i] = _calclator.convertLocalLegPos(m_node, _map_pos, i);
			}
		}
	}

	if (Keyboard::getIns()->getPressingCount(KEY_INPUT_LSHIFT) > 0 || Keyboard::getIns()->getPressingCount(KEY_INPUT_RSHIFT) > 0)
	{
		const float _com_s = 1;

		if (Keyboard::getIns()->getPressingCount(KEY_INPUT_Q) > 0)
		{
			auto _com = m_node.global_center_of_mass;
			_com.z += _com_s;
			m_node.changeGlobalCenterOfMass(_com, false);
		}
		else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_E) > 0)
		{
			auto _com = m_node.global_center_of_mass;
			_com.z -= _com_s;
			m_node.changeGlobalCenterOfMass(_com, false);
		}
		else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_A) > 0)
		{
			auto _com = m_node.global_center_of_mass;
			_com.y += _com_s;
			m_node.changeGlobalCenterOfMass(_com, false);
		}
		else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_D) > 0)
		{
			auto _com = m_node.global_center_of_mass;
			_com.y -= _com_s;
			m_node.changeGlobalCenterOfMass(_com, false);
		}
		else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_W) > 0)
		{
			auto _com = m_node.global_center_of_mass;
			_com.x += _com_s;
			m_node.changeGlobalCenterOfMass(_com, false);
		}
		else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_S) > 0)
		{
			auto _com = m_node.global_center_of_mass;
			_com.x -= _com_s;
			m_node.changeGlobalCenterOfMass(_com, false);
		}
		else if (Keyboard::getIns()->getPressingCount(KEY_INPUT_R) > 0)
		{
			auto _rot = m_node.rot;
			_rot.yaw += _com_s / 360.0f * 2 * my_math::MY_FLT_PI;
			m_node.rot = _rot;
		}
	}

	if (Keyboard::getIns()->getPressingCount(KEY_INPUT_Z) == 1)
	{
		m_camera_mode++;
		m_camera_mode %= 5;
		m_Camera.setCameraMode(static_cast<ECameraMode>(m_camera_mode));
	}

	m_HexapodRender.update(m_node);
	//m_GUI.update(m_Camera); //GUIを更新する．
	m_Camera.update();

	return true;
}

void GraphicMainTest::draw() const
{
	MapRenderer _map_render;
	_map_render.setNode(m_node);
	_map_render.draw(m_Map);

	m_HexapodRender.draw(m_node);

	m_GUI.draw(m_node);
}
