#include "PassFinderFactoryHato.h"
#include "GraphTreeCreatorHato.h"
#include "GraphSearcherHato.h"
#include "GraphSearchHato.h"
#include "LegUpDownNodeCreator.h"
#include "LegHierarchyNodeCreator.h"
#include "ComUpDownNodeCreator.h"
#include "ComMoveNodeCreator.h"

void PassFinderFactoryHato::createPassFinder(std::unique_ptr<IGraphTreeCreator>& _tree, std::unique_ptr<IGraphSearcher>& _searcher, const MapState* const _map)
{
	//木を作成するクラスのマップを作成．
	std::map<EHexapodMove, std::unique_ptr<INodeCreator>> _node_creator_map;
	_node_creator_map.emplace(EHexapodMove::LEG_HIERARCHY_CHANGE, std::make_unique<LegHierarchyNodeCreator>(_map, EHexapodMove::LEG_UP_DOWN_NEXT_COM_UP_DOWN));
	_node_creator_map.emplace(EHexapodMove::LEG_UP_DOWN_NEXT_COM_UP_DOWN, std::make_unique<LegUpDownNodeCreator>(_map, EHexapodMove::COM_UP_DOWN));
	_node_creator_map.emplace(EHexapodMove::COM_UP_DOWN, std::make_unique<ComUpDownNodeCreator>(_map, EHexapodMove::LEG_UP_DOWN_NEXT_COM_MOVE));
	_node_creator_map.emplace(EHexapodMove::LEG_UP_DOWN_NEXT_COM_MOVE, std::make_unique<LegUpDownNodeCreator>(_map, EHexapodMove::COM_MOVE));
	_node_creator_map.emplace(EHexapodMove::COM_MOVE, std::make_unique<ComMoveNodeCreator>(_map, EHexapodMove::LEG_HIERARCHY_CHANGE));

	//木を作成するクラスと，木を探索するクラスを作成．
	std::unique_ptr<IGraphTreeCreator> p_creator = std::make_unique<GraphTreeCreatorHato>(_node_creator_map);
	std::unique_ptr<IGraphSearcher> p_searcher = std::make_unique<GraphSearcherHato>();

	//完成したクラスを引数に代入．
	_tree = std::move(p_creator);
	_searcher = std::move(p_searcher);
}
