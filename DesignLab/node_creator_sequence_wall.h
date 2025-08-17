
//! @file node_creator_sequence_wall.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_NODE_CREATOR_SEQUENCE_WALL_H_
#define DESIGNLAB_NODE_CREATOR_SEQUENCE_WALL_H_

#include <map>
#include <memory>

#include "interface_node_creator_sequence.h"

namespace designlab {

//! @class NodeCreatorSequenceWall
//! @brief 第5報の直進動作を行うノードを生成するクラス.
class NodeCreatorSequenceWall final : public INodeCreatorSequence {
 public:
  NodeCreatorSequenceWall(
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
      const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr);

  std::map<HexapodMove, std::unique_ptr<INodeCreator>> Build(
      const DividedMapState& map) const override;

 private:
  const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
  const std::shared_ptr<const IHexapodStatePresenter> presenter_ptr_;
  const std::shared_ptr<const IHexapodPostureValidator> checker_ptr_;
};

}  // namespace designlab

#endif  // DESIGNLAB_NODE_CREATOR_SEQUENCE_WALL_H_
