//#include "com_move_node_creator.h"
//
//#include <iostream>
//
//#include "com_candidate_polygon_maker.h"
//#include "com_selecter.h"
//#include "graph_search_const.h"
//#include "leg_state.h"
//
//
//// @todo 現状動かないので，動くようにする
//
//ComMoveNodeCreator::ComMoveNodeCreator(const MapState_Old* const p_map, const std::shared_ptr<const AbstractHexapodStateCalculator>& calc, const EHexapodMove next_move)
//	: INodeCreator(p_map, calc, next_move), mp_map(p_map)
//{
//	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "ComMoveNodeCreator : コンストラクタが呼ばれた．\n"; }
//};
//
//
//ComMoveNodeCreator::~ComMoveNodeCreator()
//{
//	if (GraphSearchConst::DO_DEBUG_PRINT) { std::cout << "ComMoveNodeCreator : デストラクタが呼ばれた．\n"; }
//};
//
//
//void ComMoveNodeCreator::create(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph)
//{
//	//std::vector<std::pair<designlab::SPolygon2, EDiscreteComPos>> candidate_polygons;
//
//	////重心移動先の候補地点の範囲を示す多角形を作成する
//	//ComCandidatePolygonMaker polygon_maker;
//	////polygon_maker.makeCandidatePolygon(current_node, candidate_polygons);
//
//	////候補範囲から実際に移動する先の座標を選択する
//	//ComSelecter com_selecter;
//	//com_selecter.setCurrentNode(current_node);
//
//	//for (const auto& i : candidate_polygons)
//	//{
//	//	designlab::Vector3 res_pos;
//
//	//	if (com_selecter.getComFromPolygon(i.first, i.second, res_pos) == true)
//	//	{
//	//		SNode next_node = current_node;
//
//	//		next_node.changeGlobalCenterOfMass(res_pos, false);					//重心位置を変更し，それに伴い接地脚の位置も変更する
//
//	//		dl_leg::changeComPattern(next_node.leg_state, i.second);		//leg_stateのcom_patternを変更する
//
//	//		for (int i = 0; i < HexapodConst::LEG_NUM; ++i) { dl_leg::changeLegStateKeepTopBit(next_node.leg_state, i, 4); }
//
//	//		next_node.changeNextNode(current_num, m_next_move);	//深さや親ノードを変更する
//
//	//		(*output_graph).push_back(next_node);
//	//	}
//	//}
//
//	//if (DO_DEBUG_PRINT)std::cout << "ComMoveNodeCreator::create() : " << (*output_graph).size() << std::endl;
//}