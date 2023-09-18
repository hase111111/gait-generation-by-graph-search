//#include "pass_finder_none.h"
//
//
//EGraphSearchResult PassFinderNone::getNextNodebyGraphSearch(const SNode& current_node, const MapState* const p_map, const STarget& target, SNode& output_node)
//{
//	output_node = current_node;
//
//	return EGraphSearchResult::Success;
//}
//
//std::unique_ptr<IGraphTreeCreator> PassFinderNone::createGraphTreeCreator(const MapState* const map, const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr_)
//{
//	return std::unique_ptr<IGraphTreeCreator>();
//}
//
//std::unique_ptr<AbstractGraphSearcher> PassFinderNone::createGraphSearcher(const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr_)
//{
//	return std::unique_ptr<AbstractGraphSearcher>();
//}
