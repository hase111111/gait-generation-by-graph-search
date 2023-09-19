//#pragma once
//
//#include <memory>
//
//#include "map_state.h"
//#include "node.h"
//#include "abstract_pass_finder.h"
//
//
////! @class PassFinderNone
////! @brief グラフ探索を行わないクラス．Graphicクラスのデバッグを行う際に用いる．
//
//class PassFinderNone final :public AbstractPassFinder
//{
//public:
//	PassFinderNone() = default;
//	~PassFinderNone() = default;
//
//	EGraphSearchResult getNextNodebyGraphSearch(const SNode& current_node, const MapState_Old* const p_map, const STarget& target, SNode& output_node) override;
//
//private:
//	std::unique_ptr<IGraphTreeCreator> createGraphTreeCreator(const MapState_Old* const map, const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr_) override;
//
//	std::unique_ptr<AbstractGraphSearcher> createGraphSearcher(const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr_) override;
//};
//
//
////! @file pass_finder_none.h 
////! @brief グラフ探索を行わないクラス
