#include "gait_pattern_generator_thread.h"

#include <boost/thread.hpp>

#include "cassert_define.h"
#include "cmdio_util.h"
#include "designlab_array_util.h"
#include "designlab_string_util.h"
#include "graph_search_const.h"
#include "map_state.h"


namespace designlab
{

GaitPatternGeneratorThread::GaitPatternGeneratorThread(
	std::unique_ptr<GraphTreeCreator>&& graph_tree_creator_ptr,
	std::unique_ptr<IGraphSearcher>&& graph_searcher_ptr,
	const int max_depth,
	const int max_node_num
) :
	graph_tree_creator_ptr_(std::move(graph_tree_creator_ptr)),
	graph_searcher_ptr_(std::move(graph_searcher_ptr)),
	graph_tree_{ 1000 },
	graph_tree_array_(MakeArray<GaitPatternGraphTree>(
	GaitPatternGraphTree{ max_node_num / kThreadNum },
	GaitPatternGraphTree{ max_node_num / kThreadNum },
	GaitPatternGraphTree{ max_node_num / kThreadNum },
	GaitPatternGraphTree{ max_node_num / kThreadNum },
	GaitPatternGraphTree{ max_node_num / kThreadNum },
	GaitPatternGraphTree{ max_node_num / kThreadNum }
	)),
	max_depth_(max_depth)
{
}

GraphSearchResult GaitPatternGeneratorThread::GetNextNodebyGraphSearch(
	const RobotStateNode& current_node,
	const MapState& map_state,
	const RobotOperation& operation,
	RobotStateNode* output_node
)
{
	assert(current_node.IsLootNode());
	assert(output_node != nullptr);
	assert(graph_tree_creator_ptr_ != nullptr);
	assert(graph_searcher_ptr_ != nullptr);

	//初期化処理を行う．
	DevideMapState devide_map;
	devide_map.Init(map_state, current_node.global_center_of_mass);

	graph_tree_creator_ptr_->Init(devide_map);

	// グラフ探索をするための，歩容パターングラフを生成する．
	graph_tree_.Reset();
	graph_tree_.AddNode(current_node);

	const GraphSearchResult create_result = graph_tree_creator_ptr_->CreateGraphTree(
		0,
		1,
		&graph_tree_
	);

	if (create_result.result != enums::Result::kSuccess) { return create_result; }

	CmdIOUtil::Output("深さ1までグラフ木の生成が終了しました．", enums::OutputDetail::kDebug);
	CmdIOUtil::Output("グラフ木のノード数は" + std::to_string(graph_tree_.GetGraphSize()) + "です．", enums::OutputDetail::kDebug);

	// 深さ0のノードをarrayにコピーする
	for (int i = 0; i < kThreadNum; i++)
	{
		graph_tree_array_[i].Reset();
		graph_tree_array_[i].AddNode(current_node);
	}

	// 深さ1のノードをarrayにコピーする
	for (int i = 1; i < graph_tree_.GetGraphSize(); i++)
	{
		if (graph_tree_.GetNode(i).depth == 1)
		{
			graph_tree_array_[i % kThreadNum].AddNode(graph_tree_.GetNode(i));
		}
	}

	//threadを分けて，最大深さまで探索する．
	boost::thread_group thread_group;

	for (size_t i = 0; i < kThreadNum; i++)
	{
		if (graph_tree_array_[i].GetGraphSize() > 1)
		{
			CmdIOUtil::Output("スレッド" + std::to_string(i) + "でグラフ木の生成を開始します．", enums::OutputDetail::kDebug);
			CmdIOUtil::Output("スレッド" + std::to_string(i) + "で探索するノード数は" + std::to_string(graph_tree_array_[i].GetGraphSize()) + "です．", enums::OutputDetail::kDebug);

			thread_group.create_thread(
				boost::bind(
				&GraphTreeCreator::CreateGraphTree,
				graph_tree_creator_ptr_.get(),
				1,
				max_depth_,
				&graph_tree_array_[i]
			)
			);
		}
	}

	thread_group.join_all();

	CmdIOUtil::Output("グラフ木の生成が終了しました．\n", enums::OutputDetail::kDebug);

	for (size_t i = 0; i < kThreadNum; i++)
	{
		CmdIOUtil::Output("スレッド" + std::to_string(i) + "で作成したノード数は" + std::to_string(graph_tree_array_[i].GetGraphSize()) + "です．", enums::OutputDetail::kDebug);
	}


	//グラフ探索を行う．
	std::array<std::tuple<GraphSearchResult, int, int>, kThreadNum> search_result_array;

	for (size_t i = 0; i < kThreadNum; i++)
	{
		CmdIOUtil::Output("[" + std::to_string(i) + "]グラフ探索を開始します．", enums::OutputDetail::kDebug);
		search_result_array[i] = graph_searcher_ptr_->SearchGraphTree(
			graph_tree_array_[i],
			operation,
			devide_map,
			max_depth_
		);

		CmdIOUtil::Output("[" + std::to_string(i) + "]グラフ探索が終了しました．", enums::OutputDetail::kDebug);
		CmdIOUtil::Output("[" + std::to_string(i) + "]グラフ探索の結果は" + std::get<0>(search_result_array[i]).ToString() + "です．", enums::OutputDetail::kDebug);
		CmdIOUtil::Output("[" + std::to_string(i) + "]グラフ探索の結果のノードは" + std::to_string(std::get<2>(search_result_array[i])) + "です．", enums::OutputDetail::kDebug);
	}

	//各スレッドごとの探索結果を統合する．
	AppendGraphTree(search_result_array);

	//統合されたグラフを，再び探索する．
	const auto [search_result, next_node_index, _] = graph_searcher_ptr_->SearchGraphTree(graph_tree_, operation, devide_map, max_depth_);

	if (search_result.result != enums::Result::kSuccess)
	{
		CmdIOUtil::Output("グラフ木の評価に失敗しました．", enums::OutputDetail::kDebug);
		return search_result;
	}

	CmdIOUtil::Output("グラフ木の評価が終了しました．グラフ探索に成功しました．", enums::OutputDetail::kDebug);

	(*output_node) = graph_tree_.GetNode(next_node_index);

	return { enums::Result::kSuccess,"" + search_result.message };
}

void GaitPatternGeneratorThread::AppendGraphTree(
	const std::array<std::tuple<GraphSearchResult, int, int>, kThreadNum>& search_result_array
)
{
	const RobotStateNode root_node = graph_tree_.GetRootNode();
	graph_tree_.Reset();
	graph_tree_.AddNode(root_node);

	for (size_t i = 0; i < kThreadNum; i++)
	{
		const auto [search_result, _, next_node_index] = search_result_array[i];

		//条件を満たしていない場合は，次のスレッドの結果を見る．
		if (search_result.result != enums::Result::kSuccess) { continue; }

		if (graph_tree_array_[i].GetNode(next_node_index).depth != max_depth_) { continue; }


		//追加するノードを格納する．
		std::vector<RobotStateNode> add_node;

		add_node.push_back(graph_tree_array_[i].GetNode(next_node_index));

		while (add_node.back().depth != 0)
		{
			add_node.push_back(graph_tree_array_[i].GetNode(add_node.back().parent_index));
		}

		if (add_node.size() != max_depth_ + 1) { continue; }

		//深さ順にソートする．0から最も深いノードまでの順番になる．
		std::sort(add_node.begin(), add_node.end(), [](const RobotStateNode& a, const RobotStateNode& b) { return a.depth < b.depth; });

		//追加するノードをgraph_tree_に格納する．
		for (size_t j = 1; j < max_depth_ + 1; ++j)
		{
			//add_nodeの中から，深さjのノードをgraph_tree_に格納する．
			if (add_node[j].depth == 1)
			{
				add_node[j].parent_index = 0;
			}
			else
			{
				add_node[j].parent_index = graph_tree_.GetGraphSize() - 1;
			}

			graph_tree_.AddNode(add_node[j]);
		}
	}

	CmdIOUtil::Output("グラフ木の統合が終了しました．", enums::OutputDetail::kDebug);
	CmdIOUtil::Output("グラフ木のノード数は" + std::to_string(graph_tree_.GetGraphSize()) + "です．", enums::OutputDetail::kDebug);
}

}  // namespace designlab