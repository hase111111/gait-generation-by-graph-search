#pragma once
#include "MapState.h"
#include "Node.h"
#include "InterfacePassFinder.h"
#include <memory>

class PassFinderNone final :public IPassFinder
{
public:
	PassFinderNone(std::unique_ptr<AbstractPassFinderFactory>&& _factory) : IPassFinder(std::move(_factory)) {};
	~PassFinderNone() = default;

	EGraphSearchResult getNextNodebyGraphSearch(const SNode& _current_node, const MapState* const _p_map, const STarget& _target, SNode& _output_node) override;

private:

};


//! @file PassFinderNone.h 
//! @brief グラフ探索を行わないクラスの実装．
//! @author 長谷川
//! @date 2023/07/24

//! @class PassFinderNone
//! @brief グラフ探索を行わないクラス．Graphicクラスのデバッグを行う際に用いる．
//! @author 長谷川
//! @date 2023/07/24
