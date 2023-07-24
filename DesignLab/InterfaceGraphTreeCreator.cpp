#include "InterfaceGraphTreeCreator.h"

IGraphTreeCreator::IGraphTreeCreator(std::map<EHexapodMove, std::unique_ptr<INodeCreator>>& _map)
{
	for (auto& i : _map)
	{
		m_node_creator_map.insert(std::make_pair(i.first, std::move(i.second)));
	}
}
