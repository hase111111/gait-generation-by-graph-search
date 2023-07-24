#pragma once
#include "InterfaceGraphTreeCreator.h"


class GraphTreeCreatorSample final : public IGraphTreeCreator
{
public:
	GraphTreeCreatorSample(std::map<EHexapodMove, std::unique_ptr<INodeCreator>>& _map) : IGraphTreeCreator(_map) {};
	~GraphTreeCreatorSample() = default;

	EGraphSearchResult createGraphTree(const SNode& _current_node, const MapState* const _p_map, std::vector<SNode>& _output_graph, int& _make_node_num) override;

};


//! @file GraphTreeCreatorSample.h
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
