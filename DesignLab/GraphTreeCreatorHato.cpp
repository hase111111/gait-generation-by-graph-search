#include "GraphTreeCreatorHato.h"

#include <iostream>

#include "GraphSearchConst.h"


GraphTreeCreatorHato::GraphTreeCreatorHato(std::map<EHexapodMove, std::unique_ptr<INodeCreator>>& _map) : IGraphTreeCreator(_map)
{
	if (GraphSearchConst::DO_DEBUG_PRINT)
	{
		std::cout << "[GraphTreeCreator] GraphTreeCreatorHato : コンストラクタが呼ばれた．\n";
	}
}

GraphTreeCreatorHato::~GraphTreeCreatorHato()
{
	if (GraphSearchConst::DO_DEBUG_PRINT)
	{
		std::cout << "[GraphTreeCreator] GraphTreeCreatorHato : デストラクタが呼ばれた．\n";
	}
}

EGraphSearchResult GraphTreeCreatorHato::createGraphTree(const SNode& current_node, const MapState* const p_map, std::vector<SNode>* output_graph, int* make_node_num)
{
	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "[GraphTreeCreator] GraphTreeCreatorHato : createGraphTree() グラフ作成開始\n"; }

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

	(*make_node_num) = (int)(*output_graph).size();

	if ((*make_node_num) > GraphSearchConst::MAX_NODE_NUM || (*make_node_num) < 0)
	{
		return EGraphSearchResult::FailureByNodeLimitExceeded;
	}

	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "[GraphTreeCreator] GraphTreeCreatorHato : createGraphTree() グラフ作成終了\n"; }

	return EGraphSearchResult::Success;
}


void GraphTreeCreatorHato::makeNewNodesByCurrentNode(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph)
{
	(*output_graph).clear();

	if (m_node_creator_map.count(current_node.next_move) > 0)
	{
		m_node_creator_map.at(current_node.next_move)->create(current_node, current_num, output_graph);
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
