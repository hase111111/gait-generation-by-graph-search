
//! @file gpg_selector.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "gpg_selector.h"

#include <utility>

#include "cassert_define.h"
#include "gait_pattern_generator_basic.h"
#include "gait_pattern_generator_switch_by_map.h"
#include "gait_pattern_generator_thread.h"
#include "graph_searcher_plane.h"
#include "graph_searcher_spot_turn.h"
#include "graph_searcher_straight_move.h"
#include "graph_searcher_wall.h"
#include "graph_tree_creator.h"
#include "node_creator_sequence_body_rot.h"
#include "node_creator_sequence_straight_move.h"
#include "node_creator_sequence_straight_move_legacy.h"
#include "node_creator_sequence_wall.h"

namespace designlab {

GpgSelector::GpgSelector(
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
    const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
    const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr)
    : converter_ptr_(converter_ptr),
      presenter_ptr_(presenter_ptr),
      checker_ptr_(checker_ptr) {
  assert(converter_ptr_ != nullptr);
  assert(presenter_ptr_ != nullptr);
  assert(checker_ptr_ != nullptr);
}

std::unique_ptr<IGaitPatternGenerator> GpgSelector::Select(
    const GpgType type) const {
  switch (type) {
    case GpgType::kFlat: {
      return MakeGpgFlat();
    }
    case GpgType::kFlatLegacyAlgorithm: {
      return MakeGpgFlatLegacyAlgorithm();
    }
    case GpgType::kPitchRot: {
      return MakeGpgPitchRot();
    }
    case GpgType::kSwichByMap: {
      return MakeGpgSwitchByMap();
    }
    case GpgType::kWall: {
      return MakeGpgWall();
    }
    default: {
      break;
    }
  }

  assert(false);  // 未実装のGpgTypeが指定された.
  return nullptr;
}

std::unique_ptr<IGaitPatternGenerator> GpgSelector::MakeGpgFlat() const {
  auto node_creator_seq = std::make_unique<NodeCreatorSequenceStraightMove>(
      converter_ptr_, presenter_ptr_, checker_ptr_);
  auto graph_tree_creator =
      std::make_unique<GraphTreeCreator>(std::move(node_creator_seq));

  auto graph_searcher =
      std::make_unique<GraphSearcherStraightMove>(checker_ptr_);

  auto gait_pattern_generator = std::make_unique<GaitPatternGeneratorThread>(
      std::move(graph_tree_creator), std::move(graph_searcher), 5, 30000000);

  return std::move(gait_pattern_generator);
}

std::unique_ptr<IGaitPatternGenerator> GpgSelector::MakeGpgFlatLegacyAlgorithm()
    const {
  auto node_creator_seq =
      std::make_unique<NodeCreatorSequenceStraightMoveLegacy>(
          converter_ptr_, presenter_ptr_, checker_ptr_);
  auto graph_tree_creator =
      std::make_unique<GraphTreeCreator>(std::move(node_creator_seq));

  auto graph_searcher =
      std::make_unique<GraphSearcherStraightMove>(checker_ptr_);

  auto gait_pattern_generator = std::make_unique<GaitPatternGeneratorBasic>(
      std::move(graph_tree_creator), std::move(graph_searcher), 6, 20000000);

  return std::move(gait_pattern_generator);
}

std::unique_ptr<IGaitPatternGenerator> GpgSelector::MakeGpgPitchRot() const {
  auto node_creator_builder = std::make_unique<NodeCreatorSequenceBodyRot>(
      converter_ptr_, presenter_ptr_, checker_ptr_);
  auto graph_tree_creator =
      std::make_unique<GraphTreeCreator>(std::move(node_creator_builder));

  auto graph_searcher = std::make_unique<GraphSearcherPlane>(
      checker_ptr_, presenter_ptr_, converter_ptr_);

  auto gait_pattern_generator = std::make_unique<GaitPatternGeneratorBasic>(
      std::move(graph_tree_creator), std::move(graph_searcher), 5, 20000000);

  return std::move(gait_pattern_generator);
}

std::unique_ptr<IGaitPatternGenerator> GpgSelector::MakeGpgRollRot() const {
  auto node_creator_builder = std::make_unique<NodeCreatorSequenceBodyRot>(
      converter_ptr_, presenter_ptr_, checker_ptr_, Vector3::GetFrontVec());
  auto graph_tree_creator =
      std::make_unique<GraphTreeCreator>(std::move(node_creator_builder));

  auto graph_searcher = std::make_unique<GraphSearcherPlane>(
      checker_ptr_, presenter_ptr_, converter_ptr_);

  auto gait_pattern_generator = std::make_unique<GaitPatternGeneratorBasic>(
      std::move(graph_tree_creator), std::move(graph_searcher), 5, 20000000);

  return std::move(gait_pattern_generator);
}

std::unique_ptr<IGaitPatternGenerator> GpgSelector::MakeGpgSwitchByMap() const {
  auto gpg_switch_by_map = std::make_unique<GaitPatternGeneratorSwitchByMap>(
      MakeGpgFlat(), MakeGpgPitchRot(), MakeGpgRollRot());

  return std::move(gpg_switch_by_map);
}

std::unique_ptr<IGaitPatternGenerator> GpgSelector::MakeGpgWall() const {
  auto node_creator_seq = std::make_unique<NodeCreatorSequenceWall>(
      converter_ptr_, presenter_ptr_, checker_ptr_);
  auto graph_tree_creator =
      std::make_unique<GraphTreeCreator>(std::move(node_creator_seq));

  auto graph_searcher =
      std::make_unique<GraphSearcherStraightMove>(checker_ptr_);

  auto gait_pattern_generator = std::make_unique<GaitPatternGeneratorBasic>(
      std::move(graph_tree_creator), std::move(graph_searcher), 5, 20000000);

  return std::move(gait_pattern_generator);
}

}  // namespace designlab
