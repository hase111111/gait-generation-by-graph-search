
//! @file node_creator_sequence_pruning_branch.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_NODE_CREATOR_SEQUENCE_PRUNING_BRANCH_H_
#define DESIGNLAB_NODE_CREATOR_SEQUENCE_PRUNING_BRANCH_H_

#include <map>
#include <memory>

#include "interface_node_creator_sequence.h"

namespace designlab {

//! @class NodeCreatorSequencePruningBranch
//! @brief 第5報の直進動作を行うノードを生成するクラス.
class NodeCreatorSequencePruningBranch final : public INodeCreatorSequence {
 public:
  NodeCreatorSequencePruningBranch(
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
      const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
      bool use_normal);

  std::map<HexapodMove, std::unique_ptr<INodeCreator>> Build(
      const DividedMapState& map) const override;

 private:
  const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
  const std::shared_ptr<const IHexapodStatePresenter> presenter_ptr_;
  const std::shared_ptr<const IHexapodPostureValidator> checker_ptr_;
  const bool use_normal_;
};

}  // namespace designlab

#endif  // DESIGNLAB_NODE_CREATOR_SEQUENCE_PRUNING_BRANCH_H_
