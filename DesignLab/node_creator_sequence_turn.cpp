
//! @file node_creator_sequence_turn.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "node_creator_sequence_turn.h"

#include <vector>

#include "cassert_define.h"
#include "node_creator_body_rot.h"
#include "node_creator_com_move.h"
#include "node_creator_com_up_down.h"
#include "node_creator_leg_hierarchy.h"
#include "node_creator_leg_up_down.h"

namespace gaitgen {

NodeCreatorSequenceTurn::NodeCreatorSequenceTurn(
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
    const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
    const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr)
    : converter_ptr_(converter_ptr),
      presenter_ptr_(presenter_ptr),
      checker_ptr_(checker_ptr) {}

std::map<HexapodMove, std::unique_ptr<INodeCreator>>
NodeCreatorSequenceTurn::Build(const DividedMapState& map) const {
  const auto hierarchy_list = std::vector<DiscreteLegPos>{
      DiscreteLegPos::kBack,       DiscreteLegPos::kCenter,
      DiscreteLegPos::kFront,      DiscreteLegPos::kLowerBack,
      DiscreteLegPos::kLowerFront, DiscreteLegPos::kUpperBack,
      DiscreteLegPos::kUpperFront};

  std::map<HexapodMove, std::unique_ptr<INodeCreator>> node_creator;

  node_creator[HexapodMove::kLegHierarchyChange] =
      std::make_unique<NodeCreatorLegHierarchy>(HexapodMove::kLegUpDown,
                                                hierarchy_list);

  node_creator[HexapodMove::kLegUpDown] =
      std::make_unique<NodeCreatorLegUpDown>(map, converter_ptr_,
                                             presenter_ptr_, checker_ptr_,
                                             HexapodMove::kBodyYawRot);

  node_creator[HexapodMove::kBodyYawRot] = std::make_unique<NodeCreatorBodyRot>(
      map, converter_ptr_, checker_ptr_, Vector3::GetUpVec(),
      HexapodMove::kComUpDown);

  node_creator[HexapodMove::kComUpDown] =
      std::make_unique<NodeCreatorComUpDown>(map, converter_ptr_,
                                             presenter_ptr_, checker_ptr_,
                                             HexapodMove::kComMove);

  node_creator[HexapodMove::kComMove] = std::make_unique<NodeCreatorComMove>(
      map, converter_ptr_, presenter_ptr_, checker_ptr_,
      HexapodMove::kLegHierarchyChange);

  return node_creator;
}

}  // namespace gaitgen
