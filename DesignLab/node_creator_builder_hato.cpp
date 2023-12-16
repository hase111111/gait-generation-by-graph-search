#include "node_creator_builder_hato.h"

#include "cassert_define.h"
#include "node_creator_com_move.h"
#include "node_creator_com_move_straight.h"
#include "node_creator_com_up_down.h"
#include "node_creator_leg_hierarchy.h"
#include "node_creator_leg_up_down.h"
#include "node_creator_body_rot.h"


namespace designlab
{

NodeCreatorBuilderHato::NodeCreatorBuilderHato(
	const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
	const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
	const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr) :
	converter_ptr_(converter_ptr),
	presenter_ptr_(presenter_ptr),
	checker_ptr_(checker_ptr)
{
}

void NodeCreatorBuilderHato::Build(
	const DevideMapState& map,
	std::map<enums::HexapodMove, std::unique_ptr<INodeCreator> >* node_creator) const
{
	assert(node_creator != nullptr);	// node_creatorがnullptrでない．
	assert(node_creator->size() == 0);	// node_creatorは空でなければならない．


	// 追加したい場合，以下のように記述する．
	// (*node_creator)[HexapodMove::???] = std::make_unique<クラス名>(クラスのコンストラクタの引数);
	// この場合，HexapodMove::???のノードを作成するクラスは，↑ である．

	const auto hierarchy_list = std::vector<enums::DiscreteLegPos>{
		enums::DiscreteLegPos::kBack,
		enums::DiscreteLegPos::kCenter,
		enums::DiscreteLegPos::kFront,
		enums::DiscreteLegPos::kLowerBack,
		enums::DiscreteLegPos::kLowerFront,
		enums::DiscreteLegPos::kUpperBack,
		enums::DiscreteLegPos::kUpperFront
	};

	(*node_creator)[enums::HexapodMove::kLegHierarchyChange] = std::make_unique<NodeCreatorLegHierarchy>(
		enums::HexapodMove::kLegUpDown,
		hierarchy_list
	);

	(*node_creator)[enums::HexapodMove::kLegUpDown] = std::make_unique<NodeCreatorLegUpDown>(
		map,
		converter_ptr_,
		presenter_ptr_,
		checker_ptr_,
		enums::HexapodMove::kComUpDown
	);

	(*node_creator)[enums::HexapodMove::kComUpDown] = std::make_unique<NodeCreatorComUpDown>(
		map,
		converter_ptr_,
		presenter_ptr_,
		checker_ptr_,
		enums::HexapodMove::kComMove
	);

	(*node_creator)[enums::HexapodMove::kComMove] = std::make_unique<NodeCreatorComMoveStraight>(
		map,
		converter_ptr_,
		presenter_ptr_,
		checker_ptr_,
		enums::HexapodMove::kLegHierarchyChange
	);
}

} // namespace designlab