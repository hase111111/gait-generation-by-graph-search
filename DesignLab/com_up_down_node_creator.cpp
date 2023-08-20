#include "com_up_down_node_creator.h"

#include <cfloat>
#include <algorithm>

#include "graph_search_const.h"
#include "hexapod_const.h"
#include "hexapod_state_calculator.h"
#include "leg_state.h"
#include "designlab_math.h"


ComUpDownNodeCreator::ComUpDownNodeCreator(const MapState* const p_map, const EHexapodMove next_move) : INodeCreator(p_map, next_move), mp_map(p_map)
{
	if (GraphSearchConst::DO_DEBUG_PRINT)
	{
		std::cout << "[NodeCreator] ComUpDownNodeCreator : コンストラクタが呼ばれた．\n";
	}
}


ComUpDownNodeCreator::~ComUpDownNodeCreator()
{
	if (GraphSearchConst::DO_DEBUG_PRINT)
	{
		std::cout << "[NodeCreator] ComUpDownNodeCreator : デストラクタが呼ばれた．\n";
	}
}


void ComUpDownNodeCreator::create(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph)
{
	//重心を最も高くあげることのできる位置と，最も低く下げることのできる位置を求める．グローバル座標で Zの位置．
	//マップを確認して地面の最高点を求め，そこからMAX_RANGE，MIN_RANGEの分だけ離す．


	//マップの最大z座標を求める．
	const int kMapX = mp_map->getDevideMapNumX(current_node.global_center_of_mass.x);
	const int kMapY = mp_map->getDevideMapNumY(current_node.global_center_of_mass.y);
	const float kMapHighestZ = mp_map->getTopZFromDevideMap(kMapX, kMapY);

	//ロボットの重心の最も低く下げることのできるz座標と，高くあげることができるz座標を求める．どちらもグローバル座標．
	float highest_body_zpos = kMapHighestZ + HexapodConst::VERTICAL_MAX_RANGE;
	float lowest_body_zpos = kMapHighestZ + HexapodConst::VERTICAL_MIN_RANGE;


	// 最も高い地点を修正する．

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		//接地している脚についてのみ考える．
		if (dl_leg::isGrounded(current_node.leg_state, i))
		{
			//三平方の定理を使って，脚接地地点から重心位置をどれだけ上げられるか考える．
			const float edge_c = HexapodConst::FEMUR_LENGTH + HexapodConst::TIBIA_LENGTH - MARGIN;
			const float edge_b = current_node.leg_pos[i].projectedXY().length() - HexapodConst::COXA_LENGTH;

			const float edge_a = sqrt(dl_math::squared(edge_c) - dl_math::squared(edge_b));

			//接地脚の最大重心高さの中から一番小さいものを全体の最大重心位置として記録する．_aは脚の接地点からどれだけ上げられるかを表しているので，グローバル座標に変更する．
			highest_body_zpos = (std::min)(edge_a + current_node.global_center_of_mass.z + current_node.leg_pos[i].z, highest_body_zpos);
		}
	}


	//ノードを追加する．
	pushNodeByMaxAndMinPosZ(current_node, current_num, highest_body_zpos, lowest_body_zpos, output_graph);
}


void ComUpDownNodeCreator::pushNodeByMaxAndMinPosZ(const SNode& current_node, const int current_num, const float high, const float low, std::vector<SNode>* output_graph)
{
	//重心を変化させたものを追加する．変化量が一番少ないノードは削除する．
	{
		//最大と最小の間を分割する．
		const float kDivZ = (high - low) / (float)DISCRETIZATION;

		//現在の重心との差分が一番小さいものを探す．
		float dif_min = 100000.0f;
		int dif_min_index = -1;

		//分割した分新しいノードを追加する．
		for (int i = 0; i < DISCRETIZATION + 1; i++)
		{
			SNode new_node = current_node;

			//重心の位置を変更する．
			dl_vec::SVector new_com = current_node.global_center_of_mass;
			new_com.z = low + kDivZ * i;

			new_node.changeGlobalCenterOfMass(new_com, true);

			if (dif_min > abs(current_node.global_center_of_mass.z - new_node.global_center_of_mass.z))
			{
				dif_min = abs(current_node.global_center_of_mass.z - new_node.global_center_of_mass.z);
				dif_min_index = i;
			}

			//current_numを親とする，新しいノードに変更する
			new_node.changeNextNode(current_num, m_next_move);

			//ノードを追加する．
			(*output_graph).emplace_back(new_node);
		}

		//一番差分が小さくものを消す
		if (dif_min_index >= 0) { (*output_graph).erase((*output_graph).begin() + dif_min_index); }
	}


	//重心の変化が一切ないものを追加する．
	{
		SNode same_node = current_node;

		same_node.changeNextNode(current_num, m_next_move);

		(*output_graph).emplace_back(same_node);
	}
}
