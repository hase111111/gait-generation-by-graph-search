#pragma once
#include "MapState.h"
#include "Node.h"

class LegHierarchyNodeCreator final
{
public:

	void create(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph);

private:
	//‘S‚Äã‚ÌŠÖ”‚É‚Ü‚Æ‚ß‚é‚Æ‚²‚¿‚á‚Â‚­‚Ì‚ÅCˆÈ‰º‚ÌŠÖ”‚Éˆ—‚ğ•ª‚¯‚Ä‚¨‚­D

	// 1‹r‚ª—V‹r‚µ‚Ä‚¢‚é‚Æ‚«C‚»‚Ì‹r‚Ìó‘Ô‚ğ•Ê‚Ìó‘Ô‚É•ÏX‚·‚éD
	void create1LegLifted(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph);

	// 2‹r‚ª—V‹r‚µ‚Ä‚¢‚é‚Æ‚«C‚»‚Ì‹r‚Ìó‘Ô‚ğ•Ê‚Ìó‘Ô‚É•ÏX‚·‚éD
	void create2LegLifted(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph);	
	
	// 3‹r‚ª—V‹r‚µ‚Ä‚¢‚é‚Æ‚«C‚»‚Ì‹r‚Ìó‘Ô‚ğ•Ê‚Ìó‘Ô‚É•ÏX‚·‚éD
	void create3LegLifted(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph);

	const EHexapodMove m_next_move = EHexapodMove::LEG_UP_DOWN;
};
