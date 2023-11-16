#include "node_creator_com_move.h"

#include "designlab_math_util.h"
#include "graph_search_const.h"
#include "leg_state.h"


namespace dllf = designlab::leg_func;
namespace dlm = designlab::math_util;


NodeCreatorComMove::NodeCreatorComMove(
	const DevideMapState& map, 
	const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
	const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
	const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr,
	const HexapodMove next_move
) :
	map_(map), 
	maker_(converter_ptr),
	selecter_(checker_ptr),
	next_move_(next_move),
	converter_ptr_(converter_ptr),
	presenter_ptr_(presenter_ptr),
	checker_ptr_(checker_ptr)
{
}


void NodeCreatorComMove::Create(const RobotStateNode& current_node, const int current_num, std::vector<RobotStateNode>* output_graph) const
{
	std::array<ComPosAndPolygon, ComCandidatePolygonMaker::MAKE_POLYGON_NUM> candidate_polygons;

	//重心移動先の候補地点の範囲を示す多角形を作成する
	maker_.MakeCandidatePolygon(current_node, &candidate_polygons);

	//候補範囲から実際に移動する先の座標を選択する
	for (int i = 0; i < ComCandidatePolygonMaker::MAKE_POLYGON_NUM; ++i)
	{
		//そもそも多角形が候補点になりえないならば，その多角形は無視する
		if (!candidate_polygons[i].is_able) { continue; }

		designlab::Vector3 result_com;

		if (selecter_.GetComFromPolygon(candidate_polygons[i].polygon, current_node, &result_com))
		{
			RobotStateNode next_node = current_node;

			next_node.ChangeGlobalCenterOfMass(result_com, false);					//重心位置を変更し，それに伴い接地脚の位置も変更する

			dllf::ChangeDiscreteComPos(candidate_polygons[i].com_pos, &next_node.leg_state);		//leg_stateのcom_patternを変更する

			for (int j = 0; j < HexapodConst::kLegNum; ++j)
			{
				dllf::ChangeDiscreteLegPos(j, DiscreteLegPos::kCenter, &next_node.leg_state);
			}

			next_node.ChangeToNextNode(current_num, next_move_);	//深さや親ノードを変更する

			if (
				checker_ptr_->IsStable(next_node.leg_state, next_node.leg_pos) &&
				! checker_ptr_->IsBodyInterferingWithGround(next_node, map_)
			)
			{
				(*output_graph).push_back(next_node);
			}
		}
	}
}