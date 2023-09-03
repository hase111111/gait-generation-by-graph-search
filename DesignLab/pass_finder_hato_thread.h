#pragma once

#include <memory>

#include "map_state.h"
#include "node.h"
#include "abstract_pass_finder.h"


//! @class PassFinderHatoThread
//! @date 2023/08/14
//! @author 長谷川
//! @brief パス探索クラス
class PassFinderHatoThread final : public AbstractPassFinder
{
public:
	PassFinderHatoThread() = default;
	~PassFinderHatoThread() = default;

	EGraphSearchResult getNextNodebyGraphSearch(const SNode& current_node, const MapState* const p_map, const STarget& target, SNode& output_node) override;
};


//! @file pass_finder_hato_thread.h
//! @date 2023/08/21
//! @author 長谷川
//! @brief グラフ探索を並列で行うクラス
//! @n 行数 : @lineinfo
