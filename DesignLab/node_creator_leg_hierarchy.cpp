
//! @author    Hasegawa
//! @copyright 埼玉大学 設計工学研究室 2023. All right reserved.

#include "node_creator_leg_hierarchy.h"

#include "leg_state.h"


namespace designlab
{

NodeCreatorLegHierarchy::NodeCreatorLegHierarchy(
    const enums::HexapodMove next_move,
    const std::vector<enums::DiscreteLegPos>& discrete_leg_pos_list) :
    next_move_(next_move),
    discrete_leg_pos_list_(discrete_leg_pos_list)
{
    assert(discrete_leg_pos_list.size() != 0);
}


void NodeCreatorLegHierarchy::Create(
    const RobotStateNode& current_node,
    const int current_node_index,
    std::vector<RobotStateNode>* output_graph) const
{
    // 現在，接地している脚の本数を数える．
    const int kLiftedLegNum = leg_func::GetLiftedLegNum(current_node.leg_state);

    // 遊脚している脚の本数によって処理をする．
    if (kLiftedLegNum == 1)
    {
        // 1 本遊脚している．
        Create1LegLifted(current_node, current_node_index, output_graph);
    }
    else if (kLiftedLegNum == 2)
    {
        // 2 本遊脚している．
        Create2LegLifted(current_node, current_node_index, output_graph);
    }
    else if (kLiftedLegNum == 3)
    {
        // 3 本遊脚している．
        Create3LegLifted(current_node, current_node_index, output_graph);
    }
    else
    {
        // ここに来るのは接地している脚の数が6本 or 1本 or 2本の時．
        // 地面についている脚が3本を切ることはない，何故ならロボットが倒れてしまうため．
        // また6本接地しているならば脚を動かせない(遊脚する必要がある)．
        // よって処理を行わない．(そのままの状態を次のノードにする．)
        RobotStateNode new_node = current_node;

        // 次のノード用に，深さ・親・次の動作を更新する．
        new_node.ChangeToNextNode(current_node_index, next_move_);

        // 追加する．
        (*output_graph).emplace_back(new_node);
    }
}


// 全て上の関数にまとめると文量が増えるため，以下の関数に処理を分けておく．

void NodeCreatorLegHierarchy::Create1LegLifted(
    const RobotStateNode& current_node,
    const int current_node_index,
    std::vector<RobotStateNode>* output_graph) const
{
    // 遊脚している脚を探す．遊脚数は1なので1つの数字が帰るはず
    std::vector<int> lifted_leg_list;

    leg_func::GetLiftedLegIndexByVector(current_node.leg_state, &lifted_leg_list);


    // 列挙体 DiscreteLegPos の全ての要素でループを回す．
    for (const auto i : discrete_leg_pos_list_)
    {
        RobotStateNode new_node = current_node;  // 新しい脚状態を生成する.

        // 脚状態を変更する．
        leg_func::ChangeDiscreteLegPos(lifted_leg_list[0], i, &new_node.leg_state);

        // 次のノード用に，深さ・親・次の動作を更新する．
        new_node.ChangeToNextNode(current_node_index, next_move_);

        (*output_graph).emplace_back(new_node);  // 追加する．
    }
}


void NodeCreatorLegHierarchy::Create2LegLifted(
    const RobotStateNode& current_node,
    const int current_node_index,
    std::vector<RobotStateNode>* output_graph) const
{
    using leg_func::ChangeDiscreteLegPos;

    // 遊脚している脚を探す．遊脚数は2なので2つの数字が帰るはず
    std::vector<int> lifted_leg_list;

    leg_func::GetLiftedLegIndexByVector(current_node.leg_state, &lifted_leg_list);


    // 列挙体 DiscreteLegPos の全ての要素でループを回す．
    for (const auto i : discrete_leg_pos_list_)
    {
        for (const auto j : discrete_leg_pos_list_)
        {
            RobotStateNode new_node = current_node;  // 新しい脚状態を生成する.

            // 脚状態を変更する．
            ChangeDiscreteLegPos(lifted_leg_list[0], i, &new_node.leg_state);
            ChangeDiscreteLegPos(lifted_leg_list[1], j, &new_node.leg_state);

            // 次のノード用に，深さ・親・次の動作を更新する．
            new_node.ChangeToNextNode(current_node_index, next_move_);

            (*output_graph).emplace_back(new_node);  // 追加する．
        }
    }
}


void NodeCreatorLegHierarchy::Create3LegLifted(
    const RobotStateNode& current_node,
    const int current_node_index,
    std::vector<RobotStateNode>* output_graph) const
{
    using leg_func::ChangeDiscreteLegPos;

    // 遊脚している脚を探す．遊脚数は3なので3つの数字が帰るはず
    std::vector<int> lifted_leg_list;

    leg_func::GetLiftedLegIndexByVector(current_node.leg_state, &lifted_leg_list);


    // 列挙体 DiscreteLegPos の全ての要素でループを回す．
    for (const auto i : discrete_leg_pos_list_)
    {
        for (const auto j : discrete_leg_pos_list_)
        {
            for (const auto k : discrete_leg_pos_list_)
            {
                RobotStateNode new_node = current_node;  // 新しい脚状態を生成する.

                // 脚状態を変更する．
                ChangeDiscreteLegPos(lifted_leg_list[0], i, &new_node.leg_state);
                ChangeDiscreteLegPos(lifted_leg_list[1], j, &new_node.leg_state);
                ChangeDiscreteLegPos(lifted_leg_list[2], k, &new_node.leg_state);

                // 次のノード用に，深さ・親・次の動作を更新する．
                new_node.ChangeToNextNode(current_node_index, next_move_);

                (*output_graph).push_back(new_node);  // 追加する．
            }
        }
    }
}

}  // namespace designlab
