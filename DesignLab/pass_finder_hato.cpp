#include "pass_finder_hato.h"

#include "graph_search_const.h"


PassFinderHato::PassFinderHato(std::unique_ptr<AbstractPassFinderFactory>&& factory) : AbstractPassFinder(std::move(factory))
{
	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHato : コンストラクタが呼ばれた．\n"; }
};

PassFinderHato::~PassFinderHato()
{
	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHato : デストラクタが呼ばれた．\n"; }
};

EGraphSearchResult PassFinderHato::getNextNodebyGraphSearch(const SNode& current_node, const MapState* const p_map, const STarget& target, SNode& output_node)
{
	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHato : getNextNodebyGraphSearch() グラフ探索開始，まずは初期化する\n"; }

	//初期化処理．
	if (!mp_factory) { return EGraphSearchResult::FailureByInitializationFailed; }

	mp_factory->createGraphTreeCreator(p_map, mp_tree_creator);
	mp_factory->createGraphSearcher(mp_searcher);

	//早期リターン．2つのクラスの初期化に失敗したならば，即座に終了する．
	if (!mp_tree_creator) { return EGraphSearchResult::FailureByInitializationFailed; }
	if (!mp_searcher) { return EGraphSearchResult::FailureByInitializationFailed; }


	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHato : 初期化終了．グラフ探索を開始する．\n"; }

	m_graph_tree.clear();

	//まずはグラフを作成する．_graph_tree 変数に結果を参照渡しされる．
	{
		SNode parent_node = current_node;
		parent_node.changeParentNode();

		EGraphSearchResult result = mp_tree_creator->createGraphTree(parent_node, p_map, &m_graph_tree);

		if (!graphSeachResultIsSuccessful(result)) { return result; }
	}

	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHato : グラフ作成終了．グラフを評価する．\n"; }

	//次にグラフを評価して，次の動作を決定する．put_node 変数に結果を参照渡しされる．
	{
		EGraphSearchResult result = mp_searcher->searchGraphTree(m_graph_tree, target, &output_node);
		if (!graphSeachResultIsSuccessful(result)) { return result; }
	}

	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHato : グラフ評価終了．グラフ探索を終了する．\n"; }

	return EGraphSearchResult::Success;
}
