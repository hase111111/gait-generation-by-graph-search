#include "PassFinderHatoThread.h"
#include <iostream>
#include <boost/thread.hpp>
#include "GraphSearchConst.h"


PassFinderHatoThread::PassFinderHatoThread(std::unique_ptr<AbstractPassFinderFactory>&& _factory) : IPassFinder(std::move(_factory))
{
	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHatoThread : コンストラクタが呼ばれた\n"; }
};

EGraphSearchResult PassFinderHatoThread::getNextNodebyGraphSearch(const SNode& current_node, const MapState* const p_map, const STarget& target, SNode& output_node)
{
	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHatoThread : getNextNodebyGraphSearch() グラフ探索開始，まずは初期化する\n"; }

	//初期化処理．
	if (!mp_factory) { return EGraphSearchResult::FailureByInitializationFailed; }

	mp_factory->createGraphTreeCreator(p_map, mp_tree_creator);
	mp_factory->createGraphSearcher(mp_searcher);

	//早期リターン．2つのクラスの初期化に失敗したならば，即座に終了する．
	if (!mp_tree_creator) { return EGraphSearchResult::FailureByInitializationFailed; }
	if (!mp_searcher) { return EGraphSearchResult::FailureByInitializationFailed; }


	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHatoThread : 初期化終了．グラフ探索を開始する．\n"; }

	m_graph_tree.clear();

	//まずはグラフを作成する．_graph_tree 変数に結果を参照渡しされる．
	{
		//深さ1までのグラフを作成する．
		SNode parent_node = current_node;
		parent_node.changeParentNode();
		m_graph_tree.emplace_back(parent_node);

		mp_tree_creator->setMaxDepth(1);

		std::vector<SNode> depth1_node;

		EGraphSearchResult result = mp_tree_creator->createGraphTree(parent_node, p_map, &depth1_node, &m_made_node_num);
		if (graphSeachResultIsSuccessful(result) == false) { return result; }

		depth1_node.erase(depth1_node.begin());	//深さ0のノードは親ノードと同じなので，削除する．

		if (GraphSearchConst::DO_DEBUG_PRINT)
		{
			std::cout << "\n[PassFinder] PassFinderHatoThread : 深さ1のノードは" << depth1_node.size() << "個\n";
			std::cout << "[PassFinder] PassFinderHatoThread : スレッドを" << depth1_node.size() << "個作成します\n\n";
		}

		//深さ1のノードを親にして，続きのグラフを作成する．
		const size_t kDepth1NodeNum = depth1_node.size();
		std::vector<std::unique_ptr<IGraphTreeCreator>> tree_creators(kDepth1NodeNum);
		std::vector<std::vector<SNode>> threads_result(kDepth1NodeNum);
		boost::thread_group tree_creator_threads;

		for (size_t i = 0; i < kDepth1NodeNum; i++)
		{
			mp_factory->createGraphTreeCreator(p_map, tree_creators[i]);
			if (!tree_creators[i]) { return EGraphSearchResult::FailureByInitializationFailed; }
			tree_creator_threads.create_thread(boost::bind(&IGraphTreeCreator::createGraphTree, tree_creators[i].get(), depth1_node[i], p_map, &threads_result[i], &m_made_node_num));
		}

		tree_creator_threads.join_all();

		if (GraphSearchConst::DO_DEBUG_PRINT)
		{
			std::cout << "\n[PassFinder] PassFinderHatoThread : スレッド終了\n";

			for (size_t i = 0; i < kDepth1NodeNum; i++)
			{
				std::cout << "[PassFinder] PassFinderHatoThread : 子ノード数" << threads_result[i].size() << "\n";
			}
		}

		//スレッドごとの結果を結合する．
		for (size_t i = 0; i < kDepth1NodeNum; ++i)
		{
			const int kParentIndex = static_cast<int>(m_graph_tree.size());	//親のインデックス

			for (auto j : threads_result[i])
			{
				if (j.depth != 1)
				{
					j.parent_num += kParentIndex;
				}

				m_graph_tree.emplace_back(j);
			}
		}

		if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHatoThread : 統合完了．全部で" << m_graph_tree.size() << "個\n"; }
	}

	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHato : グラフ作成終了．グラフを評価する．\n"; }

	//次にグラフを評価して，次の動作を決定する．put_node 変数に結果を参照渡しされる．
	{
		EGraphSearchResult result = mp_searcher->searchGraphTree(m_graph_tree, target, &output_node);
		if (graphSeachResultIsSuccessful(result) == false) { return result; }
	}

	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHato : グラフ評価終了．グラフ探索を終了する．\n"; }

	return EGraphSearchResult::Success;
}
