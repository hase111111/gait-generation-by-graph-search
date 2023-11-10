//! @file graph_tree_creator_hato.h 
//! @brief 波東さんのグラフを作成するプログラムを移植したクラスの実装

#ifndef DESIGNLAB_GRAPH_TREE_CREATOR_HATO_H_
#define DESIGNLAB_GRAPH_TREE_CREATOR_HATO_H_

#include "interface_graph_tree_creator.h"

#include <map>
#include <memory>

#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_state_presenter.h"
#include "interface_hexapod_vaild_checker.h"
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
		const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
		const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
		const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr
	);

	~GraphTreeCreatorHato() = default;


	void Init(const DevideMapState& map_state);

	GraphSearchResult CreateGraphTree(const RobotStateNode& current_node, int max_depth, std::vector<RobotStateNode>* output_graph) override;

private:

	//! @brief 現在のグラフから，最大深さまでノードを生成する．
	//! @param[in] max_depth 最大深さ
	//! @param[out] output_graph 生成したノードを追加するベクタ．
	void MakeGraphTreeToMaxDepth(int max_depth, std::vector<RobotStateNode>* output_graph) const;

	//! @brief current_nodeの子ノードを生成して，output_graphに代入する．
	//! @param[in] current_node 現在のノード
	//! @param[in] current_num 現在のノードのindex
	//! @param[out] output_graph 生成したノードを代入するベクタ．空にしておくこと.
	void MakeNewNodesByCurrentNode(const RobotStateNode& current_node, int current_index, std::vector<RobotStateNode>* output_graph) const;


	constexpr static int kMultiThreadNum = 6;		//!< マルチスレッドの数．

	std::map<HexapodMove, std::unique_ptr<INodeCreator>> node_creator_map_;		//!< ノード生成クラスのマップ．

	const std::unique_ptr<INodeCreatorBuilder> node_creator_builder_ptr_;		//!< ノード生成クラスのビルダー．

	const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
	const std::shared_ptr<const IHexapodStatePresenter> presenter_ptr_;
	const std::shared_ptr<const IHexapodVaildChecker> checker_ptr_;
};


#endif	//DESIGNLAB_GRAPH_TREE_CREATOR_HATO_H_