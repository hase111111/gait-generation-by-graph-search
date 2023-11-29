#include "gait_pattern_generator_basic.h"

#include "cassert_define.h"
#include "cmdio_util.h"
#include "graph_search_const.h"
#include "map_state.h"


namespace dlio = designlab::cmdio;


GaitPatternGeneratorBasic::GaitPatternGeneratorBasic(
	std::unique_ptr<GraphTreeCreator>&& graph_tree_creator,
	std::unique_ptr<IGraphSearcher>&& graph_searcher
) :
	graph_tree_creator_ptr_(std::move(graph_tree_creator)),
	graph_searcher_ptr_(std::move(graph_searcher)),
	graph_tree_{ GraphSearchConst::kMaxNodeNum }
{
}

GraphSearchResult GaitPatternGeneratorBasic::GetNextNodebyGraphSearch(
	const RobotStateNode& current_node, 
	const MapState& map_state, 
	const TargetRobotState& target, 
	RobotStateNode* output_node
)
{
	assert(current_node.IsLootNode());
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

	graph_tree_.Reset();
	graph_tree_.AddNode(current_node);

	const GraphSearchResult create_result = graph_tree_creator_ptr_->CreateGraphTree(
		0, 
		GraphSearchConst::kMaxDepth,
		&graph_tree_
	);

	if (create_result != GraphSearchResult::kSuccess)
	{
		dlio::Output("グラフ木の作成に失敗しました．", OutputDetail::kDebug);
		return create_result;
	}

	dlio::Output("グラフ木の作成が終了しました．", OutputDetail::kDebug);
	dlio::Output("グラフのサイズ" + std::to_string(graph_tree_.GetGraphSize()), OutputDetail::kDebug);


	// グラフ探索を行う
	dlio::Output("グラフ木を評価します．", OutputDetail::kDebug);

	const auto [search_result, next_node, high_rated_node_index] = graph_searcher_ptr_->SearchGraphTree(graph_tree_, target);

	if (search_result != GraphSearchResult::kSuccess)
	{
		dlio::Output("グラフ木の評価に失敗しました．", OutputDetail::kDebug);
		return search_result;
	}

	*output_node = next_node;
	dlio::Output("グラフ木の評価が終了しました．グラフ探索に成功しました．", OutputDetail::kDebug);

	return GraphSearchResult::kSuccess;
}