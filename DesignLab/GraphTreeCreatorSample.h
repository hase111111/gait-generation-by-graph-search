#pragma once
#include "IGraphTreeCreator.h"

// IGraphTreeCreator を継承したい場合．以下のように
// 
//	class 好きな名前 final : public IGraphTreeCreator
//	{
//	}
// 
// と宣言します．finalはもうこれ以上継承しないよという意味．public IGraphTreeCreator はこのクラスを継承したよという意味です．
// IGraphTreeCreatorを継承したクラスに課せられる制約はただ一つ，IGraphTreeCreator の純粋仮想関数，createGraphTreeを実装することです．
// 以下のようにcreateGraphTreeをpublicなところに宣言して，後ろにoverrideとつけてください．


//グラフを作成するクラスのサンプルです．テキトーにグラフを作成します．
class GraphTreeCreatorSample final : public IGraphTreeCreator
{
public:
	GraphTreeCreatorSample() = default;
	~GraphTreeCreatorSample() = default;

	bool createGraphTree(const SNode& _current_node, const MapState* const _p_map, std::vector<SNode>& _output_graph) override;

private:

};
