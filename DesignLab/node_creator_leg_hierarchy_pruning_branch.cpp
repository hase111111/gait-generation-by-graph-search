
//! @file node_creator_leg_hierarchy_pruning_branch.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "node_creator_leg_hierarchy_pruning_branch.h"

#include <fstream>

#include "cmdio_util.h"
#include "com_type.h"
#include "leg_state.h"

namespace gaitgen {

std::map<std::pair<int, int>, std::vector<int>>
    NodeCreatorLegHierarchyPruningBranch::leg_pos_change_table_ = {};

std::set<int>
    NodeCreatorLegHierarchyPruningBranch::leg_pos_change_table_legacy = {};

NodeCreatorLegHierarchyPruningBranch::NodeCreatorLegHierarchyPruningBranch(
    const HexapodMove next_move,
    const std::vector<DiscreteLegPos>& discrete_leg_pos_list, bool use_legacy)
    : next_move_(next_move),
      discrete_leg_pos_list_(discrete_leg_pos_list),
      use_legacy_(use_legacy) {
  assert(discrete_leg_pos_list.size() != 0);

  if (leg_pos_change_table_.empty()) {
    leg_pos_change_table_ = InitTable();
  }

  if (leg_pos_change_table_legacy.empty()) {
    leg_pos_change_table_legacy = InitTableLegacy();
  }
}

void NodeCreatorLegHierarchyPruningBranch::Create(
    const RobotStateNode& current_node, const int current_node_index,
    std::vector<RobotStateNode>* output_graph) const {
  if (use_legacy_) {
    cmdio::DebugOutputF("Using normal create");
    CreateLegacy(current_node, current_node_index, output_graph);
    return;
  }

  const int leg_pos_index = BitToLegPosIndex(current_node.leg_state);
  const int com = BitToGroundLegIndex(current_node.leg_state);
  cmdio::DebugOutputF("leg_pos_index: {} -> {}, {}",
                      current_node.leg_state.to_string(), leg_pos_index, com);

  const auto it =
      leg_pos_change_table_.find(std::make_pair(com, leg_pos_index));

  if (it == leg_pos_change_table_.end()) {
    cmdio::DebugOutputF(
        "No entry in leg_pos_change_table_ for com: {}, leg_pos_index: {}", com,
        leg_pos_index);
    return;
  }

  for (const auto& new_leg_pos_index : it->second) {
    RobotStateNode new_node = current_node;
    const auto new_leg_pos_list = IndexToLegPosList(new_leg_pos_index);
    for (int leg_num = 0; leg_num < HexapodConst::kLegNum; leg_num++) {
      const auto discrete_pos = new_leg_pos_list[leg_num];
      leg_func::ChangeDiscreteLegPos(leg_num, discrete_pos,
                                     &new_node.leg_state);
    }
    new_node.ChangeToNextNode(current_node_index, next_move_);
    output_graph->emplace_back(new_node);
  }

  if (output_graph->size() > 50) {
    cmdio::DebugOutputF("Created {} new nodes from current_node_index: {} {}",
                        static_cast<int>(output_graph->size()), com,
                        leg_pos_index);
  }
}

std::map<std::pair<int, int>, std::vector<int>>
NodeCreatorLegHierarchyPruningBranch::InitTable() const {
  const std::string path = "simulation_condition/transition_pairs.csv";

  std::ifstream ifs(path);

  // ファイルを読み込む.
  std::string str;
  std::vector<std::string> str_list;

  while (std::getline(ifs, str)) {
    str_list.push_back(str);
  }
  cmdio::DebugOutputF("str_list.size(): {}", str_list.size());

  // ファイルを閉じる.
  ifs.close();

  std::map<std::pair<int, int>, std::vector<int>> table;

  bool is_header = true;
  for (const auto& i : str_list) {
    if (is_header) {
      is_header = false;
      continue;
    }

    std::stringstream ss(i);
    int from_state;
    int to_state;
    int change_value;
    char comma;
    ss >> from_state >> comma >> to_state >> comma >> change_value;
    // テーブルに追加.
    table[std::make_pair(from_state, to_state)].push_back(change_value);
    std::vector<int>& v = table[std::make_pair(from_state, to_state)];
    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());
    // cmdio::DebugOutputF("from: {}, to: {}, change: {}", from_state, to_state,
    //                     change_value);
  }

