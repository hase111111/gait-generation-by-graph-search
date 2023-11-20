#include "gait_pattern_generator_thread.h"

#include <boost/thread.hpp>

#include "cassert_define.h"
#include "cmdio_util.h"
#include "designlab_string_util.h"
#include "graph_search_const.h"
#include "map_state.h"


namespace dlio = designlab::cmdio;
namespace dlsu = designlab::string_util;


GaitPatternGeneratorThread::GaitPatternGeneratorThread(
    std::unique_ptr<GraphTreeCreator>&& graph_tree_creator_ptr,
    std::unique_ptr<IGraphSearcher>&& graph_searcher_ptr) : 
    graph_tree_creator_ptr_(std::move(graph_tree_creator_ptr)),
    graph_searcher_ptr_(std::move(graph_searcher_ptr))
{
	graph_tree_.resize(1000);

	for (size_t i = 0; i < kThreadNum; i++)
	{
		graph_tree_array_[i].resize(GraphSearchConst::kMaxNodeNum / kThreadNum);
	}
}

GraphSearchResult GaitPatternGeneratorThread::GetNextNodebyGraphSearch(
	const RobotStateNode& current_node,
	const MapState& map_state,
	const TargetRobotState& target,
	RobotStateNode* output_node
)
{
	assert(current_node.IsParentNode());
	assert(output_node != nullptr);
	assert(graph_tree_creator_ptr_ != nullptr);
	assert(graph_searcher_ptr_ != nullptr);

	//初期化処理を行う．
	DevideMapState devide_map;
	devide_map.Init(map_state, current_node.global_center_of_mass);

	graph_tree_creator_ptr_->Init(devide_map);

	// グラフ探索をするための，歩容パターングラフを生成する
	graph_tree_[0] = current_node;

	graph_tree_size_ = 1;

	const GraphSearchResult create_result = graph_tree_creator_ptr_->CreateGraphTree(
		0,
		1,
		&graph_tree_,
		&graph_tree_size_
	);

	if (create_result != GraphSearchResult::kSuccess) { return create_result; }

	dlio::Output("深さ1までグラフ木の生成が終了しました．", OutputDetail::kDebug);
	dlio::Output("グラフ木のノード数は" + std::to_string(graph_tree_size_) + "です．", OutputDetail::kDebug);

	// 深さ1のノードをarrayにコピーする
	for (int i = 0; i < kThreadNum; i++)
	{
		graph_tree_array_[i][0] = graph_tree_[0];

		graph_tree_size_array_[i] = 1;
	}

	for (int i = 1; i < graph_tree_size_; i++)
	{
		if (graph_tree_[i].depth == 1) 
		{
			graph_tree_array_[i % kThreadNum][graph_tree_size_array_[i % kThreadNum]] = graph_tree_[i];

			++graph_tree_size_array_[i % kThreadNum];
		}
	}

	//threadを分けて，最大深さまで探索する．
	boost::thread_group thread_group;

	for (size_t i = 0; i < kThreadNum; i++)
	{
		if (graph_tree_size_array_[i] > 1) 
		{
			dlio::Output("スレッド" + std::to_string(i) + "でグラフ木の生成を開始します．", OutputDetail::kDebug);
			dlio::Output("スレッド" + std::to_string(i) + "で探索するノード数は" + std::to_string(graph_tree_size_array_[i]) + "です．", OutputDetail::kDebug);

			thread_group.create_thread(
				boost::bind(
					&GraphTreeCreator::CreateGraphTree,
					graph_tree_creator_ptr_.get(),
					1,
					GraphSearchConst::kMaxDepth,
					&graph_tree_array_[i],
					&graph_tree_size_array_[i]
				)
			);
		}
	}

	thread_group.join_all();

	dlio::Output("グラフ木の生成が終了しました．\n", OutputDetail::kDebug);

	for (size_t i = 0; i < kThreadNum; i++)
	{
		dlio::Output("スレッド" + std::to_string(i) + "で作成したノード数は" + std::to_string(graph_tree_size_array_[i]) + "です．", OutputDetail::kDebug);
	}


	//グラフ探索を行う．
	std::array<std::tuple<GraphSearchResult, RobotStateNode, int>, kThreadNum> search_result_array;

	for (size_t i = 0; i < kThreadNum; i++)
	{
		dlio::Output("[" + std::to_string(i) + "]グラフ探索を開始します．", OutputDetail::kDebug);
		search_result_array[i] = graph_searcher_ptr_->SearchGraphTree(
			graph_tree_array_[i],
			graph_tree_size_array_[i],
			target
		);

		dlio::Output("[" + std::to_string(i) + "]グラフ探索が終了しました．", OutputDetail::kDebug);
		dlio::Output("[" + std::to_string(i) + "]グラフ探索の結果は" + dlsu::MyEnumToString(std::get<0>(search_result_array[i])) + "です．", OutputDetail::kDebug);
		dlio::Output("[" + std::to_string(i) + "]グラフ探索の結果のノードは" + std::to_string(std::get<2>(search_result_array[i])) + "です．", OutputDetail::kDebug);
	}

	//各スレッドごとの探索結果を統合する．
	AppendGraphTree(search_result_array);

	//統合されたグラフを，再び探索する．
	const auto [search_result, next_node, _ ] = graph_searcher_ptr_->SearchGraphTree(
		graph_tree_,
		graph_tree_size_,
		target
	);

	if (search_result != GraphSearchResult::kSuccess)
	{
		dlio::Output("グラフ木の評価に失敗しました．", OutputDetail::kDebug);
		return search_result;
	}

	dlio::Output("グラフ木の評価が終了しました．グラフ探索に成功しました．", OutputDetail::kDebug);

	*output_node = next_node;

	return GraphSearchResult::kSuccess;
}

