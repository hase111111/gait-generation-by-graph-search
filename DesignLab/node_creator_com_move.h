
//! @file node_creator_com_move.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <memory>
#include <vector>

#include "com_candidate_polygon_maker.h"
#include "com_selector.h"
#include "divided_map_state.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_posture_validator.h"
#include "interface_hexapod_state_presenter.h"
#include "interface_node_creator.h"
#include "math_polygon2.h"

namespace gaitgen {

//! @class NodeCreatorComMove
//! @brief 波東さんの手法で重心の平行移動を行うクラス.
class NodeCreatorComMove final : public INodeCreator {
 public:
  NodeCreatorComMove(
      const DividedMapState& devide_map,
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
      const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
      HexapodMove next_move);

  ~NodeCreatorComMove() = default;

  void Create(const RobotStateNode& current_node, int current_num,
              std::vector<RobotStateNode>* output_graph) const override;

 private:
  //! 候補地点を含む多角形を作成するクラス.
  const ComCandidatePolygonMaker maker_;

  const ComSelector selector_;  //!< 多角形から最適な地面を選択するクラス.

  const DividedMapState map_;    //!< 地面の状態を格納したクラス.
  const HexapodMove next_move_;  //!< 次の動作.

  const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
  const std::shared_ptr<const IHexapodStatePresenter> presenter_ptr_;
  const std::shared_ptr<const IHexapodPostureValidator> checker_ptr_;
};

}  // namespace gaitgen
