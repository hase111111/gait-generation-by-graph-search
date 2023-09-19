#pragma once

#include "interface_node_creator.h"
#include "hexapod_state_calculator.h"


//! @class LegUpNodeCreator
//! @date 2023/08/12
//! @author 長谷川
//! @brief 脚上げノード生成クラス
//! @details 脚先をあげるノードを生成する
//! @n 波東さんのアルゴリズムとは別，自作のもの.
class LegUpNodeCreator final : public INodeCreator
{
public:
	LegUpNodeCreator(const MapState_Old* const p_map, const std::shared_ptr<const AbstractHexapodStateCalculator>& calc, const EHexapodMove next_move) : INodeCreator(p_map, calc, next_move) {};
	~LegUpNodeCreator() = default;

	void create(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph) override;

private:

	HexapodStateCalclator_Old m_calculator;
};


//! @file leg_up_node_creator.h
//! @date 2023/08/12
//! @author 長谷川
//! @brief 脚上げノード生成クラス
//! @n 行数 : @lineinfo
