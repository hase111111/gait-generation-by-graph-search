
//! @file node_creator_sequence_body_rot.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <map>
#include <memory>

#include "interface_node_creator_sequence.h"

namespace gaitgen {

//! @class NodeCreatorSequenceBodyRot
//! @brief ノード作成ビルダーのボディ回転クラス.
class NodeCreatorSequenceBodyRot final : public INodeCreatorSequence {
 public:
  //! @todo デフォルト引数を削除する.
  NodeCreatorSequenceBodyRot(
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
      const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
      const Vector3& body_rot_axis = Vector3::GetLeftVec());

  std::map<HexapodMove, std::unique_ptr<INodeCreator>> Build(
      const DividedMapState& map) const override;

 private:
  const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
  const std::shared_ptr<const IHexapodStatePresenter> presenter_ptr_;
  const std::shared_ptr<const IHexapodPostureValidator> checker_ptr_;
  const Vector3 body_rot_axis_;  //!< ボディ回転の軸
};

}  // namespace gaitgen
