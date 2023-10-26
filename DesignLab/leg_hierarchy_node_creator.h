//! @file leg_hierarchy_node_creator.h
//! @brief ‹r‚ÌŠK‘w\‘¢‚ğì‚é‚½‚ß‚ÌƒNƒ‰ƒXD

#ifndef DESIGNLAB_LEG_HIERARCHY_NODE_CREATOR_H_
#define DESIGNLAB_LEG_HIERARCHY_NODE_CREATOR_H_


#include "interface_node_creator.h"

#include <memory>
#include <vector>

#include "abstract_hexapod_state_calculator.h"
#include "discrete_leg_pos.h"
#include "hexapod_next_move.h"



//! @class LegHierarchyNodeCreator
//! @brief ‹r‚ÌŠK‘w\‘¢‚ğì‚é‚½‚ß‚ÌƒNƒ‰ƒXD
class LegHierarchyNodeCreator final : public INodeCreator
{
public:

	LegHierarchyNodeCreator(HexapodMove next_move);
	~LegHierarchyNodeCreator() = default;

	void Create(const RobotStateNode& current_node, int current_node_index, std::vector<RobotStateNode>* output_graph) const override;

private:


	// 1‹r‚ª—V‹r‚µ‚Ä‚¢‚é‚Æ‚«C‚»‚Ì‹r‚Ìó‘Ô‚ğ•Ê‚Ìó‘Ô‚É•ÏX‚·‚éD
	void create1LegLifted(const RobotStateNode& current_node, int current_node_index, std::vector<RobotStateNode>* output_graph) const;

	// 2‹r‚ª—V‹r‚µ‚Ä‚¢‚é‚Æ‚«C‚»‚Ì‹r‚Ìó‘Ô‚ğ•Ê‚Ìó‘Ô‚É•ÏX‚·‚éD
	void create2LegLifted(const RobotStateNode& current_node, int current_node_index, std::vector<RobotStateNode>* output_graph) const;

	// 3‹r‚ª—V‹r‚µ‚Ä‚¢‚é‚Æ‚«C‚»‚Ì‹r‚Ìó‘Ô‚ğ•Ê‚Ìó‘Ô‚É•ÏX‚·‚éD
	void create3LegLifted(const RobotStateNode& current_node, int current_node_index, std::vector<RobotStateNode>* output_graph) const;


	const HexapodMove next_move_;									

	const std::vector<DiscreteLegPos> discrete_leg_pos_list_;		//!< —£U‰»‚³‚ê‚½‹rˆÊ’u‚ÌƒŠƒXƒgC‚±‚ÌƒŠƒXƒg‚Ì’†‚Ì’l‚©‚ç‹r‚Ìó‘Ô‚ğ•ÏX‚·‚éD
};



#endif // !DESIGNLAB_LEG_HIERARCHY_NODE_CREATOR_H_