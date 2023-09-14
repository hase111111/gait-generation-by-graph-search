#pragma once

#include "interface_node_creator.h"
#include "designlab_polygon.h"
#include "com_type.h"
#include "com_candidate_polygon_maker.h"
#include "com_selecter_hato.h"
#include "hexapod_state_calculator.h"


//! @class ComMoveNodeCreatorHato
//! @date 2023/08/12
//! @author 長谷川
//! @brief 重心の平行移動を行うクラス．波東さんの手法．
class ComMoveNodeCreatorHato final : public INodeCreator
{
public:

	ComMoveNodeCreatorHato(const MapState* const p_map, std::shared_ptr<AbstractHexapodStateCalculator> calc, const EHexapodMove next_move);
	~ComMoveNodeCreatorHato() = default;

	void create(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph) override;


private:

	bool isStable(const SNode& node) const;

	bool isIntersectGround(const SNode& node) const;


	static constexpr bool DO_DEBUG_PRINT = false;

	static constexpr float STABLE_MARGIN = 15.0f;	//!< 静的安全余裕 15mm程度が妥当らしい(波東さんのプログラムより，MAXで40mm程度)


	std::vector<dl_vec::SPolygon2> m_polygons;


	const MapState* const mp_map;

	std::shared_ptr<AbstractHexapodStateCalculator> mp_calculator;	//!< ロボットの状態を計算するクラス

	const ComCandidatePolygonMaker m_maker;

	ComSelecterHato m_selecter;

};

//! @file com_move_node_creator_hato.h
//! @date 2023/08/12
//! @author 長谷川
//! @brief 重心の平行移動を行うクラス．波東さんの手法．
//! @n 行数 : @lineinfo
