﻿
//! @file node_creator_builder_body_rot.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "node_creator_builder_body_rot.h"

#include <vector>

#include "cassert_define.h"
#include "node_creator_body_rot.h"
#include "node_creator_com_move.h"
#include "node_creator_com_move_straight.h"
#include "node_creator_com_up_down.h"
#include "node_creator_leg_hierarchy.h"
#include "node_creator_leg_up_down.h"
#include "node_creator_leg_up_down_radius.h"

namespace designlab {

NodeCreatorBuilderBodyRot::NodeCreatorBuilderBodyRot(
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
    const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
    const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr)
    : converter_ptr_(converter_ptr),
      presenter_ptr_(presenter_ptr),
      checker_ptr_(checker_ptr) {}

void NodeCreatorBuilderBodyRot::Build(
    const DividedMapState& map,
    std::map<HexapodMove, std::unique_ptr<INodeCreator> >* node_creator) const {
  assert(node_creator != nullptr);  // node_creator が nullptr でない.
  assert(node_creator->size() == 0);  // node_creator は空でなければならない.

  // 追加したい場合,以下のように記述する.
  // (*node_creator)[HexapodMove::???] =
  // std::make_unique<クラス名>(クラスのコンストラクタの引数);
  // この場合,HexapodMove::???のノードを作成するクラスは,^~~~~~ である.

  const auto hierarchy_list = std::vector<DiscreteLegPos>{
      DiscreteLegPos::kBack,       DiscreteLegPos::kCenter,
      DiscreteLegPos::kFront,      DiscreteLegPos::kLowerBack,
      DiscreteLegPos::kLowerFront, DiscreteLegPos::kUpperBack,
      DiscreteLegPos::kUpperFront};

  (*node_creator)[HexapodMove::kLegHierarchyChange] =
      std::make_unique<NodeCreatorLegHierarchy>(HexapodMove::kLegUpDown,
                                                hierarchy_list);

  (*node_creator)[HexapodMove::kLegUpDown] =
      std::make_unique<NodeCreatorLegUpDownRadius>(map, converter_ptr_,
                                                   presenter_ptr_, checker_ptr_,
                                                   HexapodMove::kComUpDown);

  (*node_creator)[HexapodMove::kComUpDown] =
      std::make_unique<NodeCreatorComUpDown>(map, converter_ptr_,
                                             presenter_ptr_, checker_ptr_,
                                             HexapodMove::kBodyPitchRot);
  (*node_creator)[HexapodMove::kBodyPitchRot] =
      std::make_unique<NodeCreatorBodyRot>(map, converter_ptr_, checker_ptr_,
                                           Vector3::GetLeftVec(),
                                           HexapodMove::kComMove);

  (*node_creator)[HexapodMove::kComMove] =
      std::make_unique<NodeCreatorComMoveStraight>(
          map, converter_ptr_, presenter_ptr_, checker_ptr_,
          HexapodMove::kLegHierarchyChange);
}

}  // namespace designlab
