#include "graph_tree_creator_hato.h"

#include <iostream>

#include <boost/thread.hpp>

#include "cassert_define.h"
#include "graph_search_const.h"



GraphTreeCreatorHato::GraphTreeCreatorHato(
	std::unique_ptr<INodeCreatorBuilder>&& node_creator_builder_ptr,
	const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
	const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
	const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr
	) :
	node_creator_builder_ptr_(std::move(node_creator_builder_ptr)),
	converter_ptr_(converter_ptr),
	presenter_ptr_(presenter_ptr),
	checker_ptr_(checker_ptr)
{
	//引数が全てnullptrでないことを確認する．
	assert(node_creator_builder_ptr_ != nullptr);	
	assert(converter_ptr_ != nullptr);	
	assert(presenter_ptr_ != nullptr);
	assert(checker_ptr_ != nullptr);
}


void GraphTreeCreatorHato::Init(const DevideMapState& map_state)
{
	node_creator_map_.clear();

	node_creator_builder_ptr_->Build(
		map_state, 
		converter_ptr_,
		presenter_ptr_,
		checker_ptr_,
		&node_creator_map_
	);
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