#include "GraphTreeCreatorSample.h"
#include "graph_search_const.h"

EGraphSearchResult GraphTreeCreatorSample::createGraphTree(const SNode& current_node, const MapState* const p_map, std::vector<SNode>* output_graph, int* make_node_num)
{
	//ここにグラフを作成する処理を書く．このクラスはサンプルなので動作をしないノードだけを返します．

	//現在のノードを親にする．
	SNode parent_node = current_node;

	parent_node.changeParentNode();
	(*output_graph).push_back(parent_node);

	//設定された探索深さまでの深さを持つグラフを作る．実際にグラフを作成する時もおそらくこんな感じでループする処理を書く．

	int cnt = 0;	//カウンタを用意

	//カウンタがvectorのサイズを超えるまでループする．
	while (cnt < (*output_graph).size())
	{
		//探索深さが足りていないノードのみ，処理をする．
		if ((*output_graph)[cnt].depth < getMaxDepth())
		{
			SNode new_node = (*output_graph)[cnt];

			//ここに新しい姿勢を生成する処理を書く．今回は何の処理もせずに次のノードとする．

			new_node.changeNextNode(cnt, EHexapodMove::NONE);

			//追加する．
			(*output_graph).emplace_back(new_node);
		}

		cnt++;	//カウンタを進める．
	}

	(*make_node_num) = (int)(*output_graph).size();

	if ((*make_node_num) > GraphSearchConst::MAX_NODE_NUM || (*make_node_num) < 0)
	{
		return EGraphSearchResult::FailureByNodeLimitExceeded;
	}

	return EGraphSearchResult::Success;
}
