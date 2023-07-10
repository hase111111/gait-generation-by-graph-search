#pragma once
#include "MapState.h"
#include "Node.h"

class ComMoveNodeCreator final
{
public:

	void create(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph);

private:

	const MapState* mp_Map;
};
