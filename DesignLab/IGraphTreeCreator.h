#pragma once
#include "MapState.h"
#include "listFunc.h"
#include <vector>

//グラフ木を作成するクラスのインターフェース．実体は作成できないのでこれを継承してたクラスを使ってください．
class IGraphTreeCreator
{
public:
	IGraphTreeCreator() = default;
	virtual ~IGraphTreeCreator() = default;		//継承をするクラスのデストラクタはvirtualにしておく．参考 https://www.yunabe.jp/docs/cpp_virtual_destructor.html

	//グラフ木を作成するクラス．
	virtual bool createGraphTree(const SNode& _current_node, const MapState* const _p_map, std::vector<SNode>& _output_graph) = 0;

private:

};
