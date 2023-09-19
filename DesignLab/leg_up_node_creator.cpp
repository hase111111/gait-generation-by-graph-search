//#include "leg_up_node_creator.h"
//
//#include "com_type.h"
//#include "leg_state.h"
//
//
//void LegUpNodeCreator::create(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph)
//{
//
//	//脚の遊脚・接地によって生じるとりうる重心をcomtypeとして仕分けている．(詳しくはComtype.hを参照)．まずは全てtrueにしておく．
//	boost::dynamic_bitset<> is_able_leg_ground_pattern(dl_com::getLegGroundPatternNum());
//
//	is_able_leg_ground_pattern.set();	//全てtrueにする．
//
//
//
//	//まず離散化された重心位置から取り得ない接地パターンを除外する．
//
//	dl_com::banLegGroundPatternFromCom(dl_leg::getComPatternState(current_node.leg_state), &is_able_leg_ground_pattern);
//
//
//
//	//既に遊脚の脚を接地することはできない．
//	{
//		std::vector<int> lifted_leg_num;
//		dl_leg::getLiftedLegIndexWithVector(current_node.leg_state, &lifted_leg_num);
//
//		for (auto& i : lifted_leg_num)
//		{
//			dl_com::banLegGroundPatternFromNotGroundableLeg(i, &is_able_leg_ground_pattern);
//		}
//	}
//
//
//
//	//子ノードを生成する．
//	for (int i = 0; i < dl_com::getLegGroundPatternNum(); i++)
//	{
//		//その重心タイプが可能であれば，
//		if (is_able_leg_ground_pattern[i])
//		{
//			SNode res_node = current_node;
//
//			res_node.changeNextNode(current_num, m_next_move);
//
//
//			//遊脚・接地を書き換える．
//			dl_leg::LegGroundedBit new_is_ground = dl_com::getLegGroundedBitFromLegGroundPatternIndex(i);
//
//			dl_leg::changeAllLegGround(new_is_ground, &res_node.leg_state);
//
//
//			for (int j = 0; j < HexapodConst::LEG_NUM; j++)
//			{
//				dl_leg::changeGround(j, new_is_ground[j], &res_node.leg_state);
//
//				if (!new_is_ground[j])
//				{
//					res_node.leg_pos[j].x = 160 * HexapodConst::DEFAULT_LEG_ANGLE_COS[j];
//					res_node.leg_pos[j].y = 160 * HexapodConst::DEFAULT_LEG_ANGLE_SIN[j];
//					res_node.leg_pos[j].z = -25;
//
//					res_node.leg_base_pos[j].x = 160 * HexapodConst::DEFAULT_LEG_ANGLE_COS[j];
//					res_node.leg_base_pos[j].y = 160 * HexapodConst::DEFAULT_LEG_ANGLE_SIN[j];
//				}
//			}
//
//			//可能な姿勢ならば，子ノードとして追加する．
//			if (m_calculator.isAblePause(res_node))
//			{
//				(*output_graph).push_back(res_node);
//			}
//		}
//	}
//
//	//出力されたノードがないならば，そのままのノードを出力する．
//	if ((*output_graph).size() == 0)
//	{
//		SNode res_node = current_node;
//
//		res_node.changeNextNode(current_num, m_next_move);
//
//		(*output_graph).push_back(res_node);
//	}
//
//}
