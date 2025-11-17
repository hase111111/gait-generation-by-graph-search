
//! @file node_creator_leg_hierarchy_pruning_branch.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "node_creator_leg_hierarchy_pruning_branch.h"

#include <fstream>

#include "cmdio_util.h"
#include "com_type.h"
#include "leg_state.h"

namespace designlab {

std::map<std::pair<int, int>, std::vector<int>>
    NodeCreatorLegHierarchyPruningBranch::leg_pos_change_table_ = {};

NodeCreatorLegHierarchyPruningBranch::NodeCreatorLegHierarchyPruningBranch(
    const HexapodMove next_move,
    const std::vector<DiscreteLegPos>& discrete_leg_pos_list)
    : next_move_(next_move), discrete_leg_pos_list_(discrete_leg_pos_list) {
  assert(discrete_leg_pos_list.size() != 0);

  if (leg_pos_change_table_.empty()) {
    leg_pos_change_table_ = InitTable();
  }
}

void NodeCreatorLegHierarchyPruningBranch::Create(
    const RobotStateNode& current_node, const int current_node_index,
    std::vector<RobotStateNode>* output_graph) const {
  const int leg_pos_index = BitToLegPosIndex(current_node.leg_state);
  const int com = BitToGroundLegIndex(current_node.leg_state);
  // cmdio::DebugOutputF("leg_pos_index: {} -> {}, {}",
  //                     current_node.leg_state.to_string(), leg_pos_index,
  //                     com);

  const auto it =
      leg_pos_change_table_.find(std::make_pair(com, leg_pos_index));

  if (it == leg_pos_change_table_.end()) {
    // cmdio::DebugOutputF(
    //     "No entry in leg_pos_change_table_ for com: {}, leg_pos_index: {}",
    //     com, leg_pos_index);
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

  // 出来上がったテーブルをprintする.
  for (const auto& [key, v] : table) {
    cmdio::DebugOutputF("com: {}, leg_pos_index: {} -> ", key.first,
                        key.second);
    for (const auto& val : v) {
      cmdio::DebugOutputF("{}, ", val);
    }
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

}  // namespace designlab
