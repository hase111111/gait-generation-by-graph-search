#include "graph_tree_creator_hato.h"

#include <iostream>

#include "cassert_define.h"
#include "graph_search_const.h"



GraphTreeCreatorHato::GraphTreeCreatorHato(
	std::unique_ptr<INodeCreatorBuilder>&& node_creator_builder_ptr,
	const std::shared_ptr<const AbstractHexapodStateCalculator> calculator_ptr
) :
	node_creator_builder_ptr_(std::move(node_creator_builder_ptr)),
	calculator_ptr_(calculator_ptr)
{
}


void GraphTreeCreatorHato::Init(const DevideMapState& map_state)
{
	node_creator_map_.clear();

	node_creator_builder_ptr_->Build(map_state, calculator_ptr_, &node_creator_map_);
}

EGraphSearchResult GraphTreeCreatorHato::CreateGraphTree(const SNode& current_node, const int max_depth, std::vector<SNode>* output_graph)
{
	assert(output_graph != nullptr);	//nullptrでない．
	assert(output_graph->empty());		//空である．
	assert(current_node.depth == 0);	//深さが0である．

	output_graph->emplace_back(current_node);	//親を追加する．


	int cnt = 0;	//カウンタを用意

	//カウンタがvectorのサイズを超えるまでループする．
	while (cnt < output_graph->size())
	{
		//探索深さが足りていないノードにのみ処理をする．
		if ((*output_graph)[cnt].depth < max_depth)
		{
			std::vector<SNode> res_vec;	// _cnt番目のノードの子ノードを入れるベクター

			makeNewNodesByCurrentNode((*output_graph)[cnt], cnt, &res_vec);		//子ノードを生成する．

			for (const auto& i : res_vec)
			{
				output_graph->emplace_back(i);		//子ノードを結果に追加する．
			}
		}

		cnt++;	//カウンタを進める．
	}


	//ノード数が上限を超えていないか確認する．
	int make_node_num = static_cast<int>(output_graph->size());

	if (GraphSearchConst::MAX_NODE_NUM < make_node_num)
	{
		return EGraphSearchResult::FailureByNodeLimitExceeded;
	}

	return EGraphSearchResult::Success;
}


void GraphTreeCreatorHato::makeNewNodesByCurrentNode(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph) const
{
	(*output_graph).clear();

	if (node_creator_map_.count(current_node.next_move) > 0)
	{
		node_creator_map_.at(current_node.next_move)->Create(current_node, current_num, output_graph);

		return;
	}
	else
	{
		assert(false);

		//定義されていないならば，同じノードをそのまま追加する．
		SNode new_node = current_node;

		new_node.ChangeToNextNode(current_num, current_node.next_move);

		(*output_graph).emplace_back(new_node);
	}
}


//std::unique_ptr<IGraphTreeCreator> graph_tree_creator = createGraphTreeCreator(devide_map_, calculator_ptr_);	//!< グラフ木の作成クラス
//std::unique_ptr<IGraphSearcher> graph_searcher = createGraphSearcher(calculator_ptr_);			//!< グラフ探索クラス




//if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderBasic : 初期化終了．グラフ探索を開始する．\n"; }

//m_graph_tree.clear();

//まずはグラフを作成する．_graph_tree 変数に結果を参照渡しされる．
//{
//	//深さ1までのグラフを作成する．
//	SNode parent_node = current_node;
//	parent_node.ChangeParentNode();
//	m_graph_tree.emplace_back(parent_node);

//	std::vector<SNode> depth1_node;

//	EGraphSearchResult result = graph_tree_creator->CreateGraphTree(parent_node, 1, &depth1_node);

//	if (!graphSeachResultIsSuccessful(result)) { return result; }

//	depth1_node.erase(depth1_node.begin());	//深さ0のノードは親ノードと同じなので，削除する．

//	if (GraphSearchConst::DO_DEBUG_PRINT)
//	{
//		std::cout << "\n[PassFinder] PassFinderBasic : 深さ1のノードは" << depth1_node.size() << "個\n";
//		std::cout << "[PassFinder] PassFinderBasic : スレッドを" << depth1_node.size() << "個作成します\n\n";
//	}

//	//深さ1のノードを親にして，続きのグラフを作成する．
//	const size_t kDepth1NodeNum = depth1_node.size();
//	std::vector<std::unique_ptr<IGraphTreeCreator>> tree_creators(kDepth1NodeNum);
//	std::vector<std::vector<SNode>> threads_result(kDepth1NodeNum);
//	boost::thread_group tree_creator_threads;

//	for (size_t i = 0; i < kDepth1NodeNum; i++)
//	{
//		tree_creators[i] = createGraphTreeCreator(devide_map_, calculator_ptr_);

//		if (!tree_creators[i]) { return EGraphSearchResult::FailureByInitializationFailed; }

//		tree_creator_threads.create_thread(boost::bind(&IGraphTreeCreator::CreateGraphTree, tree_creators[i].get(), depth1_node[i], GraphSearchConst::MAX_DEPTH, &threads_result[i]));
//	}

//	tree_creator_threads.join_all();

//	if (GraphSearchConst::DO_DEBUG_PRINT)
//	{
//		std::cout << "\n[PassFinder] PassFinderBasic : スレッド終了\n";

//		for (size_t i = 0; i < kDepth1NodeNum; i++)
//		{
//			std::cout << "[PassFinder] PassFinderBasic : 子ノード数" << threads_result[i].size() << "\n";
//		}
//	}

//	//スレッドごとの結果を結合する．
//	for (size_t i = 0; i < kDepth1NodeNum; ++i)
//	{
//		const int kParentIndex = static_cast<int>(m_graph_tree.size());	//親のインデックス

//		for (auto j : threads_result[i])
//		{
//			if (j.depth != 1)
//			{
//				j.parent_num += kParentIndex;
//			}

//			m_graph_tree.emplace_back(j);
//		}
//	}

//	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderBasic : 統合完了．全部で" << m_graph_tree.size() << "個\n"; }
//}

//if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHato : グラフ作成終了．グラフを評価する．\n"; }

////次にグラフを評価して，次の動作を決定する．put_node 変数に結果を参照渡しされる．
//{
//	EGraphSearchResult result = graph_searcher->SearchGraphTree(m_graph_tree, target, &output_node);
//	if (!graphSeachResultIsSuccessful(result)) { return result; }
//}

//if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHato : グラフ評価終了．グラフ探索を終了する．\n"; }

//return EGraphSearchResult::Success;