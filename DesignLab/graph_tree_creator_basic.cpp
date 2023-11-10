#include "graph_tree_creator_basic.h"

#include <iostream>

#include "cassert_define.h"
#include "graph_search_const.h"



GraphTreeCreatorBasic::GraphTreeCreatorBasic(
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


void GraphTreeCreatorBasic::Init(const DevideMapState& map_state)
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

GraphSearchResult GraphTreeCreatorBasic::CreateGraphTree(const RobotStateNode& current_node, const int max_depth, std::vector<RobotStateNode>* output_graph)
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
			std::vector<RobotStateNode> res_vec;	// _cnt番目のノードの子ノードを入れるベクター

			MakeNewNodesByCurrentNode((*output_graph)[cnt], cnt, &res_vec);		//子ノードを生成する．

			for (const auto& i : res_vec)
			{
				output_graph->emplace_back(i);		//子ノードを結果に追加する．
			}
		}

		cnt++;	//カウンタを進める．
	}


	//ノード数が上限を超えていないか確認する．
	int make_node_num = static_cast<int>(output_graph->size());

	if (GraphSearchConst::kMaxNodeNum < make_node_num)
	{
		return GraphSearchResult::kFailureByNodeLimitExceeded;
	}

	return GraphSearchResult::kSuccess;
}


void GraphTreeCreatorBasic::MakeNewNodesByCurrentNode(const RobotStateNode& current_node, const int current_num, std::vector<RobotStateNode>* output_graph) const
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