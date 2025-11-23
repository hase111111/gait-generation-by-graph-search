
//! @file dead_lock_checker.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "dead_lock_checker.h"

namespace designlab {

void DeadLockChecker::AddNode(const RobotStateNode& node) {
  // ノードを前から追加する
  node_vec_.push_front(node);

  // 最大保持数を超えているならばその分だけ,後ろから削除する.
  while (node_vec_.size() > kMaxDataNum) {
    node_vec_.pop_back();
  }
}

bool DeadLockChecker::IsDeadLock() const {
  // 最大数に達していない場合は,デッドロックしていないと判断する.
  if (node_vec_.size() < kMaxDataNum) {
    return false;
  }

  // 最新のノードと最も古いノードを比較し，
  // 重心位置と姿勢が同じならばデッドロックしていると判断する.
  const auto& latest_node = node_vec_.front();
  const auto& oldest_node = node_vec_.back();

  const float dist = 100.f;

  if (latest_node.center_of_mass_global_coord.GetDistanceFrom(
          oldest_node.center_of_mass_global_coord) < dist &&
      latest_node.posture == oldest_node.posture) {
    return true;
  }

  return false;
}

}  // namespace designlab
