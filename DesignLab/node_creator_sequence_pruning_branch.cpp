
//! @file node_creator_sequence_pruning_branch.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "node_creator_sequence_pruning_branch.h"

#include <vector>

#include "cassert_define.h"
#include "node_creator_com_move_straight.h"
#include "node_creator_com_up_down.h"
#include "node_creator_leg_hierarchy_pruning_branch.h"
#include "node_creator_leg_up_down_pruning_branch.h"

namespace designlab {

NodeCreatorSequencePruningBranch::NodeCreatorSequencePruningBranch(
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
    const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
    const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr)
    : converter_ptr_(converter_ptr),
      presenter_ptr_(presenter_ptr),
      checker_ptr_(checker_ptr) {}

std::map<HexapodMove, std::unique_ptr<INodeCreator>>
NodeCreatorSequencePruningBranch::Build(const DividedMapState& map) const {
  using enum DiscreteLegPos;
  using enum HexapodMove;

  // 追加したい場合,以下のように記述する.
  // (*node_creator)[HexapodMove::???] =
  // std::make_unique<クラス名>(クラスのコンストラクタの引数);
  // この場合,HexapodMove::???のノードを作成するクラスは,^~~~~~ である.

  std::map<HexapodMove, std::unique_ptr<INodeCreator>> node_creator;

  const auto hierarchy_list = std::vector<DiscreteLegPos>{
      kBack, kCenter, kFront, kLowerBack, kLowerFront, kUpperBack, kUpperFront};

  node_creator[kLegHierarchyChange] =
      std::make_unique<NodeCreatorLegHierarchyPruningBranch>(kLegUpDown,
                                                             hierarchy_list);

  node_creator[kLegUpDown] =
      std::make_unique<NodeCreatorLegUpDownPruningBranch>(
          map, converter_ptr_, presenter_ptr_, checker_ptr_, kComUpDown);

  node_creator[kComUpDown] = std::make_unique<NodeCreatorComUpDown>(
      map, converter_ptr_, presenter_ptr_, checker_ptr_, kComMove);

  node_creator[kComMove] = std::make_unique<NodeCreatorComMoveStraight>(
      map, converter_ptr_, presenter_ptr_, checker_ptr_, kLegHierarchyChange);

  return node_creator;
}

}  // namespace designlab
