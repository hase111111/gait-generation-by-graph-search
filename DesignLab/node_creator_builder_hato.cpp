#include "node_creator_builder_hato.h"

#include "cassert_define.h"
#include "node_creator_com_move.h"
#include "node_creator_com_up_down.h"
#include "node_creator_leg_hierarchy.h"
#include "node_creator_leg_up_down.h"


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
	std::map<HexapodMove, std::unique_ptr<INodeCreator> >* node_creator) const
{
	assert(node_creator != nullptr);	// node_creatorがnullptrでない．
	assert(node_creator->size() == 0);	// node_creatorは空でなければならない．


	// 追加したい場合，以下のように記述する．
	// (*node_creator)[HexapodMove::???] = std::make_unique<クラス名>(クラスのコンストラクタの引数);
	// この場合，HexapodMove::???のノードを作成するクラスは，↑ である．

	const auto hierarchy_list = std::vector<DiscreteLegPos>{
		//DiscreteLegPos::kBack,
		DiscreteLegPos::kCenter,
		DiscreteLegPos::kFront,
		//DiscreteLegPos::kLowerBack,
		DiscreteLegPos::kLowerFront,
		//DiscreteLegPos::kUpperBack,
		DiscreteLegPos::kUpperFront
	};

	(*node_creator)[HexapodMove::kLegHierarchyChange] = std::make_unique<NodeCreatorLegHierarchy>(
		HexapodMove::kLegUpDown,
		hierarchy_list
	);

	(*node_creator)[HexapodMove::kLegUpDown] = std::make_unique<NodeCreatorLegUpDown>(
		map,
		converter_ptr_,
		presenter_ptr_,
		checker_ptr_,
		HexapodMove::kComUpDown
	);

	(*node_creator)[HexapodMove::kComUpDown] = std::make_unique<NodeCreatorComUpDown>(
		map,
		converter_ptr_,
		presenter_ptr_,
		checker_ptr_,
		HexapodMove::kComMove
	);

	(*node_creator)[HexapodMove::kComMove] = std::make_unique<NodeCreatorComMove>(
		map,
		converter_ptr_,
		presenter_ptr_,
		checker_ptr_,
		HexapodMove::kLegHierarchyChange
	);
}