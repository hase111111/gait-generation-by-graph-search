
//! @file gpg_builder_flat.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "gpg_builder_flat.h"

#include <utility>

#include "cassert_define.h"
#include "graph_searcher_straight_move.h"
#include "graph_tree_creator.h"
#include "gait_pattern_generator_basic.h"
#include "node_creator_builder_straight_move.h"
#include "node_creator_builder_body_rot.h"

namespace designlab {

GpgBuilderFlat::GpgBuilderFlat(
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

std::unique_ptr<IGaitPatternGenerator> GpgBuilderFlat::Build() const {
    auto node_creator_builder =
        std::make_unique<NodeCreatorBuilderBodyRot>(
            converter_ptr_, presenter_ptr_, checker_ptr_);
    auto graph_tree_creator =
        std::make_unique<GraphTreeCreator>(std::move(node_creator_builder));

    auto graph_searcher = std::make_unique<GraphSearcherStraightMove>(checker_ptr_);

    auto gait_pattern_generator =
        std::make_unique<GaitPatternGeneratorBasic>(
            std::move(graph_tree_creator), std::move(graph_searcher), 5, 50000000);

    return std::move(gait_pattern_generator);
}

}  // namespace designlab