void GaitPatternGeneratorThread::AppendGraphTree(
	const std::array<std::tuple<GraphSearchResult, RobotStateNode, int>, kThreadNum>& search_result_array
)
{
	graph_tree_size_ = 1;

	for (size_t i = 0; i < kThreadNum; i++)
	{
		const auto [search_result, next_node, next_node_index] = search_result_array[i];

		//条件を満たしていない場合は，次のスレッドの結果を見る．
		if (search_result != GraphSearchResult::kSuccess) { continue; }

		if (graph_tree_array_[i][next_node_index].depth != GraphSearchConst::kMaxDepth) { continue; }


		//追加するノードを格納する．
		std::vector<RobotStateNode> add_node;

		add_node.push_back(graph_tree_array_[i][next_node_index]);

		while (add_node.back().depth != 0)
		{
			add_node.push_back(graph_tree_array_[i][add_node.back().parent_num]);
		}

		if (add_node.size() != GraphSearchConst::kMaxDepth + 1) { continue; }

		//深さ順にソートする．0から最も深いノードまでの順番になる．
		std::sort(add_node.begin(), add_node.end(), [](const RobotStateNode& a, const RobotStateNode& b) { return a.depth < b.depth; });

		//追加するノードをgraph_tree_に格納する．
		for (size_t j = 1; j < GraphSearchConst::kMaxDepth + 1; ++j)
		{
			//add_nodeの中から，深さjのノードをgraph_tree_に格納する．
			graph_tree_[graph_tree_size_] = add_node[j];

			if (graph_tree_[graph_tree_size_].depth == 1)
			{
				graph_tree_[graph_tree_size_].parent_num = 0;
			}
			else
			{
				graph_tree_[graph_tree_size_].parent_num = graph_tree_size_ - 1;
			}

			++graph_tree_size_;
		}
	}

	dlio::Output("グラフ木の統合が終了しました．", OutputDetail::kDebug);
	dlio::Output("グラフ木のノード数は" + std::to_string(graph_tree_size_) + "です．", OutputDetail::kDebug);
}