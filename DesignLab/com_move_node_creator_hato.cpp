#include "com_move_node_creator_hato.h"

#include "designlab_math_util.h"
#include "graph_search_const.h"
#include "leg_state.h"


namespace dllf = designlab::leg_func;
namespace dlm = designlab::math_util;


ComMoveNodeCreatorHato::ComMoveNodeCreatorHato(const DevideMapState& map, const std::shared_ptr<const AbstractHexapodStateCalculator>& calc, const HexapodMove next_move) :
	kStableMargin(15.0f),
	map_(map), 
	calculator_ptr_(calc), 
	maker_(calc),
	selecter_(calc),
	next_move_(next_move)
{
}


void ComMoveNodeCreatorHato::Create(const RobotStateNode& current_node, const int current_num, std::vector<RobotStateNode>* output_graph)
{
	std::array<ComPosAndPolygon, ComCandidatePolygonMaker::MAKE_POLYGON_NUM> candidate_polygons;

	//重心移動先の候補地点の範囲を示す多角形を作成する
	maker_.MakeCandidatePolygon(current_node, &candidate_polygons);

	//候補範囲から実際に移動する先の座標を選択する
	selecter_.SetCurrentNode(current_node);

	for (int i = 0; i < ComCandidatePolygonMaker::MAKE_POLYGON_NUM; ++i)
	{
		//そもそも多角形が候補点になりえないならば，その多角形は無視する
		if (!candidate_polygons[i].is_able) { continue; }

		designlab::Vector3 result_com;

		if (selecter_.GetComFromPolygon(candidate_polygons[i].polygon, &result_com))
		{
			RobotStateNode next_node = current_node;

			next_node.ChangeGlobalCenterOfMass(result_com, false);					//重心位置を変更し，それに伴い接地脚の位置も変更する

			dllf::ChangeDiscreteComPos(candidate_polygons[i].com_pos, &next_node.leg_state);		//leg_stateのcom_patternを変更する

			for (int j = 0; j < HexapodConst::kLegNum; ++j)
			{
				dllf::ChangeDiscreteLegPos(j, DiscreteLegPos::kCenter, &next_node.leg_state);
			}

			next_node.ChangeToNextNode(current_num, next_move_);	//深さや親ノードを変更する

			if (IsStable(next_node) && !IsIntersectGround(next_node))
			{
				(*output_graph).push_back(next_node);
			}
		}
	}
}


bool ComMoveNodeCreatorHato::IsStable(const RobotStateNode& node) const
{
	//重心を原点とした座標系で，脚の位置を計算する．

	if (calculator_ptr_->CalculateStabilityMargin(node.leg_state, node.leg_pos) < kStableMargin)
	{
		return false;
	}
	else
	{
		return true;
	}
}


bool ComMoveNodeCreatorHato::IsIntersectGround(const RobotStateNode& node) const
{
	float top_z = -10000.0f;	//地面との交点のうち最も高いものを格納する

	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		const designlab::Vector3 kCoxaPos = calculator_ptr_->GetGlobalLegBasePosition(i, node.global_center_of_mass, node.rot, false);	//脚の根元の座標(グローバル)を取得する

		if (map_.IsInMap(kCoxaPos)) 
		{
			const float kMapTopZ = map_.GetTopZ(map_.GetDevideMapIndexX(kCoxaPos.x), map_.GetDevideMapIndexY(kCoxaPos.y));

			top_z = (std::max)(top_z, kMapTopZ);	//最も高い点を求める		
		}
	}

	if (top_z + HexapodConst::VERTICAL_MIN_RANGE - dlm::kAllowableError < node.global_center_of_mass.z)
	{
		return false;
	}

	return true;
}
