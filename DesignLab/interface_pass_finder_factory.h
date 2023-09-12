#pragma once

#include <memory>

#include "interface_graph_tree_creator.h"
#include "abstract_hexapod_state_calculator.h"
#include "abstract_graph_searcher.h"
#include "map_state.h"


//! @class IPassFinderFactory
//! @date 2023/08/21
//! @author 長谷川
//! @brief パス探索に必要なクラスを生成するファクトリークラス．
class IPassFinderFactory
{
public:
	IPassFinderFactory() = default;
	virtual ~IPassFinderFactory() = default;

	//! @brief グラフ木の生成に必要なクラスを生成する．
	//! @param [in] map マップ情報．
	//! @param [out] tree 歩容パターン生成の探索に必要な木構造を生成するクラス．
	virtual void createGraphTreeCreator(const MapState* const map, std::shared_ptr<AbstractHexapodStateCalculator> calc, std::unique_ptr<IGraphTreeCreator>& tree) = 0;

	//! @brief グラフ探索を行うクラスを生成する．
	//! @param [out] searcher 歩容パターン生成の探索を行うクラス．
	virtual void createGraphSearcher(std::unique_ptr<AbstractGraphSearcher>& searcher, std::shared_ptr<AbstractHexapodStateCalculator> calc) = 0;
};


//! @file interface_pass_finder_factory.h
//! @date 2023/08/21
//! @author 長谷川
//! @brief パス探索に必要なクラスを生成するファクトリークラス．
//! @n 行数 : @lineinfo