  // 出来上がったテーブルを print する.
  for (const auto& [key, v] : table) {
    cmdio::DebugOutputF("com: {}, leg_pos_index: {} -> ", key.first,
                        key.second);
    for (const auto& val : v) {
      cmdio::DebugOutputF("{}, ", val);
    }
  }
  return table;
}

std::set<int> NodeCreatorLegHierarchyPruningBranch::InitTableLegacy() const {
  const std::string path = "simulation_condition/transition_pairs.csv";

  std::ifstream ifs(path);

  // ファイルを読み込む.
  std::string str;
  std::vector<std::string> str_list;

  while (std::getline(ifs, str)) {
    str_list.push_back(str);
  }
  cmdio::DebugOutputF("str_list.size(): {}", str_list.size());

  // ファイルを閉じる.
  ifs.close();

  std::set<int> table;

  bool is_header = true;
  for (const auto& i : str_list) {
    if (is_header) {
      is_header = false;
      continue;
    }

    std::stringstream ss(i);
    int _{}, to_state{}, change_value{};
    char comma;
    ss >> _ >> comma >> to_state >> comma >> change_value;
    // テーブルに追加.
    table.insert(to_state);
    table.insert(change_value);
  }

  return table;
}

int NodeCreatorLegHierarchyPruningBranch::BitToLegPosIndex(
    const leg_func::LegStateBit& leg_bit) const {
  int res = 0;
  for (int i = 0; i < HexapodConst::kLegNum; i++) {
    const auto discrete_pos =
        leg_func::GetDiscreteLegPos(leg_bit, HexapodConst::kLegNum - i - 1);
    const int pos_index = static_cast<int>(discrete_pos) - 1;
    // cmdio::DebugOutputF("pos_index: {} ", pos_index);
    int add_value = pos_index;
    for (int j = 0; j < i; j++) {
      add_value *= 7;
    }
    res += add_value;
    // cmdio::DebugOutputF("res: {} ", res);
  }
  return res;
}

int NodeCreatorLegHierarchyPruningBranch::BitToGroundLegIndex(
    const leg_func::LegStateBit& leg_bit) const {
  const auto bit = leg_func::GetLegGroundedBit(leg_bit);
  const auto index = com_func::GetLegGroundPatternIndexFromLegGroundedBit(bit);
  return index;
}

std::vector<DiscreteLegPos>
NodeCreatorLegHierarchyPruningBranch::IndexToLegPosList(int index) const {
  std::vector<DiscreteLegPos> table;
  for (int i = 0; i < HexapodConst::kLegNum; i++) {
    const int pos_index = index % 7;
    table.emplace_back(static_cast<DiscreteLegPos>(pos_index + 1));
    index /= 7;
  }
  return table;
}

void NodeCreatorLegHierarchyPruningBranch::CreateLegacy(
    const RobotStateNode& current_node, const int current_node_index,
    std::vector<RobotStateNode>* output_graph) const {
  // 現在,接地している脚の本数を数える.
  const int kLiftedLegNum = leg_func::GetLiftedLegNum(current_node.leg_state);

  // 遊脚している脚の本数によって処理をする.
  if (kLiftedLegNum == 1) {
    // 1 本遊脚している.
    Create1LegLifted(current_node, current_node_index, output_graph);
  } else if (kLiftedLegNum == 2) {
    // 2 本遊脚している.
    Create2LegLifted(current_node, current_node_index, output_graph);
  } else if (kLiftedLegNum == 3) {
    // 3 本遊脚している.
    Create3LegLifted(current_node, current_node_index, output_graph);
  } else {
    // ここに来るのは接地している脚の数が6本 or 1本 or 2本の時.
    // 地面についている脚が3本を切ることはない,何故ならロボットが倒れてしまうため.
    // また6本接地しているならば脚を動かせない(遊脚する必要がある).
    // よって処理を行わない.(そのままの状態を次のノードにする.)
    RobotStateNode new_node = current_node;

    // 次のノード用に,深さ・親・次の動作を更新する.
    new_node.ChangeToNextNode(current_node_index, next_move_);

    // 追加する.
    (*output_graph).emplace_back(new_node);
  }
}

