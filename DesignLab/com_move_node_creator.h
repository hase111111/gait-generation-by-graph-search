//#pragma once
//#include "interface_node_creator.h"
//#include "designlab_polygon.h"
//#include "com_type.h"
//
//
////! @class ComMoveNodeCreator
////! @date 2023/08/12
////! @author 長谷川
////! @brief 重心の平行移動を行うクラス．
//class ComMoveNodeCreator final : public INodeCreator
//{
//public:
//
//	ComMoveNodeCreator(const MapState_Old* const p_map, const std::shared_ptr<const AbstractHexapodStateCalculator>& calc, const EHexapodMove next_move);
//	~ComMoveNodeCreator();
//
//	void create(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph) override;
//
//private:
//
//	const MapState_Old* const mp_map;
//
//	std::vector<designlab::SPolygon2> m_polygons;
//
//	const bool DO_DEBUG_PRINT = false;
//};
//
////! @file com_move_node_creator.h
////! @date 2023/08/12
////!	@author 長谷川
////! @brief 重心の平行移動を行うクラス．
////! @n 行数 : @lineinfo
