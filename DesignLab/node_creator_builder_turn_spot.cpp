#include "node_creator_builder_turn_spot.h"

#include "node_creator_body_rot.h"
#include "cassert_define.h"
#include "node_creator_com_move.h"
#include "node_creator_com_up_down.h"
#include "node_creator_leg_hierarchy.h"
#include "node_creator_leg_up_down.h"


namespace dl = ::designlab;


NodeCreatorBuilderTurnSpot::NodeCreatorBuilderTurnSpot(
	const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
	const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
	const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr) :
	converter_ptr_(converter_ptr),
	presenter_ptr_(presenter_ptr),
	checker_ptr_(checker_ptr)
{
}

void NodeCreatorBuilderTurnSpot::Build(
	const DevideMapState& map,
	std::map<HexapodMove, std::unique_ptr<INodeCreator>>* node_creator) const
{
	assert(node_creator != nullptr);	// node_creatorがnullptrでない．
	assert(node_creator->size() == 0);	// node_creatorは空でなければならない．


	(*node_creator)[HexapodMove::kLegHierarchyChange] = std::make_unique<NodeCreatorLegHierarchy>(HexapodMove::kLegUpDown);

	(*node_creator)[HexapodMove::kLegUpDown] = std::make_unique<NodeCreatorLegUpDown>(
		map,
		converter_ptr_,
		presenter_ptr_,
		checker_ptr_,
		HexapodMove::kBodyYawRot
	);

	(*node_creator)[HexapodMove::kBodyYawRot] = std::make_unique<NodeCreatorBodyRot>(
		map,
		converter_ptr_,
		checker_ptr_,
		dl::Vector3::GetUpVec(),
		HexapodMove::kComUpDown
	);

	(*node_creator)[HexapodMove::kComUpDown] = std::make_unique<NodeCreatorComUpDown>(
		map,
		converter_ptr_,
		presenter_ptr_,
		checker_ptr_,
		HexapodMove::kLegHierarchyChange
	);
}