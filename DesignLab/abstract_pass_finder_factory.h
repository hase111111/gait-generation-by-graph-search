#pragma once

#include <memory>

#include "interface_graph_tree_creator.h"
#include "abstract_hexapod_state_calculator.h"
#include "InterfaceGraphSearcher.h"
#include "map_state.h"


//! @class AbstractPassFinderFactory
//! @date 2023/08/21
//! @author 長谷川
//! @brief パス探索に必要なクラスを生成するアブストラクトファクトリー．
class AbstractPassFinderFactory
{
public:
	AbstractPassFinderFactory() = default;
	virtual ~AbstractPassFinderFactory() = default;

	//! @brief グラフ木の生成に必要なクラスを生成する．
	//! @param [in] map マップ情報．
	//! @param [out] tree 歩容パターン生成の探索に必要な木構造を生成するクラス．
	virtual void createGraphTreeCreator(const MapState* const map, std::shared_ptr<AbstractHexapodStateCalculator> calc, std::unique_ptr<IGraphTreeCreator>& tree) = 0;

	//! @brief グラフ探索を行うクラスを生成する．
	//! @param [out] searcher 歩容パターン生成の探索を行うクラス．
	virtual void createGraphSearcher(std::unique_ptr<IGraphSearcher>& searcher) = 0;
};


//! @file abstract_pass_finder_factory.h
//! @date 2023/08/21
//! @author 長谷川
//! @brief パス探索に必要なクラスを生成するアブストラクトファクトリー．
//! @n 行数 : @lineinfo
