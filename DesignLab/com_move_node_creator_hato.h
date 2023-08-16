#pragma once

#include "interface_node_creator.h"
#include "my_polygon.h"
#include "com_type.h"
#include "com_candidate_polygon_maker.h"
#include "com_selecter_hato.h"
#include "hexapod_state_calculator.h"


//! @class ComMoveNodeCreatorHato
//! @date 2023/08/12
//! @author ���J��
//! @brief �d�S�̕��s�ړ����s���N���X�D�g������̎�@�D
class ComMoveNodeCreatorHato final : public INodeCreator
{
public:

	ComMoveNodeCreatorHato(const MapState* const p_map, const EHexapodMove next_move);
	~ComMoveNodeCreatorHato();

	void create(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph) override;


private:

	bool isStable(const SNode& node) const;

	bool isIntersectGround(const SNode& node) const;


	static constexpr bool DO_DEBUG_PRINT = false;

	static constexpr float STABLE_MARGIN = 10.0f;	//!< �ÓI���S�]�T 15mm���x���Ó��炵��(�g������̃v���O�������CMAX��40mm���x)


	const MapState* const mp_map;

	std::vector<my_vec::SPolygon2> m_polygons;

	const HexapodStateCalclator m_calclator;

	const ComCandidatePolygonMaker m_maker;

	ComSelecterHato m_selecter;

};

//! @file com_move_node_creator_hato.h
//! @date 2023/08/12
//! @author ���J��
//! @brief �d�S�̕��s�ړ����s���N���X�D�g������̎�@�D
//! @n �s�� : @lineinfo