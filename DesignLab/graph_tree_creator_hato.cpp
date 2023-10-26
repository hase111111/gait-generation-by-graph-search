#include "graph_tree_creator_hato.h"

#include <iostream>

#include <boost/thread.hpp>

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

GraphSearchResult GraphTreeCreatorHato::CreateGraphTree(const RobotStateNode& current_node, const int max_depth, std::vector<RobotStateNode>* output_graph)
{
	assert(output_graph != nullptr);	//nullptrでない．
	assert(output_graph->empty());		//空である．
	assert(current_node.depth == 0);	//深さが0である．

	// まずは，深さ1までのグラフを作成する．
	std::vector<RobotStateNode> depth1_graph;

	depth1_graph.emplace_back(current_node);	//親を追加する．

	MakeGraphTreeToMaxDepth(1, &depth1_graph);	//深さ1までのグラフを作成する．


	// 次に，深さ1までのグラフを親にして，続きのグラフを作成する．
	// 複数のスレッドで並列に処理する．こうすることで，処理時間を短縮する．
	std::vector<std::vector<RobotStateNode>> threads_result(kMultiThreadNum);	//スレッドごとの結果を格納するベクター
	boost::thread_group tree_creator_threads;	//スレッドを格納するクラス

	int node_cnt = 0;	//ノードのカウンタ

	for (const auto &i : depth1_graph)
	{
		if (i.depth == 1)
		{
			// 深さ1のノードをスレッドごとに分ける．
			threads_result[node_cnt % kMultiThreadNum].emplace_back(i);

			node_cnt++;
		}
	}

	for (int i = 0; i < kMultiThreadNum; ++i)
	{
		//スレッドを作成する．
		tree_creator_threads.create_thread(boost::bind(&GraphTreeCreatorHato::MakeGraphTreeToMaxDepth, this, max_depth, &threads_result[i]));
	}

	tree_creator_threads.join_all();	//各スレッドの終了を待つ．
	
	//スレッドごとの結果を結合する．
	(*output_graph).emplace_back(current_node);	//親を追加する．

	for (int i = 0; i < kMultiThreadNum; ++i)
	{
		const int kParentIndex = static_cast<int>(output_graph->size());	//親のインデックス

		for (auto j : threads_result[i])
		{
			if (j.depth > 1)
			{
				j.parent_num += kParentIndex;
			}

			output_graph->emplace_back(j);
		}
	}

	//ノード数が上限を超えていないか確認する．
	int make_node_num = static_cast<int>(output_graph->size());

	if (GraphSearchConst::kMaxNodeNum < make_node_num)
	{
		return GraphSearchResult::kFailureByNodeLimitExceeded;
	}

	return GraphSearchResult::kSuccess;
}


void GraphTreeCreatorHato::MakeGraphTreeToMaxDepth(int max_depth, std::vector<RobotStateNode>* current_graph) const
{
	assert(current_graph != nullptr);	//nullptrでない．

	if ((*current_graph).empty()) { return; }	//空ならば何もしない．

	int cnt = 0;	//カウンタを用意

	//カウンタがvectorのサイズを超えるまでループする．
	while (cnt < current_graph->size())
	{
		//探索深さが足りていないノードにのみ処理をする．
		if ((*current_graph)[cnt].depth < max_depth)
		{
			std::vector<RobotStateNode> res_vec;	// _cnt番目のノードの子ノードを入れるベクター

			MakeNewNodesByCurrentNode((*current_graph)[cnt], cnt, &res_vec);		//子ノードを生成する．

			for (const auto& i : res_vec)
			{
				current_graph->emplace_back(i);		//子ノードを結果に追加する．
			}
		}

		cnt++;	//カウンタを進める．
	}
}

void GraphTreeCreatorHato::MakeNewNodesByCurrentNode(const RobotStateNode& current_node, const int current_num, std::vector<RobotStateNode>* output_graph) const
{
	assert(output_graph != nullptr);	//nullptrでない．
	assert(output_graph->empty());		//空である．

	if (node_creator_map_.count(current_node.next_move) > 0)
	{
		node_creator_map_.at(current_node.next_move)->Create(current_node, current_num, output_graph);

		return;
	}
	else
	{
		assert(false);	//ノード生成クラスが登録されていない．

		//assertの下に処理を追加する理由としては，assertが呼ばれない場合(Releaseビルドの際など)にも一応動作可能にするため． 

		//定義されていないならば，同じノードをそのまま追加する．
		RobotStateNode new_node = current_node;

		new_node.ChangeToNextNode(current_num, current_node.next_move);

		(*output_graph).emplace_back(new_node);
	}
}


