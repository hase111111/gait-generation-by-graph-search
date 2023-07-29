#pragma once
#include "InterfaceNodeCreator.h"

class LegHierarchyNodeCreator final : public INodeCreator
{
public:

	LegHierarchyNodeCreator(const MapState* const p_map, const EHexapodMove next_move);
	~LegHierarchyNodeCreator();

	void create(const SNode& current_node, const int current_node_index, std::vector<SNode>* output_graph) override;

private:
	//‘S‚Äã‚ÌŠÖ”‚É‚Ü‚Æ‚ß‚é‚Æ‚²‚¿‚á‚Â‚­‚Ì‚ÅCˆÈ‰º‚ÌŠÖ”‚Éˆ—‚ğ•ª‚¯‚Ä‚¨‚­D

	// 1‹r‚ª—V‹r‚µ‚Ä‚¢‚é‚Æ‚«C‚»‚Ì‹r‚Ìó‘Ô‚ğ•Ê‚Ìó‘Ô‚É•ÏX‚·‚éD
	void create1LegLifted(const SNode& current_node, const int current_node_index, std::vector<SNode>* output_graph);

	// 2‹r‚ª—V‹r‚µ‚Ä‚¢‚é‚Æ‚«C‚»‚Ì‹r‚Ìó‘Ô‚ğ•Ê‚Ìó‘Ô‚É•ÏX‚·‚éD
	void create2LegLifted(const SNode& current_node, const int current_node_index, std::vector<SNode>* output_graph);

	// 3‹r‚ª—V‹r‚µ‚Ä‚¢‚é‚Æ‚«C‚»‚Ì‹r‚Ìó‘Ô‚ğ•Ê‚Ìó‘Ô‚É•ÏX‚·‚éD
	void create3LegLifted(const SNode& current_node, const int current_node_index, std::vector<SNode>* output_graph);
};

//! @file LegHierarchyNodeCreator.h
//! @brief ‹r‚ÌŠK‘w\‘¢‚ğì‚é‚½‚ß‚ÌƒNƒ‰ƒXD
//! @date 2023/7/24
//! @auther ’·’Jì

//! @class LegHierarchyNodeCreator
//! @brief ‹r‚ÌŠK‘w\‘¢‚ğì‚é‚½‚ß‚ÌƒNƒ‰ƒXD
//! @date 2023/7/24
//! @author ’·’Jì