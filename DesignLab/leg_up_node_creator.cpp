#include "leg_up_node_creator.h"

#include "com_type.h"
#include "leg_state.h"


void LegUpNodeCreator::create(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph)
{
	//脚の遊脚・接地によって生じるとりうる重心をcomtypeとして仕分けている．(詳しくはComtype.hを参照)．まずは全てtrueにしておく．
	bool is_able_type[dl_com::COM_TYPE_NUM];

	for (int i = 0; i < dl_com::COM_TYPE_NUM; i++)
	{
		is_able_type[i] = true;
	}

	//重心が現在どこにあるか(前よりか真ん中か...)などのパラメータはこのcom patternで仕分けている．(詳しくはComtype.hを参照)．これを取得する．
	EDiscreteComPos com_pattern = dl_leg::getComPatternState(current_node.leg_state);

	//com patternよりとることができないcom typeを全てfalseにする．
	dl_com::checkAbleComTypeFromComPattern(static_cast<int>(com_pattern) - 1, is_able_type);

	//既に遊脚の脚を接地することはできない．
	{
		std::vector<int> lifted_leg_num;
		dl_leg::getLiftedLegIndexWithVector(current_node.leg_state, &lifted_leg_num);

		for (auto& i : lifted_leg_num)
		{
			dl_com::checkAbleComTypeFromNotGroundableLeg(i, is_able_type);
		}
	}

	//子ノードを生成する．
	for (int i = 0; i < dl_com::COM_TYPE_NUM; i++)
	{
		//その重心タイプが可能であれば，
		if (is_able_type[i])
		{
			SNode res_node = current_node;
			res_node.changeNextNode(current_num, m_next_move);

			//遊脚・接地を書き換える．
			bool new_ground[HexapodConst::LEG_NUM] = {};
			dl_com::getGroundLegFromComType(i, new_ground);

			for (int j = 0; j < HexapodConst::LEG_NUM; j++)
			{
				dl_leg::changeGround(j, new_ground[j], &res_node.leg_state);

				if (!new_ground[j])
				{
					res_node.leg_pos[j].x = 160 * HexapodConst::DEFAULT_LEG_ANGLE_COS[j];
					res_node.leg_pos[j].y = 160 * HexapodConst::DEFAULT_LEG_ANGLE_SIN[j];
					res_node.leg_pos[j].z = -10;
				}
			}

			//可能な姿勢ならば，子ノードとして追加する．
			if (m_calculator.isAblePause(res_node))
			{
				(*output_graph).push_back(res_node);
			}
		}
	}

	//出力されたノードがないならば，そのままのノードを出力する．
	if ((*output_graph).size() == 0)
	{
		SNode res_node = current_node;

		res_node.changeNextNode(current_num, m_next_move);

		(*output_graph).push_back(res_node);
	}

}
