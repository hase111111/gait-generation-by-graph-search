
//! @file node_creator_builder_ground_conforming_rot.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php


#include "node_creator_builder_ground_conforming_rot.h"

#include "cassert_define.h"

namespace designlab {

NodeCreatorBuilderGroundConformingRot::NodeCreatorBuilderGroundConformingRot(
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
    const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
    const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr) :
    converter_ptr_(converter_ptr),
    presenter_ptr_(presenter_ptr),
    checker_ptr_(checker_ptr) {
    assert(converter_ptr_ != nullptr);
    assert(presenter_ptr_ != nullptr);
    assert(checker_ptr_ != nullptr);
}

void NodeCreatorBuilderGroundConformingRot::Build(
    const DividedMapState& map,
    std::map<HexapodMove, std::unique_ptr<INodeCreator>>* node_creator) const {

}

}  // namespace designlab
