#include "pass_finder_hato.h"

#include "graph_search_const.h"



EGraphSearchResult PassFinderHato::getNextNodebyGraphSearch(const SNode& current_node, const MapState* const p_map, const STarget& target, SNode& output_node)
{
	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHato : getNextNodebyGraphSearch() グラフ探索開始，まずは初期化する\n"; }

	//初期化処理．
	if (!mp_factory) { return EGraphSearchResult::FailureByInitializationFailed; }

	std::unique_ptr<IGraphTreeCreator> graph_tree_creator;	//!< グラフ木の作成クラス
	std::unique_ptr<IGraphSearcher> graph_searcher;		//!< グラフ探索クラス

	mp_factory->createGraphTreeCreator(p_map, mp_calculator, graph_tree_creator);
	mp_factory->createGraphSearcher(graph_searcher);

	m_graph_tree.clear();

	//早期リターン．2つのクラスの初期化に失敗したならば，即座に終了する．
	if (!graph_tree_creator) { return EGraphSearchResult::FailureByInitializationFailed; }
	if (!graph_searcher) { return EGraphSearchResult::FailureByInitializationFailed; }


	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHato : 初期化終了．グラフ探索を開始する．\n"; }



	//まずはグラフを作成する．_graph_tree 変数に結果を参照渡しされる．
	{
		SNode parent_node = current_node;
		parent_node.changeParentNode();

		EGraphSearchResult result = graph_tree_creator->createGraphTree(parent_node, p_map, &m_graph_tree);

		if (!graphSeachResultIsSuccessful(result)) { return result; }
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
