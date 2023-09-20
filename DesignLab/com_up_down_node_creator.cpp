#include "com_up_down_node_creator.h"

#include <algorithm>
#include <cfloat>

#include "designlab_math.h"
#include "graph_search_const.h"
#include "hexapod_const.h"
#include "leg_state.h"


ComUpDownNodeCreator::ComUpDownNodeCreator(const DevideMapState& map, const std::shared_ptr<const AbstractHexapodStateCalculator>& calc, const EHexapodMove next_move) :
	map_(map),
	calclator_(calc),
	next_move_(next_move)
{
}


void ComUpDownNodeCreator::Create(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph)
{
	//重心を最も高くあげることのできる位置と，最も低く下げることのできる位置を求める．グローバル座標で Zの位置．
	//マップを確認して地面の最高点を求め，そこからMAX_RANGE，MIN_RANGEの分だけ離す．


	//マップの最大z座標を求める．
	float map_highest_z = -100000;

	if (map_.IsInMap(current_node.global_center_of_mass)) 
	{
		const int kMapX = map_.GetDevideMapIndexX(current_node.global_center_of_mass.x);
		const int kMapY = map_.GetDevideMapIndexY(current_node.global_center_of_mass.y);
		map_highest_z = map_.GetTopZ(kMapX, kMapY);
	}

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		const designlab::Vector3 kCoxaVec = calclator_->getGlobalLegBasePosition(i, current_node.global_center_of_mass, current_node.rot, false);

		if (map_.IsInMap(kCoxaVec)) 
		{
			const int kCoxaX = map_.GetDevideMapIndexX(kCoxaVec.x);
			const int kCoxaY = map_.GetDevideMapIndexY(kCoxaVec.y);
			map_highest_z = (std::max)(map_.GetTopZ(kCoxaX, kCoxaY), map_highest_z);
		}
	}


	//ロボットの重心の最も低く下げることのできるz座標と，高くあげることができるz座標を求める．どちらもグローバル座標．
	float highest_body_zpos = map_highest_z + HexapodConst::VERTICAL_MAX_RANGE;
	float lowest_body_zpos = map_highest_z + HexapodConst::VERTICAL_MIN_RANGE;


	// 最も高い地点を修正する．

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		//接地している脚についてのみ考える．
		if (dl_leg::isGrounded(current_node.leg_state, i))
		{
			//三平方の定理を使って，脚接地地点から重心位置をどれだけ上げられるか考える．
			const float edge_c = HexapodConst::PHANTOMX_FEMUR_LENGTH + HexapodConst::PHANTOMX_TIBIA_LENGTH - MARGIN;
			const float edge_b = current_node.leg_pos[i].ProjectedXY().Length() - HexapodConst::PHANTOMX_COXA_LENGTH;

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


		//分割した分新しいノードを追加する．
		for (int i = 0; i < DISCRETIZATION + 1; i++)
		{
			bool is_vaild = true;

			SNode new_node = current_node;

			//重心の位置を変更する．
			designlab::Vector3 new_com = current_node.global_center_of_mass;
			new_com.z = low + kDivZ * i;

			new_node.changeGlobalCenterOfMass(new_com, true);


			for (int j = 0; j < HexapodConst::LEG_NUM; j++)
			{
				if (!calclator_->isLegInRange(j, new_node.leg_pos[j])) { is_vaild = false; }
			}

			//current_numを親とする，新しいノードに変更する
			new_node.changeNextNode(current_num, next_move_);

			//ノードを追加する．
			if (is_vaild)
			{
				(*output_graph).emplace_back(new_node);
			}
		}

	}


	//重心の変化が一切ないものを追加する．
	{
		SNode same_node = current_node;

		same_node.changeNextNode(current_num, next_move_);

		(*output_graph).emplace_back(same_node);
	}
}
