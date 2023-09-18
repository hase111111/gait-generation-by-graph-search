#include "pass_finder_hato_thread.h"

#include <iostream>
#include <memory>

#include <boost/thread.hpp>

#include "graph_search_const.h"
#include "designlab_cmdio.h"
#include "graph_tree_creator_hato.h"
#include "graph_searcher_hato.h"
#include "leg_up_down_node_creator.h"
#include "leg_hierarchy_node_creator.h"
#include "com_up_down_node_creator.h"
#include "com_move_node_creator_hato.h"


PassFinderHatoThread::PassFinderHatoThread(const std::shared_ptr<const AbstractHexapodStateCalculator> calculator_ptr) :
	calculator_ptr_(calculator_ptr)
{
}

EGraphSearchResult PassFinderHatoThread::getNextNodebyGraphSearch(const SNode& current_node, const MapState* const p_map, const STarget& target, SNode& output_node)
{
	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHatoThread : getNextNodebyGraphSearch() グラフ探索開始，まずは初期化する\n"; }

	//初期化処理．
	std::unique_ptr<IGraphTreeCreator> graph_tree_creator = createGraphTreeCreator(p_map, calculator_ptr_);	//!< グラフ木の作成クラス
	std::unique_ptr<AbstractGraphSearcher> graph_searcher = createGraphSearcher(calculator_ptr_);			//!< グラフ探索クラス

	//早期リターン．2つのクラスの初期化に失敗したならば，即座に終了する．
	if (!graph_tree_creator) { return EGraphSearchResult::FailureByInitializationFailed; }
	if (!graph_searcher) { return EGraphSearchResult::FailureByInitializationFailed; }


	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHatoThread : 初期化終了．グラフ探索を開始する．\n"; }

	m_graph_tree.clear();

	//まずはグラフを作成する．_graph_tree 変数に結果を参照渡しされる．
	{
		//深さ1までのグラフを作成する．
		SNode parent_node = current_node;
		parent_node.changeParentNode();
		m_graph_tree.emplace_back(parent_node);

		graph_tree_creator->setMaxDepth(1);

		std::vector<SNode> depth1_node;

		EGraphSearchResult result = graph_tree_creator->createGraphTree(parent_node, p_map, &depth1_node);

		if (!graphSeachResultIsSuccessful(result)) { return result; }

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
			tree_creators[i] = createGraphTreeCreator(p_map, calculator_ptr_);

			if (!tree_creators[i]) { return EGraphSearchResult::FailureByInitializationFailed; }

			tree_creator_threads.create_thread(boost::bind(&IGraphTreeCreator::createGraphTree, tree_creators[i].get(), depth1_node[i], p_map, &threads_result[i]));
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
		EGraphSearchResult result = graph_searcher->searchGraphTree(m_graph_tree, target, &output_node);
		if (!graphSeachResultIsSuccessful(result)) { return result; }
	}

	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHato : グラフ評価終了．グラフ探索を終了する．\n"; }

	return EGraphSearchResult::Success;
}

std::unique_ptr<IGraphTreeCreator> PassFinderHatoThread::createGraphTreeCreator(const MapState* const map, const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr_)
{
	//木を作成するクラスのマップを作成．
	std::map<EHexapodMove, std::unique_ptr<INodeCreator>> node_creator_map;
	node_creator_map.emplace(EHexapodMove::LEG_HIERARCHY_CHANGE, std::make_unique<LegHierarchyNodeCreator>(map, calculator_ptr_, EHexapodMove::LEG_UP_DOWN));
	node_creator_map.emplace(EHexapodMove::LEG_UP_DOWN, std::make_unique<LegUpDownNodeCreator>(map, calculator_ptr_, EHexapodMove::COM_UP_DOWN));
	node_creator_map.emplace(EHexapodMove::COM_UP_DOWN, std::make_unique<ComUpDownNodeCreator>(map, calculator_ptr_, EHexapodMove::COM_MOVE));
	node_creator_map.emplace(EHexapodMove::COM_MOVE, std::make_unique<ComMoveNodeCreatorHato>(map, calculator_ptr_, EHexapodMove::LEG_HIERARCHY_CHANGE));

	//木を作成するクラスと，木を探索するクラスを作成．
	std::unique_ptr<IGraphTreeCreator> p_creator = std::make_unique<GraphTreeCreatorHato>(node_creator_map);

	//完成したクラスを返す
	return std::move(p_creator);
}

std::unique_ptr<AbstractGraphSearcher> PassFinderHatoThread::createGraphSearcher(const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr_)
{
	std::unique_ptr<AbstractGraphSearcher> p_searcher = std::make_unique<GraphSearcherHato>(calculator_ptr_);
	return std::move(p_searcher);
}
