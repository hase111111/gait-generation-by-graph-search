#pragma once
#include "InterfaceNodeCreator.h"

class LegHierarchyNodeCreator final : public INodeCreator
{
public:

	LegHierarchyNodeCreator(const MapState* const _p_map, const EHexapodMove _next_move) : INodeCreator(_p_map, _next_move) {};
	~LegHierarchyNodeCreator() = default;

	void create(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph) override;

private:
	//‘S‚Äã‚ÌŠÖ”‚É‚Ü‚Æ‚ß‚é‚Æ‚²‚¿‚á‚Â‚­‚Ì‚ÅCˆÈ‰º‚ÌŠÖ”‚Éˆ—‚ğ•ª‚¯‚Ä‚¨‚­D

	// 1‹r‚ª—V‹r‚µ‚Ä‚¢‚é‚Æ‚«C‚»‚Ì‹r‚Ìó‘Ô‚ğ•Ê‚Ìó‘Ô‚É•ÏX‚·‚éD
	void create1LegLifted(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph);

	// 2‹r‚ª—V‹r‚µ‚Ä‚¢‚é‚Æ‚«C‚»‚Ì‹r‚Ìó‘Ô‚ğ•Ê‚Ìó‘Ô‚É•ÏX‚·‚éD
	void create2LegLifted(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph);

	// 3‹r‚ª—V‹r‚µ‚Ä‚¢‚é‚Æ‚«C‚»‚Ì‹r‚Ìó‘Ô‚ğ•Ê‚Ìó‘Ô‚É•ÏX‚·‚éD
	void create3LegLifted(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph);
};

//! @file LegHierarchyNodeCreator.h
//! @brief ‹r‚ÌŠK‘w\‘¢‚ğì‚é‚½‚ß‚ÌƒNƒ‰ƒXD
//! @date 2023/7/24
//! @auther ’·’Jì

//! @class LegHierarchyNodeCreator
//! @brief ‹r‚ÌŠK‘w\‘¢‚ğì‚é‚½‚ß‚ÌƒNƒ‰ƒXD
//! @date 2023/7/24
//! @author ’·’Jì