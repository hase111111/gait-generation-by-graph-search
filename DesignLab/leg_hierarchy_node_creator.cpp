#include "leg_hierarchy_node_creator.h"

#include "graph_search_const.h"
#include "leg_state.h"


namespace dllf = designlab::leg_func;


LegHierarchyNodeCreator::LegHierarchyNodeCreator(const HexapodMove next_move) : 
	next_move_(next_move),
	discrete_leg_pos_list_({ 
		DiscreteLegPos::kBack,
		DiscreteLegPos::kCenter,
		DiscreteLegPos::kFront,
		DiscreteLegPos::kLowerBack,
		DiscreteLegPos::kLowerFront,
		DiscreteLegPos::kUpperBack,
		DiscreteLegPos::kUpperFront 
	})
{
}


void LegHierarchyNodeCreator::Create(const RobotStateNode& current_node, const int current_node_index, std::vector<RobotStateNode>* output_graph) const
{
	//現在，接地している脚の本数を数える
	const int kLiftedLegNum = dllf::GetLiftedLegNum(current_node.leg_state);

	//遊脚している脚の本数によって処理をする
	if (kLiftedLegNum == 1)
	{
		// 1 本遊脚している．
		create1LegLifted(current_node, current_node_index, output_graph);
	}
	else if (kLiftedLegNum == 2)
	{
		// 2 本遊脚している．
		create2LegLifted(current_node, current_node_index, output_graph);
	}
	else if (kLiftedLegNum == 3)
	{
		// 3 本遊脚している．
		create3LegLifted(current_node, current_node_index, output_graph);
	}
	else
	{
		//ここに来るのは接地している脚の数が6本 or 1本 or 2本．地面についている脚が3本を切ることはない，何故ならロボットが倒れてしまうため．
		// 
		//また6本接地しているならば脚を動かせない(遊脚する必要がある)．よって処理を行わない．(そのままの状態を次のノードにする．)
		RobotStateNode new_node = current_node;

		new_node.ChangeToNextNode(current_node_index, next_move_);		//次のノード用に，深さ・親・次の動作を更新する．
		(*output_graph).emplace_back(new_node);		//追加する．
	}
}


//全て上の関数にまとめるとごちゃつくので，以下の関数に処理を分けておく．

void LegHierarchyNodeCreator::create1LegLifted(const RobotStateNode& current_node, const int current_node_index, std::vector<RobotStateNode>* output_graph) const
{
	//遊脚している脚を探す．遊脚数は1なので1つの数字が帰るはず
	std::vector<int> lifted_leg_list;

	dllf::GetLiftedLegIndexByVector(current_node.leg_state, &lifted_leg_list);


	// 列挙体 DiscreteLegPos の全ての要素でループを回す．
	for (const auto i : discrete_leg_pos_list_)
	{
		RobotStateNode new_node = current_node;		//新しい脚状態を生成する.

		dllf::ChangeDiscreteLegPos(lifted_leg_list[0], i, &new_node.leg_state);	//脚状態を変更する．

		new_node.ChangeToNextNode(current_node_index, next_move_);		//次のノード用に，深さ・親・次の動作を更新する．

		(*output_graph).emplace_back(new_node);	//追加する．
	}
}


void LegHierarchyNodeCreator::create2LegLifted(const RobotStateNode& current_node, const int current_node_index, std::vector<RobotStateNode>* output_graph) const
{
	//遊脚している脚を探す．遊脚数は2なので2つの数字が帰るはず
	std::vector<int> lifted_leg_list;

	dllf::GetLiftedLegIndexByVector(current_node.leg_state, &lifted_leg_list);


	// 列挙体 DiscreteLegPos の全ての要素でループを回す．
	for (const auto i : discrete_leg_pos_list_)
	{
		for (const auto j : discrete_leg_pos_list_)
		{
			RobotStateNode new_node = current_node;		//新しい脚状態を生成する.

			dllf::ChangeDiscreteLegPos(lifted_leg_list[0], i, &new_node.leg_state);			//脚状態を変更する．
			dllf::ChangeDiscreteLegPos(lifted_leg_list[1], j, &new_node.leg_state);

			new_node.ChangeToNextNode(current_node_index, next_move_);		//次のノード用に，深さ・親・次の動作を更新する．

			(*output_graph).emplace_back(new_node);	//追加する．
		}
	}
}


void LegHierarchyNodeCreator::create3LegLifted(const RobotStateNode& current_node, const int current_node_index, std::vector<RobotStateNode>* output_graph) const
{
	//遊脚している脚を探す．遊脚数は3なので3つの数字が帰るはず
	std::vector<int> lifted_leg_list;

	dllf::GetLiftedLegIndexByVector(current_node.leg_state, &lifted_leg_list);


	// 列挙体 DiscreteLegPos の全ての要素でループを回す．
	for (const auto i : discrete_leg_pos_list_)
	{
		for (const auto j : discrete_leg_pos_list_)
		{
			for (const auto k : discrete_leg_pos_list_)
			{
				RobotStateNode new_node = current_node;		//新しい脚状態を生成する.

				dllf::ChangeDiscreteLegPos(lifted_leg_list[0], i, &new_node.leg_state);			//脚状態を変更する．
				dllf::ChangeDiscreteLegPos(lifted_leg_list[1], j, &new_node.leg_state);
				dllf::ChangeDiscreteLegPos(lifted_leg_list[2], k, &new_node.leg_state);

				new_node.ChangeToNextNode(current_node_index, next_move_);		//次のノード用に，深さ・親・次の動作を更新する．

				(*output_graph).push_back(new_node);	//追加する．
			}
		}
	}
}
