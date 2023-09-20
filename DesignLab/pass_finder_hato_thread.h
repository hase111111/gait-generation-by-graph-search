//! @file pass_finder_hato_thread.h
//! @brief 並列で処理を行うクラス

#pragma once

#include <memory>

#include "abstract_pass_finder.h"
#include "map_state.h"
#include "node.h"


//! @class PassFinderHatoThread
//! @brief パス探索クラス

class PassFinderHatoThread final : public AbstractPassFinder
{
public:
	PassFinderHatoThread(const std::shared_ptr<const AbstractHexapodStateCalculator> calculator_ptr);
	~PassFinderHatoThread() = default;

	EGraphSearchResult getNextNodebyGraphSearch(const SNode& current_node, const MapState& map_ref, const STarget& target, SNode& output_node) override;

private:

	std::unique_ptr<IGraphTreeCreator> createGraphTreeCreator(const DevideMapState& map, const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr_) override;

	std::unique_ptr<IGraphSearcher> createGraphSearcher(const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr_) override;


	const std::shared_ptr<const AbstractHexapodStateCalculator> calculator_ptr_;

	DevideMapState devide_map_;
};