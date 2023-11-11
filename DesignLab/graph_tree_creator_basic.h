//! @file graph_tree_creator_basic.h 
//! @brief 歩容パターングラフを作成するクラス

#ifndef DESIGNLAB_GRAPH_TREE_CREATOR_BASIC_H_
#define DESIGNLAB_GRAPH_TREE_CREATOR_BASIC_H_

#include "interface_graph_tree_creator.h"

#include <map>
#include <memory>

#include "interface_node_creator.h"
#include "interface_node_creator_builder.h"


//! @class GraphTreeCreatorBasic
//! @brief 歩容パターングラフを作成するクラス
//! @details 先行研究のプログラムを見ればわかる通り，実際には処理効率をあげるために，
//! @n 複数スレッド同時に処理を行うのだが，このクラスでは単一のスレッドで処理を行う．
class GraphTreeCreatorBasic final : public IGraphTreeCreator
{
public:
	GraphTreeCreatorBasic(
		std::unique_ptr<INodeCreatorBuilder>&& node_creator_builder_ptr,
		const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
		const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
		const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr
	);

	~GraphTreeCreatorBasic() = default;


	void Init(const DevideMapState& map_state);

	GraphSearchResult CreateGraphTree(const RobotStateNode& current_node, int max_depth, std::vector<RobotStateNode>* output_graph) override;

private:

	//! @brief current_nodeの子ノードを生成して，output_graphに代入する．
	//! @param[in] current_node 現在のノード
	//! @param[in] current_num 現在のノードのindex
	//! @param[out] output_graph 生成したノードを代入するベクタ．空にしておくこと.
	void MakeNewNodesByCurrentNode(const RobotStateNode& current_node, const int current_index, std::vector<RobotStateNode>* output_graph) const;


	std::map<HexapodMove, std::unique_ptr<INodeCreator>> node_creator_map_;		//!< ノード生成クラスのマップ．

	const std::unique_ptr<INodeCreatorBuilder> node_creator_builder_ptr_;		//!< ノード生成クラスのビルダー．

	const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
	const std::shared_ptr<const IHexapodStatePresenter> presenter_ptr_;
	const std::shared_ptr<const IHexapodVaildChecker> checker_ptr_;
};


#endif	// DESIGNLAB_GRAPH_TREE_CREATOR_BASIC_H_