#pragma once
#include "InterfaceGraphTreeCreator.h"
#include "InterfaceGraphSearcher.h"
#include <memory>
#include "map_state.h"

class AbstractPassFinderFactory
{
public:
	AbstractPassFinderFactory() = default;
	virtual ~AbstractPassFinderFactory() = default;

	//! @brief パス探索に必要なクラスを生成する．
	//! @param [in] map マップ情報．
	//! @param [out] tree 歩容パターン生成の探索に必要な木構造を生成するクラス．
	virtual void createGraphTreeCreator(const MapState* const map, std::unique_ptr<IGraphTreeCreator>& tree) = 0;

	//! @param [out] searcher 歩容パターン生成の探索を行うクラス．
	virtual void createGraphSearcher(std::unique_ptr<IGraphSearcher>& searcher) = 0;
};
