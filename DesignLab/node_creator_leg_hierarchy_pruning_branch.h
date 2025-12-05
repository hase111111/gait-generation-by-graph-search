
//! @file node_creator_leg_hierarchy_pruning_branch.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <map>
#include <memory>
#include <set>
#include <utility>
#include <vector>

#include "discrete_leg_pos.h"
#include "hexapod_next_move.h"
#include "interface_node_creator.h"
#include "leg_state.h"

namespace gaitgen {

//! @class NodeCreatorLegHierarchyPruningBranch
//! @brief 脚の階層構造を作るためのクラス.
class NodeCreatorLegHierarchyPruningBranch final : public INodeCreator {
 public:
  //! @brief コンストラクタ.
  //! @param[in] next_move 次の動作.
  //! @param[in] discrete_leg_pos_list 離散化された脚位置のリスト.
  NodeCreatorLegHierarchyPruningBranch(
      HexapodMove next_move,
      const std::vector<DiscreteLegPos>& discrete_leg_pos_list,
      bool use_legacy);

  ~NodeCreatorLegHierarchyPruningBranch() = default;

  void Create(const RobotStateNode& current_node, int current_node_index,
              std::vector<RobotStateNode>* output_nodes) const override;

 private:
  std::map<std::pair<int, int>, std::vector<int>> InitTable() const;
  std::set<int> InitTableLegacy() const;

  int BitToLegPosIndex(const leg_func::LegStateBit& leg_bit) const;
  int BitToGroundLegIndex(const leg_func::LegStateBit& leg_bit) const;
  std::vector<DiscreteLegPos> IndexToLegPosList(int index) const;

  void CreateLegacy(const RobotStateNode& current_node, int current_node_index,
                    std::vector<RobotStateNode>* output_nodes) const;

  void Create1LegLifted(const RobotStateNode& current_node,
                        int current_node_index,
                        std::vector<RobotStateNode>* output_nodes) const;

  void Create2LegLifted(const RobotStateNode& current_node,
                        int current_node_index,
                        std::vector<RobotStateNode>* output_nodes) const;

  void Create3LegLifted(const RobotStateNode& current_node,
                        int current_node_index,
                        std::vector<RobotStateNode>* output_nodes) const;

  const HexapodMove next_move_;  //!< 次の動作.

  //! 離散化された脚位置のリスト,このリストの中の値から脚の状態を変更する.
  const std::vector<DiscreteLegPos> discrete_leg_pos_list_;

  static std::map<std::pair<int, int>, std::vector<int>> leg_pos_change_table_;
  static std::set<int> leg_pos_change_table_legacy;

  const bool use_legacy_;
};

}  // namespace gaitgen
