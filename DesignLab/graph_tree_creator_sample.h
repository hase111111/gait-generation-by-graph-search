#pragma once
#include "interface_graph_tree_creator.h"


class GraphTreeCreatorSample final : public IGraphTreeCreator
{
public:
	GraphTreeCreatorSample(std::map<EHexapodMove, std::unique_ptr<INodeCreator>>& map) : IGraphTreeCreator(map) {};
	~GraphTreeCreatorSample() = default;

	EGraphSearchResult CreateGraphTree(const SNode& current_node, const DevideMapState& map_ref, std::vector<SNode>* output_graph) override;

};


//! @file graph_tree_creator_sample.h
//! @brief グラフ木を作成するクラスのサンプル
//! @author 長谷川

//! @class GraphTreeCreatorSample
//! @brief グラフを作成するクラスのサンプルです．テキトーにグラフを作成します．
//! @details IGraphTreeCreator を継承したい場合．以下のように<br> <br>
//!	class 好きな名前 final : public IGraphTreeCreator <br>
//!	{ <br>
//!	} <br> <br>
//! と宣言する．<br>finalはもうこれ以上継承しないよという意味．public IGraphTreeCreator はこのクラスを継承したよという意味．<br>
//! IGraphTreeCreatorを継承したクラスに課せられる制約はただ一つ，IGraphTreeCreator の純粋仮想関数，createGraphTreeを実装すること．<br>
//! 以下のようにcreateGraphTreeをpublicなところに宣言して，後ろにoverrideとつけるように．<br>
