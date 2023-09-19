#include "graph_tree_creator_hato.h"

#include <iostream>

#include "graph_search_const.h"



GraphTreeCreatorHato::GraphTreeCreatorHato(std::map<EHexapodMove, std::unique_ptr<INodeCreator>>& _map) : IGraphTreeCreator(_map)
{
}


EGraphSearchResult GraphTreeCreatorHato::createGraphTree(const SNode& current_node, const MapState_Old* const p_map, std::vector<SNode>* output_graph)
{
	(*output_graph).clear();					//出力する結果を空にする．
	(*output_graph).emplace_back(current_node);	//親を追加する．


	int cnt = 0;	//カウンタを用意

	//カウンタがvectorのサイズを超えるまでループする．
	while (cnt < (*output_graph).size())
	{
		//探索深さが足りていないノードにのみ処理をする．
		if ((*output_graph)[cnt].depth < getMaxDepth())
		{
			std::vector<SNode> res_vec;	// _cnt番目のノードの子ノードを入れるベクター

			makeNewNodesByCurrentNode((*output_graph)[cnt], cnt, &res_vec);		//子ノードを生成する．

			for (const auto& i : res_vec)
			{
				(*output_graph).emplace_back(i);		//子ノードを結果に追加する．
			}
		}

		cnt++;	//カウンタを進める．
	}


	//ノード数が上限を超えていないか確認する．
	int make_node_num = static_cast<int>((*output_graph).size());

	if (GraphSearchConst::MAX_NODE_NUM < make_node_num)
	{
		return EGraphSearchResult::FailureByNodeLimitExceeded;
	}

	return EGraphSearchResult::Success;
}


void GraphTreeCreatorHato::makeNewNodesByCurrentNode(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph)
{
	(*output_graph).clear();

	if (m_node_creator_map.count(current_node.next_move) > 0)
	{
		m_node_creator_map[current_node.next_move]->create(current_node, current_num, output_graph);

		return;
	}
	else
	{
		//定義されていないならば，同じノードをそのまま追加する．
		SNode new_node = current_node;

		new_node.changeNextNode(current_num, current_node.next_move);

		(*output_graph).emplace_back(new_node);
	}
}
