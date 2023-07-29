#include "GraphSearcherHato.h"

#include <iostream>

#include "Define.h"
#include "GraphSearchConst.h"
#include "LegState.h"


GraphSearcherHato::GraphSearcherHato()
{
	if (GraphSearchConst::DO_DEBUG_PRINT)
	{
		std::cout << "[GraphSearcher] GraphSearcherHato : コンストラクタが呼ばれた" << std::endl;
	}
}

GraphSearcherHato::~GraphSearcherHato()
{
	if (GraphSearchConst::DO_DEBUG_PRINT)
	{
		std::cout << "[GraphSearcher] GraphSearcherHato : デストラクタが呼ばれた" << std::endl;
	}
}

EGraphSearchResult GraphSearcherHato::searchGraphTree(const std::vector<SNode>& graph, const STarget& target, SNode* output_result)
{
	if (GraphSearchConst::DO_DEBUG_PRINT)
	{
		std::cout << "[GraphSearcher] GraphSearcherHato : searchGraphTree() 探索開始\n";
	}

	// _targetの値によって，探索方法を変える必要がある．探索方法を抽象化するべき．

	// @todo initializerで初期化する処理を書く

	// ターゲットモードが直進と仮定して処理を書いている

	int result_index = -1;
	float max_move_dif = target.TargetPosition.lengthSquare();
	int max_leg_change = 0;

	const size_t kGraphSize = graph.size();
	size_t parent_num = 0;

	for (size_t i = 0; i < kGraphSize; i++)
	{
		if (graph.at(i).depth == 0)
		{
			parent_num = i;
			break;
		}
	}

	for (size_t i = 0; i < kGraphSize; i++)
	{
		//最大深さのノードのみを評価する
		if (graph.at(i).depth == Define::GRAPH_SEARCH_DEPTH)
		{
			if (result_index < 0)
			{
				result_index = i;
				max_move_dif = target.TargetPosition.lengthSquare();
				max_leg_change = LegStateEdit::getLegUpDownCount(graph.at(parent_num).leg_state, graph.at(i).leg_state);
			}

			my_vec::SVector2 move_dif = target.TargetPosition.projectedXY() - graph.at(i).global_center_of_mass.projectedXY();
			int leg_change = LegStateEdit::getLegUpDownCount(graph.at(parent_num).leg_state, graph.at(i).leg_state);

			if (max_move_dif + MARGIN_OF_MOVE > move_dif.lengthSquare())
			{
				max_move_dif = move_dif.lengthSquare();
				result_index = i;
				max_leg_change = leg_change;

			}
		}
	}

	// index が範囲外ならば失敗
	if (result_index < 0 || result_index >= kGraphSize) { return EGraphSearchResult::Failure; }

	//深さ1まで遡って値を返す
	while (graph.at(result_index).depth != 1)
	{
		result_index = graph.at(result_index).parent_num;
	}

	(*output_result) = graph.at(result_index);


	if (GraphSearchConst::DO_DEBUG_PRINT)
	{
		std::cout << "[GraphSearcher] GraphSearcherHato : searchGraphTree() 探索終了" << std::endl;
	}

	return EGraphSearchResult::Success;
}
