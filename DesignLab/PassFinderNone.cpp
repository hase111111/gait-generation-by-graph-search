#include "PassFinderNone.h"

EGraphSearchResult PassFinderNone::getNextNodebyGraphSearch(const SNode& _current_node, const MapState* const _p_map, const STarget& _target, SNode& _output_node)
{
	_output_node = _current_node;
	return EGraphSearchResult::Success;
}
