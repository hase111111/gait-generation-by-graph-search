
//! @file node_creator_com_move_wall.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_NODE_CREATOR_COM_MOVE_WALL_H_
#define DESIGNLAB_NODE_CREATOR_COM_MOVE_WALL_H_

#include <array>
#include <memory>
#include <vector>

#include "divided_map_state.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_posture_validator.h"
#include "interface_hexapod_state_presenter.h"
#include "interface_node_creator.h"
#include "math_polygon2.h"

namespace designlab {

//! @class NodeCreatorComMoveWall
//! @brief 直線方向に重心の平行移動を行うクラス.
class NodeCreatorComMoveWall final : public INodeCreator {
 public:
  NodeCreatorComMoveWall(
      const DividedMapState& devide_map,
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
      const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
      HexapodMove next_move);

  ~NodeCreatorComMoveWall() = default;

  void Create(const RobotStateNode& current_node, int current_num,
              std::vector<RobotStateNode>* output_graph) const override;

 private:
  static constexpr int kCandidateDirectionNum = 8;  //!< 候補となる移動方向の数.
  static constexpr float kMaxMoveDistance = 70.f;   //!< 重心の移動距離の最大値.
  static constexpr float kMoveDistanceStep = 5.f;   //!< 重心の移動距離の刻み幅.

  const DividedMapState map_;    //!< 地面の状態を格納したクラス.
  const HexapodMove next_move_;  //!< 次の動作.

  const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
  const std::shared_ptr<const IHexapodStatePresenter> presenter_ptr_;
  const std::shared_ptr<const IHexapodPostureValidator> checker_ptr_;

  //! 候補となる移動方向.
  std::array<Vector3, kCandidateDirectionNum> candidate_directions_;
};

}  // namespace designlab

#endif  // DESIGNLAB_NODE_CREATOR_COM_MOVE_WALL_H_
