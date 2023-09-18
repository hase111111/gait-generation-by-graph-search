//#include "pass_finder_factory_fixed.h"
//
//#include <iostream>
//
//#include "graph_tree_creator_hato.h"
//#include "graph_searcher_hato.h"
//#include "leg_up_down_node_creator.h"
//#include "leg_hierarchy_node_creator.h"
//#include "com_up_down_node_creator.h"
//#include "com_move_node_creator_hato.h"
//
//
//void PassFinderFactoryFixed::createGraphTreeCreator(const MapState* const p_map, std::shared_ptr<AbstractHexapodStateCalculator> calc, std::unique_ptr<IGraphTreeCreator>& tree)
//{
//	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[Factory] PassFinderFactoryHato: createGraphTreeCreator() グラフ作成クラスの作成開始\n\n"; }
//
//	//木を作成するクラスのマップを作成．
//	std::map<EHexapodMove, std::unique_ptr<INodeCreator>> node_creator_map;
//	node_creator_map.emplace(EHexapodMove::LEG_HIERARCHY_CHANGE, std::make_unique<LegHierarchyNodeCreator>(p_map, calc, EHexapodMove::LEG_UP_DOWN));
//	node_creator_map.emplace(EHexapodMove::LEG_UP_DOWN, std::make_unique<LegUpDownNodeCreator>(p_map, calc, EHexapodMove::COM_UP_DOWN));
//	node_creator_map.emplace(EHexapodMove::COM_UP_DOWN, std::make_unique<ComUpDownNodeCreator>(p_map, calc, EHexapodMove::COM_MOVE));
//	node_creator_map.emplace(EHexapodMove::COM_MOVE, std::make_unique<ComMoveNodeCreatorHato>(p_map, calc, EHexapodMove::LEG_HIERARCHY_CHANGE));
//
//	//木を作成するクラスと，木を探索するクラスを作成．
//	std::unique_ptr<IGraphTreeCreator> p_creator = std::make_unique<GraphTreeCreatorHato>(node_creator_map);
//
//	//完成したクラスを引数に代入．
//	tree = std::move(p_creator);
//
//	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[Factory] PassFinderFactoryHato: グラフ作成クラスの作成終了\n"; }
//}
//
//
//void PassFinderFactoryFixed::createGraphSearcher(std::unique_ptr<AbstractGraphSearcher>& searcher, std::shared_ptr<AbstractHexapodStateCalculator> calc)
//{
//	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[Factory] PassFinderFactoryHato: createGraphSearcher() グラフ探索クラスの作成開始\n\n"; }
//
//	std::unique_ptr<AbstractGraphSearcher> p_searcher = std::make_unique<GraphSearcherHato>(calc);
//	searcher = std::move(p_searcher);
//
//	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "\n[Factory] PassFinderFactoryHato: グラフ探索クラスの作成終了\n"; }
//}
