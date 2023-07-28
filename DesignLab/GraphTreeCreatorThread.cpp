#include "GraphTreeCreatorThread.h"
#include <boost/thread.hpp>
#include <vector>
#include <array>
#include <memory>
#include "Define.h"

//https://stackoverflow.com/questions/5416800/how-can-i-add-boost-threads-to-a-vector

EGraphSearchResult GraphTreeCreatorThread::createGraphTree(const SNode& currentNode, const MapState* const pointerMap, std::vector<SNode>& outputGraph, int& _make_node_num)
{
	//現在のノードを親にする．
	SNode parentNode = currentNode;

	parentNode.changeParentNode();
	outputGraph.clear();				//出力する結果を空にする．
	outputGraph.push_back(parentNode);	//親を追加する．

	//深さ1のノードを生成する．
	std::vector<SNode> depth1Nodes;

	makeNewNodesByCurrentNode(parentNode, 0, depth1Nodes);


	//深さ最大まで，マルチスレッドでノードを生成する．
	boost::thread_group graphMakerGroup;	//スレッドを格納するベクター

	const size_t depth1NodesSize = depth1Nodes.size();	//深さ1のノードの数

	std::vector<std::vector<SNode>> threadOutputGraph(depth1NodesSize);	//スレッドごとの結果を格納するベクター


	//スレッドを生成する．
	for (size_t i = 0; i < depth1NodesSize; ++i)
	{
		graphMakerGroup.create_thread(boost::bind(&GraphTreeCreatorThread::makeGraphToMaxDepth, this, depth1Nodes[i], &threadOutputGraph[i]));
	}

	//スレッドの終了を待つ．
	graphMakerGroup.join_all();

	//スレッドごとの結果を結合する．
	for (size_t i = 0; i < depth1NodesSize; ++i)
	{
		const int parentIndex = static_cast<int>(outputGraph.size());	//親のインデックス

		for (auto& j : threadOutputGraph[i])
		{
			if (j.depth != 1)
			{
				j.parent_num = parentIndex + j.parent_num;
			}

			outputGraph.push_back(j);
		}
	}

	return EGraphSearchResult::Success;
}

void GraphTreeCreatorThread::makeNewNodesByCurrentNode(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph)
{
	_output_graph.clear();

	if (m_node_creator_map.count(_current_node.next_move) > 0)
	{
		m_node_creator_map.at(_current_node.next_move)->create(_current_node, _current_num, _output_graph);
		return;
	}
	else
	{
		//定義されていないならば，同じノードをそのまま追加する．
		SNode _new_node = _current_node;

		_new_node.changeNextNode(_current_num, _current_node.next_move);

		_output_graph.push_back(_new_node);
	}
}

void GraphTreeCreatorThread::makeGraphToMaxDepth(const SNode& currentNode, std::vector<SNode>* outputGraph)
{
	int cnt = 0;	//カウンタを用意

	(*outputGraph).push_back(currentNode);	//親を追加する．

	//カウンタがvectorのサイズを超えるまでループする．
	while (cnt < (*outputGraph).size())
	{
		//探索深さが足りていないノードにのみ処理をする．
		if ((*outputGraph)[cnt].depth < Define::GRAPH_SEARCH_DEPTH)
		{
			std::vector<SNode> resultGraph;	// _cnt番目のノードの子ノードを入れるベクター

			makeNewNodesByCurrentNode((*outputGraph)[cnt], cnt, resultGraph);		//子ノードを生成する．

			for (const auto& i : resultGraph)
			{
				(*outputGraph).push_back(i);		//子ノードを結果に追加する．
			}
		}

		cnt++;	//カウンタを進める．
	}
}
