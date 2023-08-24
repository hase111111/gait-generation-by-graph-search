#include "simulation_record.h"

#include "leg_state.h"
#include "hexapod_const.h"


std::ofstream& operator<<(std::ofstream& ofs, const SSimulationRecord& record)
{


	const size_t length = record.m_node.size();

	for (size_t i = 0; i < length; i++)
	{
		ofs << i << ",";

		//—V‹rEÚ’n‹r‚Ìo—Í
		for (int j = 0; j < HexapodConst::LEG_NUM; j++)
		{
			ofs << dl_leg::isGrounded(record.m_node[i].leg_state, j) << ",";
		}

		//ŠK‘w‚Ìo—Í
		for (int j = 0; j < HexapodConst::LEG_NUM; j++)
		{
			ofs << dl_leg::getLegState(record.m_node[i].leg_state, j) << ",";
		}

		//dSˆÊ’u‚Ìo—Í
		ofs << dl_leg::getComPatternState(record.m_node[i].leg_state) << ",";

		//Žp¨‚Ìo—Í
		ofs << record.m_node[i].rot.pitch << "," << record.m_node[i].rot.roll << "," << record.m_node[i].rot.yaw << ",";
	}
}
