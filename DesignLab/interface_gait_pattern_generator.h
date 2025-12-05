
//! @file interface_gait_pattern_generator.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <vector>

#include "graph_search_result_record.h"
#include "map_state.h"
#include "my_expected.h"
#include "robot_operation.h"
#include "robot_state_node.h"

namespace designlab {

//! @class IGaitPatternGenerator
//! @brief グラフ探索による歩容パターン生成を行うクラスのインターフェース.
//! @details
//! @n 波東さんのプログラムで言うところのPassFindingクラス.
//! 実体は作成できないのでこれを継承してたクラスを使うこと.
//! @n 継承をするクラスのデストラクタは virtualにしておく.
//!
//! 参考
//! @li C++ でデストラクタを virtual にしなくてはならない条件と理由
//! https://www.yunabe.jp/docs/cpp_virtual_destructor.html (アクセス日
//! 2023/12/27)
class IGaitPatternGenerator {
 public:
  IGaitPatternGenerator() = default;
  virtual ~IGaitPatternGenerator() = default;

  //! @brief グラフ探索を行い,次の動作として最適なノードを返す.
  //! @param[in] current_node 現在のロボットの状態を表すノード.
  //! 親ノードを渡す必要がある.
  //! @param[in] map 現在のマップの状態.
  //! @param[in] operation ロボットの動作.
  //! @return expected型で次のノードを返す.失敗時は文字列で理由を返す.
  virtual nostd::expected<RobotStateNode, std::string> GetNextNodeByGraphSearch(
      const RobotStateNode& current_node, const MapState& map,
      const RobotOperation& operation) = 0;

  virtual int GetExpandedNodeCount() const = 0;

  virtual std::vector<int> GetExpandedNodeCountPerDepth() const = 0;
};

}  // namespace designlab
