#include "graphic_main_test.h"

#include "dxlib_util.h"
#include "hexapod_state_calculator.h"
#include "keyboard.h"
#include "map_renderer.h"


namespace
{
	constexpr int temp_size = 50;
	constexpr auto temp_ex = 10.8;
	bool temp[HexapodConst::LEG_NUM][temp_size][temp_size][temp_size] = {};
}

namespace dldu = designlab::dxlib_util;


GraphicMainTest::GraphicMainTest(const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr,
	const std::shared_ptr<const SApplicationSettingRecorder>& setting_ptr) :
	calculator_ptr_(calculator_ptr),
	node_display_gui_(setting_ptr ? setting_ptr->window_size_x - NodeDisplayGui::kWidth - 10 : 0, 10, calculator_ptr),
	hexapod_renderer_(calculator_ptr)
{
	m_node.init(false);

	MapCreator map_creator;
	map_state_ = map_creator.Create(EMapCreateMode::FLAT, MapCreator::OPTION_NONE, false);
	devide_map_state_.Init(map_state_);


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

					if (calculator_ptr->isLegInRange(i, pos))
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


bool GraphicMainTest::Update()
{
	const float kSpeed = 1;

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_1 + i) > 0)
		{
			if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_Q) > 0) { m_node.leg_pos[i].z += kSpeed; }
			else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_E) > 0) { m_node.leg_pos[i].z -= kSpeed; }
			else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_A) > 0) { m_node.leg_pos[i].y += kSpeed; }
			else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_D) > 0) { m_node.leg_pos[i].y -= kSpeed; }
			else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_W) > 0) { m_node.leg_pos[i].x += kSpeed; }
			else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_S) > 0) { m_node.leg_pos[i].x -= kSpeed; }
			else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_M) == 1)
			{
				HexapodStateCalclator_Old calclator;

				dl_vec::SVector global = calclator.getGlobalLegBasePos(m_node, i, true);

				int map_x = devide_map_state_.GetDevideMapIndexX(global.x);
				int map_y = devide_map_state_.GetDevideMapIndexY(global.y);

				if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_UP) > 0) { map_x++; }
				else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_DOWN) > 0) { map_x--; }
				if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_LEFT) > 0) { map_y++; }
				else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_RIGHT) > 0) { map_y--; }

				dl_vec::SVector _map_pos = devide_map_state_.GetPointPos(map_x, map_y, m_map_index % devide_map_state_.GetPointNum(map_x, map_y));
				m_map_index++;

				m_node.leg_pos[i] = calclator.convertLocalLegPos(m_node, _map_pos, i, true);
			}
		}
	}

	if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_LSHIFT) > 0 || Keyboard::GetIns()->GetPressingCount(KEY_INPUT_RSHIFT) > 0)
	{
		const float kComSpeed = 1;

		if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_Q) > 0)
		{
			auto com = m_node.global_center_of_mass;
			com.z += kComSpeed;
			m_node.changeGlobalCenterOfMass(com, false);
		}
		else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_E) > 0)
		{
			auto com = m_node.global_center_of_mass;
			com.z -= kComSpeed;
			m_node.changeGlobalCenterOfMass(com, false);
		}
		else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_A) > 0)
		{
			auto com = m_node.global_center_of_mass;
			com.y += kComSpeed;
			m_node.changeGlobalCenterOfMass(com, false);
		}
		else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_D) > 0)
		{
			auto com = m_node.global_center_of_mass;
			com.y -= kComSpeed;
			m_node.changeGlobalCenterOfMass(com, false);
		}
		else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_W) > 0)
		{
			auto com = m_node.global_center_of_mass;
			com.x += kComSpeed;
			m_node.changeGlobalCenterOfMass(com, false);
		}
		else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_S) > 0)
		{
			auto com = m_node.global_center_of_mass;
			com.x -= kComSpeed;
			m_node.changeGlobalCenterOfMass(com, false);
		}
		else if (Keyboard::GetIns()->GetPressingCount(KEY_INPUT_R) > 0)
		{
			auto rot = m_node.rot;
			rot.yaw += kComSpeed / 360.0f * 2 * dl_math::MY_FLT_PI;
			m_node.rot = rot;
		}
	}

	hexapod_renderer_.set_draw_node(m_node);

	node_display_gui_.SetDisplayNode(m_node);

	node_display_gui_.Update();

	camera_gui_.setHexapodPos(m_node.global_center_of_mass);  //カメラの位置を更新する．

	camera_gui_.Update();      //カメラのGUIを更新する．

	return true;
}


void GraphicMainTest::Draw() const
{
	dldu::SetZBufferEnable();


	MapRenderer map_render;

	map_render.Draw(map_state_);


	hexapod_renderer_.Draw();


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

						dldu::DrawCube3D(dldu::ConvertToDxlibVec(calculator_ptr_->getGlobalLegPosition(i, pos, m_node.global_center_of_mass, m_node.rot, true)), 10, color[i]);
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


	camera_gui_.Draw();        //カメラのGUIを描画する．

	node_display_gui_.Draw();	 //ノードの情報を表示するGUIを描画する．

}
