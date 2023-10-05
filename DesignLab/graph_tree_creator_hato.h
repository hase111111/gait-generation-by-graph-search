//! @file graph_tree_creator_hato.h 
//! @brief 波東さんのグラフを作成するプログラムを移植したクラスの実装

#ifndef DESIGNLAB_GRAPH_TREE_CREATOR_HATO_H_
#define DESIGNLAB_GRAPH_TREE_CREATOR_HATO_H_

#include "interface_graph_tree_creator.h"

#include <map>
#include <memory>

#include "abstract_hexapod_state_calculator.h"
#include "interface_node_creator.h"
#include "interface_node_creator_builder.h"


//! @class GraphTreeCreatorHato
//! @brief 波東さんのグラフを作成するプログラムを移植したもの
//! @details もともとのプログラムで行われた処理の中でコメントアウトされていたものはすべて削除したので，
//! @n 知りたければ過去のプログラムを参照すること．
class GraphTreeCreatorHato final : public IGraphTreeCreator
{
public:
	GraphTreeCreatorHato(
		std::unique_ptr<INodeCreatorBuilder>&& node_creator_builder_ptr,
		const std::shared_ptr<const AbstractHexapodStateCalculator> calculator_ptr
	);

	~GraphTreeCreatorHato() = default;


	void Init(const DevideMapState& map_state);

	GraphSearchResult CreateGraphTree(const RobotStateNode& current_node, int max_depth, std::vector<RobotStateNode>* output_graph) override;

private:

	// out_put_graphの値をリセットしてから，_current_nodeの子ノードを生成して，output_graphに代入する．
	void makeNewNodesByCurrentNode(const RobotStateNode& current_node, const int current_num, std::vector<RobotStateNode>* output_graph) const;


	std::map<HexapodMove, std::unique_ptr<INodeCreator>> node_creator_map_;		//!< ノード生成クラスのマップ．

	const std::unique_ptr<INodeCreatorBuilder> node_creator_builder_ptr_;			//!< ノード生成クラスのビルダー．

	const std::shared_ptr<const AbstractHexapodStateCalculator> calculator_ptr_;	//!< ヘキサポッドの状態を計算するクラス．
};


#endif	//DESIGNLAB_GRAPH_TREE_CREATOR_HATO_H_