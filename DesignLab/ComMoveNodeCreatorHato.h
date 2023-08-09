#pragma once
#include "InterfaceNodeCreator.h"
#include "my_polygon.h"
#include "com_type.h"
#include "ComCandidatePolygonMaker.h"
#include "ComSelecterHato.h"
#include "HexapodStateCalculator.h"


class ComMoveNodeCreatorHato final : public INodeCreator
{
public:

	ComMoveNodeCreatorHato(const MapState* const _p_map, const EHexapodMove _next_move);
	~ComMoveNodeCreatorHato();

	void create(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph) override;

private:

	const MapState* const mp_map;

	std::vector<my_vec::SPolygon2> m_Polygons;

	const HexapodStateCalclator m_calclator;

	const ComCandidatePolygonMaker m_maker;

	ComSelecterHato m_selecter;

	static constexpr bool DO_DEBUG_PRINT = false;

	static constexpr float STABLE_MARGIN = 10.0f;	//!< 静的安全余裕 15mm程度が妥当らしい(波東さんのプログラムより，MAXで40mm程度)

	bool isStable(const SNode& _node) const;

	bool isIntersectGround(const SNode& _node) const;
};

//! @file ComMoveNodeCreatorHato.h
//! @brief 重心の平行移動を行うクラス．波東さんの手法．
//! @date 2023/7/25
//!	@auther 長谷川

//! @class ComMoveNodeCreatorHato
//! @brief 重心の平行移動を行うクラス．波東さんの手法．
//! @date 2023/7/25
//! @auther 長谷川