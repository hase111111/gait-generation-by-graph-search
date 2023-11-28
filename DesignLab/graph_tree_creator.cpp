#include "graph_tree_creator.h"

#include <iostream>

#include "cassert_define.h"
#include "graph_search_const.h"


GraphTreeCreator::GraphTreeCreator(std::unique_ptr<INodeCreatorBuilder>&& node_creator_builder_ptr) :
	node_creator_builder_ptr_(std::move(node_creator_builder_ptr))
{
	//引数が全てnullptrでないことを確認する．
	assert(node_creator_builder_ptr_ != nullptr);
}

void GraphTreeCreator::Init(const DevideMapState& map_state)
{
	// 現在持っているノード生成クラスを全て削除する．
	node_creator_map_.clear();

	node_creator_builder_ptr_->Build(map_state, &node_creator_map_);
}

GraphSearchResult GraphTreeCreator::CreateGraphTree(int start_depth, int max_depth, GaitPatternGraphTree* graph) const
{
	assert(0 <= start_depth);			// start_depthは0以上である．
	assert(start_depth < max_depth);	// start_depthはmax_depthより小さい．
	assert(graph != nullptr);			// nullptrでない．
	assert(!graph->Empty());			// 空でない．

	int cnt = 0;	//カウンタを用意

	//カウンタがvectorのサイズを超えるまでループする．
	while (cnt < graph->GetGraphSize())
	{
		//探索深さが足りていないノードにのみ処理をする．
		if (start_depth <= graph->GetNode(cnt).depth && graph->GetNode(cnt).depth < max_depth)
		{
			std::vector<RobotStateNode> res_vec;	// cnt番目のノードの子ノードを入れるベクター

			MakeNewNodesByCurrentNode(graph->GetNode(cnt), cnt, &res_vec);		//子ノードを生成する．

			for (const auto& i : res_vec)
			{
				graph->AddNode(i);	//子ノードを追加する．
			}
		}

		++cnt;	//カウンタを進める．
	}

	return GraphSearchResult::kSuccess;
}


void GraphTreeCreator::MakeNewNodesByCurrentNode(const RobotStateNode& current_node, const int current_num, std::vector<RobotStateNode>* output_graph) const
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

		//assertの下に処理を追加する理由としては，assertが呼ばれない場合(Releaseビルドの際など)にも動作可能にするため． 

		//定義されていないならば，同じノードをそのまま追加する．
		RobotStateNode new_node = current_node;

		new_node.ChangeToNextNode(current_num, current_node.next_move);

		(*output_graph).emplace_back(new_node);
	}
}