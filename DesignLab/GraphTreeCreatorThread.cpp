#include "GraphTreeCreatorThread.h"
#include <vector>
#include <memory>
#include "Define.h"

//https://stackoverflow.com/questions/5416800/how-can-i-add-boost-threads-to-a-vector

EGraphSearchResult GraphTreeCreatorThread::createGraphTree(const SNode& current_node, const MapState* const p_map, std::vector<SNode>* output_graph, int* _make_node_num)
{
	//if (DO_DEBUG_PRINT) { std::cout << "\nGraphTreeCreatorThread : 処理開始\n"; }

	////現在のノードを親にする．
	//SNode parent_node = current_node;

	//parent_node.changeParentNode();
	//output_graph.clear();				//出力する結果を空にする．
	//output_graph.push_back(parent_node);	//親を追加する．

	////深さ1のノードを生成する．
	//std::vector<SNode> depth1_nodes;

	//makeNewNodesByCurrentNode(parent_node, 0, &depth1_nodes);


	////深さ最大まで，マルチスレッドでノードを生成する．
	//boost::thread_group graph_make_threads;	//スレッドを格納するベクター

	//const size_t kDepth1NodesSize = depth1_nodes.size();	//深さ1のノードの数

	//std::vector<std::vector<SNode>> thread_output_graph(kDepth1NodesSize);	//スレッドごとの結果を格納するベクター

	////スレッドを生成する．
	//for (size_t i = 0; i < kDepth1NodesSize; ++i)
	//{
	//	graph_make_threads.create_thread(boost::bind(&GraphTreeCreatorThread::makeGraphToMaxDepth, this, depth1_nodes[i], &thread_output_graph[i]));
	//}

	////スレッドの終了を待つ．
	//graph_make_threads.join_all();

	//if (DO_DEBUG_PRINT) { std::cout << "join all : スレッドの終了を待ちました．\n"; }

	////スレッドごとの結果を結合する．
	//for (size_t i = 0; i < kDepth1NodesSize; ++i)
	//{
	//	const int kParentIndex = static_cast<int>(output_graph.size());	//親のインデックス

	//	for (auto j : thread_output_graph[i])
	//	{
	//		if (j.depth != 1)
	//		{
	//			j.parent_num += kParentIndex;
	//		}

	//		output_graph.emplace_back(j);
	//	}
	//}

	//if (DO_DEBUG_PRINT) { std::cout << "スレッドごとの結果を結合しました．\n\n"; }

	return EGraphSearchResult::Success;
}

void GraphTreeCreatorThread::makeNewNodesByCurrentNode(const SNode& current_node, const int current_node_index, std::vector<SNode>* output_graph) const
{
	(*output_graph).clear();

	if (m_node_creator_map.count(current_node.next_move) > 0)
	{
		m_node_creator_map.at(current_node.next_move)->create(current_node, current_node_index, output_graph);
		return;
	}
	else
	{
		//定義されていないならば，同じノードをそのまま追加する．
		SNode new_node = current_node;

		new_node.changeNextNode(current_node_index, current_node.next_move);

		(*output_graph).emplace_back(new_node);
	}
}