#pragma once
#include "InterfaceNodeCreator.h"
#include "HexapodStateCalculator.h"

class LegUpNodeCreator final : public INodeCreator
{
public:
	LegUpNodeCreator(const MapState* const p_map, const EHexapodMove next_move) : INodeCreator(p_map, next_move) {};
	~LegUpNodeCreator() = default;

	void create(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph) override;

private:

	HexapodStateCalclator m_calculator;
};
