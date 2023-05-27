#include "ComUpDownNodeCreator.h"
#include "HexapodConst.h"
#include <cfloat>
#include "LegState.h"

void ComUpDownNodeCreator::create(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph)
{
	//重心を最も高くあげることのできる位置と，最も低く下げることのできる位置を求める．
	float _highest_pos = FLT_MIN;
	float _lowest_pos = FLT_MAX;

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		//接地している脚についてのみ考える．
		if (LegState::isGrounded(_current_node.leg_state, i) == true)
		{

		}
	}
}
