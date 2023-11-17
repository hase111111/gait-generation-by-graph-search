#include "gait_pattern_generator_thread.h"

#include <boost/thread.hpp>

#include "cassert_define.h"
#include "cmdio_util.h"
#include "graph_search_const.h"
#include "map_state.h"


namespace dlio = designlab::cmdio;


GaitPatternGeneratorThread::GaitPatternGeneratorThread(
    std::unique_ptr<GraphTreeCreator>&& graph_tree_creator_ptr,
    std::unique_ptr<IGraphSearcher>&& graph_searcher_ptr) : 
    graph_tree_creator_ptr_(std::move(graph_tree_creator_ptr)),
    graph_searcher_ptr_(std::move(graph_searcher_ptr))
{
    graph_tree_.resize(GraphSearchConst::kMaxNodeNum);

	for (size_t i = 0; i < kThreadNum; i++)
	{
		graph_tree_array_[i].resize(GraphSearchConst::kMaxNodeNum);
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
	dlio::Output("グラフ探索中を開始します．まずは初期化します．\n", OutputDetail::kDebug);

	DevideMapState devide_map;
	devide_map.Init(map_state, current_node.global_center_of_mass);

	graph_tree_creator_ptr_->Init(devide_map);

	// グラフ探索をするための，歩容パターングラフを生成する
	dlio::Output("初期化が終了しました．グラフ木を作成します．", OutputDetail::kDebug);

	graph_tree_[0] = current_node;

	int graph_tree_size = 1;

	const GraphSearchResult create_result = graph_tree_creator_ptr_->CreateGraphTree(
		0,
		1,
		&graph_tree_,
		&graph_tree_size
	);

	if (create_result != GraphSearchResult::kSuccess) { return create_result; }

	// 深さ1のノードをarrayにコピーする
	for (size_t i = 0; i < kThreadNum; i++)
	{
		graph_tree_array_[i][0] = graph_tree_[0];

		graph_tree_size_array_[i] = 1;
	}

	for (size_t i = 0; i < graph_tree_size; i++)
	{
		graph_tree_array_[i % kThreadNum][i] = graph_tree_[i];

		++graph_tree_size_array_[i % kThreadNum];
	}

	//threadを分けて，最大深さまで探索する
	boost::thread_group thread_group;

	for (size_t i = 0; i < kThreadNum; i++)
	{
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

	thread_group.join_all();


	// グラフ探索を行う
	const GraphSearchResult search_result = graph_searcher_ptr_->SearchGraphTree(graph_tree_, graph_tree_size, target, output_node);

	if (search_result != GraphSearchResult::kSuccess)
	{
		dlio::Output("グラフ木の評価に失敗しました．", OutputDetail::kDebug);
		return search_result;
	}

	dlio::Output("グラフ木の評価が終了しました．グラフ探索に成功しました．", OutputDetail::kDebug);

	return GraphSearchResult::kSuccess;
}