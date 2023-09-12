#include "leg_hierarchy_node_creator.h"

#include <iostream>

#include "graph_search_const.h"
#include "leg_state.h"


LegHierarchyNodeCreator::LegHierarchyNodeCreator(const MapState* const p_map, std::shared_ptr<AbstractHexapodStateCalculator> calc, const EHexapodMove next_move)
	: INodeCreator(p_map, calc, next_move)
{
	if (GraphSearchConst::DO_DEBUG_PRINT)
	{
		std::cout << "[NodeCreator] LegHierarchyNodeCreator : コンストラクタが呼ばれた\n";
	}
}


LegHierarchyNodeCreator::~LegHierarchyNodeCreator()
{
	if (GraphSearchConst::DO_DEBUG_PRINT)
	{
		std::cout << "[NodeCreator] LegHierarchyNodeCreator : デストラクタが呼ばれた\n";
	}
}


void LegHierarchyNodeCreator::create(const SNode& current_node, const int current_node_index, std::vector<SNode>* output_graph)
{
	//現在，接地している脚の本数を数える
	const int kLiftedLegNum = dl_leg::getLiftedLegNum(current_node.leg_state);

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
		SNode new_node = current_node;

		new_node.changeNextNode(current_node_index, m_next_move);		//次のノード用に，深さ・親・次の動作を更新する．
		(*output_graph).emplace_back(new_node);		//追加する．
	}
}


//全て上の関数にまとめるとごちゃつくので，以下の関数に処理を分けておく．

void LegHierarchyNodeCreator::create1LegLifted(const SNode& current_node, const int current_node_index, std::vector<SNode>* output_graph)
{
	//遊脚している脚を探す．遊脚数は1なので1つの数字が帰るはず
	std::vector<int> lifted_leg_list;

	dl_leg::getLiftedLegIndexWithVector(current_node.leg_state, &lifted_leg_list);


	// 列挙体 EDiscreteLegPos の全ての要素でループを回す．
	for (auto i : EDiscreteLegPos())
	{
		SNode new_node = current_node;		//新しい脚状態を生成する.

		dl_leg::changeLegStateKeepTopBit(lifted_leg_list[0], i, &new_node.leg_state);	//脚状態を変更する．

		new_node.changeNextNode(current_node_index, m_next_move);		//次のノード用に，深さ・親・次の動作を更新する．

		(*output_graph).emplace_back(new_node);	//追加する．
	}
}


void LegHierarchyNodeCreator::create2LegLifted(const SNode& current_node, const int current_node_index, std::vector<SNode>* output_graph)
{
	//遊脚している脚を探す．遊脚数は2なので2つの数字が帰るはず
	std::vector<int> lifted_leg_list;

	dl_leg::getLiftedLegIndexWithVector(current_node.leg_state, &lifted_leg_list);


	// 列挙体 EDiscreteLegPos の全ての要素でループを回す．
	for (auto i : EDiscreteLegPos())
	{
		for (auto j : EDiscreteLegPos())
		{
			SNode new_node = current_node;		//新しい脚状態を生成する.

			dl_leg::changeLegStateKeepTopBit(lifted_leg_list[0], i, &new_node.leg_state);			//脚状態を変更する．
			dl_leg::changeLegStateKeepTopBit(lifted_leg_list[1], j, &new_node.leg_state);

			new_node.changeNextNode(current_node_index, m_next_move);		//次のノード用に，深さ・親・次の動作を更新する．

			(*output_graph).emplace_back(new_node);	//追加する．
		}
	}
}


void LegHierarchyNodeCreator::create3LegLifted(const SNode& current_node, const int current_node_index, std::vector<SNode>* output_graph)
{
	//遊脚している脚を探す．遊脚数は3なので3つの数字が帰るはず
	std::vector<int> lifted_leg_list;

	dl_leg::getLiftedLegIndexWithVector(current_node.leg_state, &lifted_leg_list);


	// 列挙体 EDiscreteLegPos の全ての要素でループを回す．
	for (auto i : EDiscreteLegPos())
	{
		for (auto j : EDiscreteLegPos())
		{
			for (auto k : EDiscreteLegPos())
			{
				SNode new_node = current_node;		//新しい脚状態を生成する.

				dl_leg::changeLegStateKeepTopBit(lifted_leg_list[0], i, &new_node.leg_state);			//脚状態を変更する．
				dl_leg::changeLegStateKeepTopBit(lifted_leg_list[1], j, &new_node.leg_state);
				dl_leg::changeLegStateKeepTopBit(lifted_leg_list[2], k, &new_node.leg_state);

				new_node.changeNextNode(current_node_index, m_next_move);		//次のノード用に，深さ・親・次の動作を更新する．

				(*output_graph).push_back(new_node);	//追加する．
			}
		}
	}
}
