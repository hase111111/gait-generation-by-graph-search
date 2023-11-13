#include "pass_finder_basic.h"

#include "cassert_define.h"
#include "cmdio_util.h"
#include "graph_search_const.h"
#include "map_state.h"


namespace dlio = designlab::cmdio;


PassFinderBasic::PassFinderBasic(
	std::unique_ptr<GraphTreeCreator>&& graph_tree_creator, 
	std::unique_ptr<IGraphSearcher>&& graph_searcher
) :
	graph_tree_creator_ptr_(std::move(graph_tree_creator)), 
	graph_searcher_ptr_(std::move(graph_searcher))
{
}

GraphSearchResult PassFinderBasic::GetNextNodebyGraphSearch(const RobotStateNode& current_node, const MapState& map_state, const TargetRobotState& target, RobotStateNode* output_node)
{
	assert(output_node != nullptr);
	assert(graph_tree_creator_ptr_ != nullptr);
	assert(graph_searcher_ptr_ != nullptr);

	//初期化処理を行う．
	dlio::Output("PassFinderBasic::GetNextNodebyGraphSearch．\nまずは初期化する．(マップを分割する)\n", OutputDetail::kDebug);

	DevideMapState devide_map;
	devide_map.Init(map_state, current_node.global_center_of_mass);

	graph_tree_creator_ptr_->Init(devide_map);

	dlio::Output("初期化終了．", OutputDetail::kDebug);


	// グラフ探索をするための，歩容パターングラフを生成する
	dlio::Output("グラフ木を作成する", OutputDetail::kDebug);

	RobotStateNode parent_node = current_node;
	parent_node.ChangeParentNode();

	graph_tree_.resize(GraphSearchConst::kMaxNodeNum);

	graph_tree_[0] = parent_node;

	int graph_tree_size = 1;

	GraphSearchResult result = graph_tree_creator_ptr_->CreateGraphTree(0, GraphSearchConst::kMaxDepth, &graph_tree_, &graph_tree_size);

	if (result != GraphSearchResult::kSuccess)
	{
		dlio::Output("グラフ木の作成に失敗．", OutputDetail::kDebug);
		return result; 
	}

	dlio::Output("グラフ木の作成終了．", OutputDetail::kDebug);
	dlio::Output("グラフのサイズ" + std::to_string(graph_tree_size), OutputDetail::kDebug);


	// グラフ探索を行う
	dlio::Output("グラフ木を評価する", OutputDetail::kDebug);

	result = graph_searcher_ptr_->SearchGraphTree(graph_tree_, graph_tree_size, target, output_node);

	if (result != GraphSearchResult::kSuccess)
	{
		dlio::Output("グラフ木の評価に失敗．", OutputDetail::kDebug);
		return result; 
	}

	dlio::Output("グラフ木の評価終了．グラフ探索に成功した", OutputDetail::kDebug);

	return GraphSearchResult::kSuccess;
}