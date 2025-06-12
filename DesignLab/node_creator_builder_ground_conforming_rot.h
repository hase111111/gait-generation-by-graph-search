
//! @file node_creator_builder_ground_conforming_rot.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_NODE_CREATOR_BUILDER_GROUND_CONFORMING_ROT_H_
#define DESIGNLAB_NODE_CREATOR_BUILDER_GROUND_CONFORMING_ROT_H_

#include <map>
#include <memory>

#include "interface_node_creator_builder.h"

namespace designlab {

//! @class NodeCreatorBuilderGroundConformingRot
//! @brief
//! 波東さんが行った処理と同様の方法になるようにノード生成クラスを生成するクラス.
class NodeCreatorBuilderGroundConformingRot final : public INodeCreatorBuilder {
 public:
  NodeCreatorBuilderGroundConformingRot(
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
      const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr);

  void Build(const DividedMapState& map,
             std::map<HexapodMove, std::unique_ptr<INodeCreator> >*
                 node_creator) const override;

 private:
  const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
  const std::shared_ptr<const IHexapodStatePresenter> presenter_ptr_;
  const std::shared_ptr<const IHexapodPostureValidator> checker_ptr_;
};

}  // namespace designlab

#endif  // DESIGNLAB_NODE_CREATOR_BUILDER_GROUND_CONFORMING_ROT_H_
