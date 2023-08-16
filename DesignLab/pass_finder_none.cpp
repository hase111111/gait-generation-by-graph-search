#include "pass_finder_none.h"


EGraphSearchResult PassFinderNone::getNextNodebyGraphSearch(const SNode& current_node, const MapState* const p_map, const STarget& target, SNode& output_node)
{
	output_node = current_node;
	return EGraphSearchResult::Success;
}
