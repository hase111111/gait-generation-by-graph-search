#pragma once

#include "interface_node_creator.h"
#include "hexapod_state_calculator.h"


//! @class LegUpNodeCreator
//! @date 2023/08/12
//! @author ���J��
//! @brief �r�グ�m�[�h�����N���X
//! @details �r���������m�[�h�𐶐�����
//! @n �g������̃A���S���Y���Ƃ͕ʁC����̂���.
class LegUpNodeCreator final : public INodeCreator
{
public:
	LegUpNodeCreator(const MapState* const p_map, const EHexapodMove next_move) : INodeCreator(p_map, next_move) {};
	~LegUpNodeCreator() = default;

	void create(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph) override;

private:

	HexapodStateCalclator m_calculator;
};


//! @file leg_up_node_creator.h
//! @date 2023/08/12
//! @author ���J��
//! @brief �r�グ�m�[�h�����N���X
//! @n �s�� : @lineinfo