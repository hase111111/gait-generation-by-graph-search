#pragma once
#include "interface_pass_finder.h"


//! @class PassFinderHato
//! @date 2023/08/17
//! @author 長谷川
//! @brief 波東先輩の手法で，グラフ探索を行うクラス．
class PassFinderHato final : public IPassFinder
{
public:
	PassFinderHato(std::unique_ptr<AbstractPassFinderFactory>&& factory);
	~PassFinderHato();

	EGraphSearchResult getNextNodebyGraphSearch(const SNode& current_node, const MapState* const p_map, const STarget& target, SNode& output_node) override;

private:

};


//! @file pass_finder_hato.h 
//! @date 2023/08/17
//! @author 長谷川
//! @brief 波東さんの手法でグラフ探索を行うクラス．