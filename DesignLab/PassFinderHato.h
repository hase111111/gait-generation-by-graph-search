#pragma once
#include "InterfacePassFinder.h"


class PassFinderHato final : public IPassFinder
{
public:
	PassFinderHato(std::unique_ptr<AbstractPassFinderFactory>&& factory);
	~PassFinderHato();

	EGraphSearchResult getNextNodebyGraphSearch(const SNode& current_node, const MapState* const p_map, const STarget& target, SNode& output_node) override;

private:

};


//! @file PassFinderHato.h 
//! @brief 波東さんの手法でグラフ探索を行うクラスの実装．
//! @author 長谷川
//! @date 2023/07/24

//! @class PassFinderHato
//! @brief 波東先輩の手法で，グラフ探索を行うクラス．
//! @author 長谷川
//! @date 2023/07/24