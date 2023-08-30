#include "robot_graund_point_renderer.h"

#include "DxLib.h"
#include "designlab_dxlib.h"
#include "hexapod_state_calculator.h"
#include "leg_state.h"


RobotGraundPointRenderer::RobotGraundPointRenderer() : GRAUND_POINT_COLOR(GetColor(230, 15, 145)), GRAUND_POINT_COLOR_BLACK(GetColor(237, 159, 160))
{
}


void RobotGraundPointRenderer::setNode(const std::vector<SNode>& node)
{
	HexapodStateCalclator hexapod_state_calclator;

	while (m_loaded_node_num < node.size())
	{
		for (int i = 0; i < HexapodConst::LEG_NUM; i++)
		{
			if (dl_leg::isGrounded(node[m_loaded_node_num].leg_state, i))
			{
				m_graund_point.push_back(hexapod_state_calclator.getGlobalLegPos(node[m_loaded_node_num], i, false));
			}
		}

		++m_loaded_node_num;
	}
}


void RobotGraundPointRenderer::draw(const size_t draw_simu_num, const bool draw_all_simulation) const
{
	//‚Ð‚Æ‚Ü‚¸‘S‚Ä•`‰æ‚·‚é

	for (size_t i = 0; i < m_graund_point.size(); i++)
	{
		if (draw_all_simulation || i == draw_simu_num)
		{
			dl_dxlib::drawCube3DWithTopPos(dl_dxlib::convertToDxVec(m_graund_point[i]), 25, GRAUND_POINT_COLOR);
		}
		else
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			dl_dxlib::drawCube3DWithTopPos(dl_dxlib::convertToDxVec(m_graund_point[i]), 25, GRAUND_POINT_COLOR_BLACK);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}
