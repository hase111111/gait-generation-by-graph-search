#pragma once
#include "InterfaceGraphTreeCreator.h"
#include "InterfaceGraphSearcher.h"
#include <memory>

//! @brief GraphSearchクラスを初期化するクラス．イニシャライザーとはInitialize(初期化)する人のこと
//! @details このクラスを使うと，グラフ探索をするクラスと，グラフ木を作成するクラスを同時に初期化できる．
//! @note 設計を見直したいので，後で消すかも
class GraphSearchInitializer
{
public:
	GraphSearchInitializer() = default;
	~GraphSearchInitializer() = default;

	//! @brief 探索するグラフ木を作成するクラスと，グラフ探索をするクラスをそれぞれ初期化する．
	//! @param _p_tree_creator グラフ木を作成するクラスのインスタンス．
	//! @param _p_graph_searcher グラフ探索をするクラスのインスタンス．
	//! @return 初期化に成功したらtrue，失敗したらfalse
	bool init(std::unique_ptr<IGraphTreeCreator>& _p_tree_creator, std::unique_ptr<IGraphSearcher>& _p_graph_searcher);

private:

};
