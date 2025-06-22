
//! @file node_creator_sequence_turn_spot.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "node_creator_sequence_turn_spot.h"

#include <vector>

#include "cassert_define.h"
#include "node_creator_body_rot.h"
#include "node_creator_com_move.h"
#include "node_creator_com_up_down.h"
#include "node_creator_leg_hierarchy.h"
#include "node_creator_leg_up_down.h"
#include "node_creator_leg_up_down_2d.h"
#include "node_creator_leg_up_down_radius.h"

namespace designlab {

NodeCreatorSequenceTurnSpot::NodeCreatorSequenceTurnSpot(
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
    const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
    const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr)
    : converter_ptr_(converter_ptr),
      presenter_ptr_(presenter_ptr),
      checker_ptr_(checker_ptr) {}

std::map<HexapodMove, std::unique_ptr<INodeCreator>>
NodeCreatorSequenceTurnSpot::Build(const DividedMapState& map) const {
  using enum DiscreteLegPos;
  using enum HexapodMove;

  std::map<HexapodMove, std::unique_ptr<INodeCreator>> node_creator;

  const auto hierarchy_list = std::vector<DiscreteLegPos>{
      kBack, kCenter, kFront, kLowerBack, kLowerFront, kUpperBack, kUpperFront};

  node_creator[kLegHierarchyChange] =
      std::make_unique<NodeCreatorLegHierarchy>(kLegUpDown, hierarchy_list);

  node_creator[kLegUpDown] = std::make_unique<NodeCreatorLegUpDownRadius>(
      map, converter_ptr_, presenter_ptr_, checker_ptr_, kComUpDown);

  node_creator[kComUpDown] = std::make_unique<NodeCreatorComUpDown>(
      map, converter_ptr_, presenter_ptr_, checker_ptr_, kBodyYawRot);

  node_creator[kBodyYawRot] = std::make_unique<NodeCreatorBodyRot>(
      map, converter_ptr_, checker_ptr_, Vector3::GetUpVec(),
      kLegHierarchyChange);

  return node_creator;
}

}  // namespace designlab
