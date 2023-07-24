#pragma once
#include "InterfaceGraphTreeCreator.h"
#include "InterfaceGraphSearcher.h"
#include <memory>
#include "MapState.h"

class AbstractPassFinderFactory
{
public:
	AbstractPassFinderFactory() = default;
	virtual ~AbstractPassFinderFactory() = default;

	//! @brief パス探索に必要なクラスを生成する．
	//! @param [out] _tree 歩容パターン生成の探索に必要な木構造を生成するクラス．
	//! @param [out] _searcher 歩容パターン生成の探索を行うクラス．
	virtual void createPassFinder(std::unique_ptr<IGraphTreeCreator>& _tree, std::unique_ptr<IGraphSearcher>& _searcher, const MapState* const _map) = 0;

};
