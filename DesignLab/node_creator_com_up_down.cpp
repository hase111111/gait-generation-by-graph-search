#include "node_creator_com_up_down.h"

#include <algorithm>
#include <cfloat>

#include "designlab_math_util.h"
#include "graph_search_const.h"
#include "hexapod_const.h"
#include "leg_state.h"
#include "phantomx_mk2_const.h"


namespace dllf = designlab::leg_func;
namespace dlm = ::designlab::math_util;


NodeCreatorComUpDown::NodeCreatorComUpDown(
	const DevideMapState& devide_map,
	const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
	const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
	const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr,
	HexapodMove next_move
) :
	map_(devide_map),
	converter_ptr_(converter_ptr),
	presenter_ptr_(presenter_ptr),
	checker_ptr_(checker_ptr),
	next_move_(next_move)
{
}


void NodeCreatorComUpDown::Create(const RobotStateNode& current_node, const int current_num, std::vector<RobotStateNode>* output_graph) const
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

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		//脚の先端の座標を求める．
		const designlab::Vector3 kCoxaVec = converter_ptr_->ConvertRobotToGlobalCoordinate(
			presenter_ptr_->GetLegBasePosRobotCoodinate(i), current_node.global_center_of_mass, current_node.quat, true
		);

		if (map_.IsInMap(kCoxaVec)) 
		{
			const int kCoxaX = map_.GetDevideMapIndexX(kCoxaVec.x);
			const int kCoxaY = map_.GetDevideMapIndexY(kCoxaVec.y);
			map_highest_z = (std::max)(map_.GetTopZ(kCoxaX, kCoxaY), map_highest_z);
		}
	}


	//ロボットの重心の最も低く下げることのできるz座標と，高くあげることができるz座標を求める．どちらもグローバル座標．
	float highest_body_zpos = map_highest_z + presenter_ptr_->GetGroundHeightMarginMax();
	float lowest_body_zpos = map_highest_z + presenter_ptr_->GetGroundHeightMarginMin();


	// 最も高い地点を修正する．

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		//接地している脚についてのみ考える．
		if (dllf::IsGrounded(current_node.leg_state, i))
		{
			//三平方の定理を使って，脚接地地点から重心位置をどれだけ上げられるか考える．
			const float edge_c = PhantomXMkIIConst::kFemurLength + PhantomXMkIIConst::kTibiaLength - MARGIN;
			const float edge_b = current_node.leg_pos[i].ProjectedXY().GetLength() - PhantomXMkIIConst::kCoxaLength;

			const float edge_a = sqrt(dlm::Squared(edge_c) - dlm::Squared(edge_b));

			//接地脚の最大重心高さの中から一番小さいものを全体の最大重心位置として記録する．_aは脚の接地点からどれだけ上げられるかを表しているので，グローバル座標に変更する．
			highest_body_zpos = (std::min)(edge_a + current_node.global_center_of_mass.z + current_node.leg_pos[i].z, highest_body_zpos);
		}
	}


	//ノードを追加する．
	pushNodeByMaxAndMinPosZ(current_node, current_num, highest_body_zpos, lowest_body_zpos, output_graph);
}


void NodeCreatorComUpDown::pushNodeByMaxAndMinPosZ(const RobotStateNode& current_node, const int current_num, const float high, const float low, std::vector<RobotStateNode>* output_graph) const
{
	//重心を変化させたものを追加する．変化量が一番少ないノードは削除する．
	{
		//最大と最小の間を分割する．
		const float kDivZ = (high - low) / (float)DISCRETIZATION;


		//分割した分新しいノードを追加する．
		for (int i = 0; i < DISCRETIZATION + 1; i++)
		{
			bool is_vaild = true;

			RobotStateNode new_node = current_node;

			//重心の位置を変更する．
			designlab::Vector3 new_com = current_node.global_center_of_mass;
			new_com.z = low + kDivZ * i;

			new_node.ChangeGlobalCenterOfMass(new_com, true);


			for (int j = 0; j < HexapodConst::kLegNum; j++)
			{
				if (!checker_ptr_->IsLegInRange(j, new_node.leg_pos[j])) { is_vaild = false; }
			}

			//current_numを親とする，新しいノードに変更する
			new_node.ChangeToNextNode(current_num, next_move_);

			//ノードを追加する．
			if (is_vaild)
			{
				(*output_graph).emplace_back(new_node);
			}
		}

	}


	//重心の変化が一切ないものを追加する．
	{
		RobotStateNode same_node = current_node;

		same_node.ChangeToNextNode(current_num, next_move_);

		(*output_graph).emplace_back(same_node);
	}
}
