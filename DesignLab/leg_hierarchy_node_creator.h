#pragma once

#include <vector>

#include "discrete_leg_pos.h"
#include "interface_node_creator.h"



//! @class LegHierarchyNodeCreator
//! @date 2023/08/12
//! @author ’·’Jì
//! @brief ‹r‚ÌŠK‘w\‘¢‚ğì‚é‚½‚ß‚ÌƒNƒ‰ƒXD
class LegHierarchyNodeCreator final : public INodeCreator
{
public:

	LegHierarchyNodeCreator(const MapState* const p_map, const std::shared_ptr<const AbstractHexapodStateCalculator>& calc, const EHexapodMove next_move);
	~LegHierarchyNodeCreator() = default;

	void create(const SNode& current_node, const int current_node_index, std::vector<SNode>* output_graph) override;

private:


	// 1‹r‚ª—V‹r‚µ‚Ä‚¢‚é‚Æ‚«C‚»‚Ì‹r‚Ìó‘Ô‚ğ•Ê‚Ìó‘Ô‚É•ÏX‚·‚éD
	void create1LegLifted(const SNode& current_node, const int current_node_index, std::vector<SNode>* output_graph);

	// 2‹r‚ª—V‹r‚µ‚Ä‚¢‚é‚Æ‚«C‚»‚Ì‹r‚Ìó‘Ô‚ğ•Ê‚Ìó‘Ô‚É•ÏX‚·‚éD
	void create2LegLifted(const SNode& current_node, const int current_node_index, std::vector<SNode>* output_graph);

	// 3‹r‚ª—V‹r‚µ‚Ä‚¢‚é‚Æ‚«C‚»‚Ì‹r‚Ìó‘Ô‚ğ•Ê‚Ìó‘Ô‚É•ÏX‚·‚éD
	void create3LegLifted(const SNode& current_node, const int current_node_index, std::vector<SNode>* output_graph);


	const std::vector<EDiscreteLegPos> discrete_leg_pos_list_;		//!< —£U‰»‚³‚ê‚½‹rˆÊ’u‚ÌƒŠƒXƒgC‚±‚ÌƒŠƒXƒg‚Ì’†‚Ì’l‚©‚ç‹r‚Ìó‘Ô‚ğ•ÏX‚·‚éD
};


//! @file leg_hierarchy_node_creator.h
//! @date 2023/08/12
//! @author ’·’Jì
//! @brief ‹r‚ÌŠK‘w\‘¢‚ğì‚é‚½‚ß‚ÌƒNƒ‰ƒXD
//! @n s” : @lineinfo