void NodeCreatorLegHierarchyPruningBranch::Create1LegLifted(
    const RobotStateNode& current_node, const int current_node_index,
    std::vector<RobotStateNode>* output_graph) const {
  // 遊脚している脚を探す.遊脚数は1なので1つの数字が帰るはず.
  std::vector<int> lifted_leg_list;

  leg_func::GetLiftedLegIndexByVector(current_node.leg_state, &lifted_leg_list);

  // 列挙体 DiscreteLegPos の全ての要素でループを回す.
  for (const auto i : discrete_leg_pos_list_) {
    RobotStateNode new_node = current_node;  // 新しい脚状態を生成する.

    // 脚状態を変更する.
    leg_func::ChangeDiscreteLegPos(lifted_leg_list[0], i, &new_node.leg_state);

    // 次のノード用に,深さ・親・次の動作を更新する.
    new_node.ChangeToNextNode(current_node_index, next_move_);

    if (const auto new_idx = BitToLegPosIndex(new_node.leg_state);
        leg_pos_change_table_legacy.find(new_idx) ==
        leg_pos_change_table_legacy.end()) {
      continue;
    }

    (*output_graph).emplace_back(new_node);  // 追加する.
  }
}

void NodeCreatorLegHierarchyPruningBranch::Create2LegLifted(
    const RobotStateNode& current_node, const int current_node_index,
    std::vector<RobotStateNode>* output_graph) const {
  using leg_func::ChangeDiscreteLegPos;

  // 遊脚している脚を探す.遊脚数は2なので2つの数字が帰るはず.
  std::vector<int> lifted_leg_list;

  leg_func::GetLiftedLegIndexByVector(current_node.leg_state, &lifted_leg_list);

  // 列挙体 DiscreteLegPos の全ての要素でループを回す.
  for (const auto i : discrete_leg_pos_list_) {
    for (const auto j : discrete_leg_pos_list_) {
      RobotStateNode new_node = current_node;  // 新しい脚状態を生成する.

      // 脚状態を変更する.
      ChangeDiscreteLegPos(lifted_leg_list[0], i, &new_node.leg_state);
      ChangeDiscreteLegPos(lifted_leg_list[1], j, &new_node.leg_state);

      // 次のノード用に,深さ・親・次の動作を更新する.
      new_node.ChangeToNextNode(current_node_index, next_move_);

      if (const auto new_idx = BitToLegPosIndex(new_node.leg_state);
          leg_pos_change_table_legacy.find(new_idx) ==
          leg_pos_change_table_legacy.end()) {
        continue;
      }

      (*output_graph).emplace_back(new_node);  // 追加する.
    }
  }
}

void NodeCreatorLegHierarchyPruningBranch::Create3LegLifted(
    const RobotStateNode& current_node, const int current_node_index,
    std::vector<RobotStateNode>* output_graph) const {
  using leg_func::ChangeDiscreteLegPos;

  // 遊脚している脚を探す.遊脚数は3なので3つの数字が帰るはず.
  std::vector<int> lifted_leg_list;

  leg_func::GetLiftedLegIndexByVector(current_node.leg_state, &lifted_leg_list);

  // 列挙体 DiscreteLegPos の全ての要素でループを回す.
  for (const auto i : discrete_leg_pos_list_) {
    for (const auto j : discrete_leg_pos_list_) {
      for (const auto k : discrete_leg_pos_list_) {
        RobotStateNode new_node = current_node;  // 新しい脚状態を生成する.

        // 脚状態を変更する.
        ChangeDiscreteLegPos(lifted_leg_list[0], i, &new_node.leg_state);
        ChangeDiscreteLegPos(lifted_leg_list[1], j, &new_node.leg_state);
        ChangeDiscreteLegPos(lifted_leg_list[2], k, &new_node.leg_state);

        // 次のノード用に,深さ・親・次の動作を更新する.
        new_node.ChangeToNextNode(current_node_index, next_move_);

        if (const auto new_idx = BitToLegPosIndex(new_node.leg_state);
            leg_pos_change_table_legacy.find(new_idx) ==
            leg_pos_change_table_legacy.end()) {
          continue;
        }

        (*output_graph).push_back(new_node);  // 追加する.
      }
    }
  }
}

}  // namespace gaitgen
