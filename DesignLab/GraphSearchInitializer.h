#pragma once
#include "IGraphTreeCreator.h"
#include "IGraphSearcher.h"
#include <memory>

//GraphSearchクラスを初期化するクラス．イニシャライザーとはInitialize(初期化)する人のこと
class GraphSearchInitializer
{
public:
	GraphSearchInitializer() = default;
	~GraphSearchInitializer() = default;

	//探索するグラフ木を作成するクラスと，グラフ探索をするクラスをそれぞれ初期化する．
	bool init(std::unique_ptr<IGraphTreeCreator>& _p_tree_creator, std::unique_ptr<IGraphSearcher>& _p_graph_searcher);

private:

};
