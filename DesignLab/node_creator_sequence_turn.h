
//! @file node_creator_sequence_turn.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <map>
#include <memory>

#include "interface_node_creator_sequence.h"

namespace gaitgen {

//! @class NodeCreatorSequenceTurn
//! @brief 旋回動作用のノード生成クラスのビルダー.
class NodeCreatorSequenceTurn final : public INodeCreatorSequence {
 public:
  NodeCreatorSequenceTurn(
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

}  // namespace gaitgen
