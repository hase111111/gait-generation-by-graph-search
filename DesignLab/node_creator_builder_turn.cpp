﻿
//! @file node_creator_builder_turn.cpp

// Copyright(c) 2023 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include <vector>

#include "node_creator_builder_turn.h"

#include "node_creator_body_rot.h"
#include "cassert_define.h"
#include "node_creator_com_move.h"
#include "node_creator_com_up_down.h"
#include "node_creator_leg_hierarchy.h"
#include "node_creator_leg_up_down.h"


namespace designlab
{

NodeCreatorBuilderTurn::NodeCreatorBuilderTurn(
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
    const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
    const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr) :
    converter_ptr_(converter_ptr),
    presenter_ptr_(presenter_ptr),
    checker_ptr_(checker_ptr)
{
}

void NodeCreatorBuilderTurn::Build(
    const DividedMapState& map,
    std::map<HexapodMove, std::unique_ptr<INodeCreator>>* node_creator) const
{
    assert(node_creator != nullptr);  // node_creator が nullptr でない．
    assert(node_creator->size() == 0);  // node_creator は空でなければならない．

    const auto hierarchy_list = std::vector<enums::DiscreteLegPos>{
        enums::DiscreteLegPos::kBack,
        enums::DiscreteLegPos::kCenter,
        enums::DiscreteLegPos::kFront,
        enums::DiscreteLegPos::kLowerBack,
        enums::DiscreteLegPos::kLowerFront,
        enums::DiscreteLegPos::kUpperBack,
        enums::DiscreteLegPos::kUpperFront
    };

    (*node_creator)[HexapodMove::kLegHierarchyChange] =
        std::make_unique<NodeCreatorLegHierarchy>(HexapodMove::kLegUpDown,
                                                  hierarchy_list);

    (*node_creator)[HexapodMove::kLegUpDown] = std::make_unique<NodeCreatorLegUpDown>(
      map,
      converter_ptr_,
      presenter_ptr_,
      checker_ptr_,
      HexapodMove::kBodyYawRot);

    (*node_creator)[HexapodMove::kBodyYawRot] = std::make_unique<NodeCreatorBodyRot>(
      map,
      converter_ptr_,
      checker_ptr_,
      Vector3::GetUpVec(),
      HexapodMove::kComUpDown);

    (*node_creator)[HexapodMove::kComUpDown] = std::make_unique<NodeCreatorComUpDown>(
      map,
      converter_ptr_,
      presenter_ptr_,
      checker_ptr_,
      HexapodMove::kComMove);

    (*node_creator)[HexapodMove::kComMove] = std::make_unique<NodeCreatorComMove>(
      map,
      converter_ptr_,
      presenter_ptr_,
      checker_ptr_,
      HexapodMove::kLegHierarchyChange);
}

}  // namespace designlab
