#include "LegHierarchyNodeCreator.h"

#include <iostream>

#include "GraphSearchConst.h"
#include "LegState.h"


LegHierarchyNodeCreator::LegHierarchyNodeCreator(const MapState* const p_map, const EHexapodMove next_move) : INodeCreator(p_map, next_move)
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
	const int lifted_leg_num = LegStateEdit::getLiftedLegNum(current_node.leg_state);

	//遊脚している脚の本数によって処理をする
	if (lifted_leg_num == 1)
	{
		// 1 本遊脚している．
		create1LegLifted(current_node, current_node_index, output_graph);
	}
	else if (lifted_leg_num == 2)
	{
		// 2 本遊脚している．
		create2LegLifted(current_node, current_node_index, output_graph);
	}
	else if (lifted_leg_num == 3)
	{
		// 3 本遊脚している．
		create3LegLifted(current_node, current_node_index, output_graph);
	}
	else
	{
		//ここに来るのは接地している脚の数が6本 or 1本 or 2本．地面についている脚が3本を切ることはない，何故ならロボットが倒れてしまうため．
		//また6本接地しているならば脚を動かせない(遊脚する必要がある)．よって処理を行わない．(そのままの状態を次のノードにする．)
		SNode new_node = current_node;

		new_node.changeNextNode(current_node_index, m_next_move);		//次のノード用に，深さ・親・次の動作を更新する．
		(*output_graph).emplace_back(new_node);		//追加する．
	}
}


void LegHierarchyNodeCreator::create1LegLifted(const SNode& current_node, const int current_node_index, std::vector<SNode>* output_graph)
{
	//遊脚している脚を探す．遊脚数は1なので1つの数字が帰るはず
	std::vector<int> lifted_leg_list;
	LegStateEdit::getLiftedLegNumWithVector(current_node.leg_state, lifted_leg_list);

	//脚状態 0001(1) から 0111(7)まで 次のパターンを生成する．なお上位bitは遊脚を表す．(0なら遊脚)
	for (int i = 1; i <= LegStateEdit::DISCRETE_NUM; i++)
	{
		SNode new_node = current_node;		//新しい脚状態を生成する.

		LegStateEdit::changeLegState(new_node.leg_state, lifted_leg_list.at(0), i);	//脚状態を変更する．

		new_node.changeNextNode(current_node_index, m_next_move);		//次のノード用に，深さ・親・次の動作を更新する．

		(*output_graph).emplace_back(new_node);	//追加する．
	}
}

void LegHierarchyNodeCreator::create2LegLifted(const SNode& current_node, const int current_node_index, std::vector<SNode>* output_graph)
{
	//遊脚している脚を探す．遊脚数は2なので2つの数字が帰るはず
	std::vector<int> lifted_leg_list;
	LegStateEdit::getLiftedLegNumWithVector(current_node.leg_state, lifted_leg_list);

	//脚状態 0001(1) から 0111(7)まで 次のパターンを生成する．なお上位bitは遊脚を表す．(0なら遊脚)
	for (int i = 1; i <= LegStateEdit::DISCRETE_NUM; i++)
	{
		for (int j = 1; j <= LegStateEdit::DISCRETE_NUM; j++)
		{
			SNode new_node = current_node;		//新しい脚状態を生成する.

			LegStateEdit::changeLegState(new_node.leg_state, lifted_leg_list.at(0), i);			//脚状態を変更する．
			LegStateEdit::changeLegState(new_node.leg_state, lifted_leg_list.at(1), j);

			new_node.changeNextNode(current_node_index, m_next_move);		//次のノード用に，深さ・親・次の動作を更新する．

			(*output_graph).emplace_back(new_node);	//追加する．
		}
	}
}

void LegHierarchyNodeCreator::create3LegLifted(const SNode& current_node, const int current_node_index, std::vector<SNode>* output_graph)
{
	//遊脚している脚を探す．遊脚数は3なので3つの数字が帰るはず
	std::vector<int> lifted_leg_list;
	LegStateEdit::getLiftedLegNumWithVector(current_node.leg_state, lifted_leg_list);

	//脚状態 0001(1) から 0111(7)まで 次のパターンを生成する．なお上位bitは遊脚を表す．(0なら遊脚)
	for (int i = 1; i <= LegStateEdit::DISCRETE_NUM; i++)
	{
		for (int j = 1; j <= LegStateEdit::DISCRETE_NUM; j++)
		{
			for (int k = 1; k <= LegStateEdit::DISCRETE_NUM; k++)
			{
				SNode new_node = current_node;		//新しい脚状態を生成する.

				LegStateEdit::changeLegState(new_node.leg_state, lifted_leg_list.at(0), i);	//脚状態を変更する．
				LegStateEdit::changeLegState(new_node.leg_state, lifted_leg_list.at(1), j);
				LegStateEdit::changeLegState(new_node.leg_state, lifted_leg_list.at(2), k);

				new_node.changeNextNode(current_node_index, m_next_move);		//次のノード用に，深さ・親・次の動作を更新する．

				(*output_graph).push_back(new_node);	//追加する．
			}
		}
	}
}
