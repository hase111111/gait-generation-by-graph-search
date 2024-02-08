
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "node_creator_builder_straight_move.h"

#include <vector>

#include "cassert_define.h"
#include "node_creator_com_move.h"
#include "node_creator_com_move_straight.h"
#include "node_creator_com_up_down.h"
#include "node_creator_leg_hierarchy.h"
#include "node_creator_leg_up_down.h"
#include "node_creator_leg_up_down_radius.h"
#include "node_creator_body_rot.h"


namespace designlab
{

NodeCreatorBuilderStraightMove::NodeCreatorBuilderStraightMove(
  const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
  const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
  const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr) :
    converter_ptr_(converter_ptr),
    presenter_ptr_(presenter_ptr),
    checker_ptr_(checker_ptr)
{
}

void NodeCreatorBuilderStraightMove::Build(
  const DividedMapState& map,
  std::map<enums::HexapodMove, std::unique_ptr<INodeCreator> >* node_creator) const
{
    using enum enums::DiscreteLegPos;
    using enum enums::HexapodMove;

    assert(node_creator != nullptr);  // node_creator が nullptr でない．
    assert(node_creator->size() == 0);  // node_creator は空でなければならない．


    // 追加したい場合，以下のように記述する．
    // (*node_creator)[HexapodMove::???] = std::make_unique<クラス名>(クラスのコンストラクタの引数);
    // この場合，HexapodMove::???のノードを作成するクラスは，^~~~~~ である．

    const auto hierarchy_list = std::vector<enums::DiscreteLegPos>{
      kBack, kCenter, kFront,
      kLowerBack, kLowerFront, kUpperBack, kUpperFront
    };

    (*node_creator)[enums::HexapodMove::kLegHierarchyChange] =
        std::make_unique<NodeCreatorLegHierarchy>(kLegUpDown, hierarchy_list);

    (*node_creator)[enums::HexapodMove::kLegUpDown] =
        std::make_unique<NodeCreatorLegUpDownRadius>(map, converter_ptr_, presenter_ptr_, checker_ptr_, kComUpDown);

    (*node_creator)[enums::HexapodMove::kComUpDown] =
        std::make_unique<NodeCreatorComUpDown>(map, converter_ptr_, presenter_ptr_, checker_ptr_, kComMove);

    (*node_creator)[enums::HexapodMove::kComMove] =
        std::make_unique<NodeCreatorComMove>(map, converter_ptr_, presenter_ptr_, checker_ptr_, kLegHierarchyChange);
}

}  // namespace designlab
