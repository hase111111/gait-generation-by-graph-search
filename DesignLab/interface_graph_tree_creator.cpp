#include "interface_graph_tree_creator.h"


IGraphTreeCreator::IGraphTreeCreator(std::map<EHexapodMove, std::unique_ptr<INodeCreator>>& map)
{
	for (auto& i : map)
	{
		m_node_creator_map.insert(std::make_pair(i.first, std::move(i.second)));
	}
}