//std::unique_ptr<IGraphTreeCreator> graph_tree_creator = createGraphTreeCreator(devide_map_, calculator_ptr_);	//!< グラフ木の作成クラス
//std::unique_ptr<IGraphSearcher> graph_searcher = createGraphSearcher(calculator_ptr_);			//!< グラフ探索クラス




//if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderBasic : 初期化終了．グラフ探索を開始する．\n"; }
//
//m_graph_tree.clear();
//
//まずはグラフを作成する．_graph_tree 変数に結果を参照渡しされる．
//{
//	//深さ1までのグラフを作成する．
//	RobotStateNode parent_node = current_node;
//	parent_node.ChangeParentNode();
//	m_graph_tree.emplace_back(parent_node);
//
//	std::vector<RobotStateNode> depth1_node;
//
//	GraphSearchResult result = graph_tree_creator->CreateGraphTree(parent_node, 1, &depth1_node);
//
//	if (!graphSeachResultIsSuccessful(result)) { return result; }
//
//	depth1_node.erase(depth1_node.begin());	//深さ0のノードは親ノードと同じなので，削除する．
//
//	if (GraphSearchConst::DO_DEBUG_PRINT)
//	{
//		std::cout << "\n[PassFinder] PassFinderBasic : 深さ1のノードは" << depth1_node.size() << "個\n";
//		std::cout << "[PassFinder] PassFinderBasic : スレッドを" << depth1_node.size() << "個作成します\n\n";
//	}
//
//	//深さ1のノードを親にして，続きのグラフを作成する．
//	const size_t kDepth1NodeNum = depth1_node.size();
//	std::vector<std::unique_ptr<IGraphTreeCreator>> tree_creators(kDepth1NodeNum);
//	std::vector<std::vector<RobotStateNode>> threads_result(kDepth1NodeNum);
//	boost::thread_group tree_creator_threads;
//
//	for (size_t i = 0; i < kDepth1NodeNum; i++)
//	{
//		tree_creators[i] = createGraphTreeCreator(devide_map_, calculator_ptr_);
//
//		if (!tree_creators[i]) { return GraphSearchResult::kFailureByInitializationFailed; }
//
//		tree_creator_threads.create_thread(boost::bind(&IGraphTreeCreator::CreateGraphTree, tree_creators[i].get(), depth1_node[i], GraphSearchConst::kMaxDepth, &threads_result[i]));
//	}
//
//	tree_creator_threads.join_all();
//
//	if (GraphSearchConst::DO_DEBUG_PRINT)
//	{
//		std::cout << "\n[PassFinder] PassFinderBasic : スレッド終了\n";
//
//		for (size_t i = 0; i < kDepth1NodeNum; i++)
//		{
//			std::cout << "[PassFinder] PassFinderBasic : 子ノード数" << threads_result[i].size() << "\n";
//		}
//	}
//
//	//スレッドごとの結果を結合する．
//	for (size_t i = 0; i < kDepth1NodeNum; ++i)
//	{
//		const int kParentIndex = static_cast<int>(m_graph_tree.size());	//親のインデックス
//
//		for (auto j : threads_result[i])
//		{
//			if (j.depth != 1)
//			{
//				j.parent_num += kParentIndex;
//			}
//
//			m_graph_tree.emplace_back(j);
//		}
//	}
//
//	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderBasic : 統合完了．全部で" << m_graph_tree.size() << "個\n"; }
//}
//
//if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHato : グラフ作成終了．グラフを評価する．\n"; }
//
////次にグラフを評価して，次の動作を決定する．put_node 変数に結果を参照渡しされる．
//{
//	GraphSearchResult result = graph_searcher->SearchGraphTree(m_graph_tree, target, &output_node);
//	if (!graphSeachResultIsSuccessful(result)) { return result; }
//}
//
//if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[PassFinder] PassFinderHato : グラフ評価終了．グラフ探索を終了する．\n"; }
//
//return GraphSearchResult::kSuccess;