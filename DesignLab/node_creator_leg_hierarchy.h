﻿
//! @file node_creator_leg_hierarchy.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_NODE_CREATOR_LEG_HIERARCHY_H_
#define DESIGNLAB_NODE_CREATOR_LEG_HIERARCHY_H_

#include <memory>
#include <vector>

#include "discrete_leg_pos.h"
#include "hexapod_next_move.h"
#include "interface_node_creator.h"

namespace designlab {

//! @class NodeCreatorLegHierarchy
//! @brief 脚の階層構造を作るためのクラス.
class NodeCreatorLegHierarchy final : public INodeCreator {
 public:
  //! @brief コンストラクタ.
  //! @param[in] next_move 次の動作.
  //! @param[in] discrete_leg_pos_list 離散化された脚位置のリスト.
  NodeCreatorLegHierarchy(
      HexapodMove next_move,
      const std::vector<DiscreteLegPos>& discrete_leg_pos_list);

  ~NodeCreatorLegHierarchy() = default;

  void Create(const RobotStateNode& current_node, int current_node_index,
              std::vector<RobotStateNode>* output_nodes) const override;

 private:
  //! @brief 1脚が遊脚しているとき,その脚の状態を別の状態に変更する.
  //! @param[in] current_node 現在のノード.
  //! @param[in] current_node_index 現在のノードのインデックス.
  //! @param[out] output_nodes 生成されたノードのリスト.
  void Create1LegLifted(const RobotStateNode& current_node,
                        int current_node_index,
                        std::vector<RobotStateNode>* output_nodes) const;

  //! @brief 2脚が遊脚しているとき,その脚の状態を別の状態に変更する.
  //! @param[in] current_node 現在のノード.
  //! @param[in] current_node_index 現在のノードのインデックス.
  //! @param[out] output_nodes 生成されたノードのリスト.
  void Create2LegLifted(const RobotStateNode& current_node,
                        int current_node_index,
                        std::vector<RobotStateNode>* output_nodes) const;

  //! @brief 3脚が遊脚しているとき,その脚の状態を別の状態に変更する.
  //! @param[in] current_node 現在のノード.
  //! @param[in] current_node_index 現在のノードのインデックス.
  //! @param[out] output_nodes 生成されたノードのリスト.
  void Create3LegLifted(const RobotStateNode& current_node,
                        int current_node_index,
                        std::vector<RobotStateNode>* output_nodes) const;

  const HexapodMove next_move_;  //!< 次の動作.

  //! 離散化された脚位置のリスト,このリストの中の値から脚の状態を変更する.
  const std::vector<DiscreteLegPos> discrete_leg_pos_list_;
};

}  // namespace designlab

#endif  // DESIGNLAB_NODE_CREATOR_LEG_HIERARCHY_H_
