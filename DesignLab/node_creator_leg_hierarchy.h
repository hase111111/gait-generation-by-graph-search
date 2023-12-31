
//! @file      node_creator_leg_hierarchy.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_NODE_CREATOR_LEG_HIERARCHY_H_
#define DESIGNLAB_NODE_CREATOR_LEG_HIERARCHY_H_

#include <memory>
#include <vector>

#include "discrete_leg_pos.h"
#include "hexapod_next_move.h"
#include "interface_node_creator.h"

namespace designlab
{

//! @class NodeCreatorLegHierarchy
//! @brief 脚の階層構造を作るためのクラス．
class NodeCreatorLegHierarchy final : public INodeCreator
{
public:
    NodeCreatorLegHierarchy(enums::HexapodMove next_move,
                            const std::vector<enums::DiscreteLegPos>& discrete_leg_pos_list);
    ~NodeCreatorLegHierarchy() = default;

    void Create(const RobotStateNode& current_node, int current_node_index,
                std::vector<RobotStateNode>* output_nodes) const override;

private:
    //! @brief 1脚が遊脚しているとき，その脚の状態を別の状態に変更する．
    void Create1LegLifted(const RobotStateNode& current_node, int current_node_index,
                          std::vector<RobotStateNode>* output_nodes) const;

    //! @brief 2脚が遊脚しているとき，その脚の状態を別の状態に変更する．
    void Create2LegLifted(const RobotStateNode& current_node, int current_node_index,
                          std::vector<RobotStateNode>* output_nodes) const;

    //! @brief 3脚が遊脚しているとき，その脚の状態を別の状態に変更する．
    void Create3LegLifted(const RobotStateNode& current_node, int current_node_index,
                          std::vector<RobotStateNode>* output_nodes) const;


    const enums::HexapodMove next_move_;  //!< 次の動作．

    //! 離散化された脚位置のリスト，このリストの中の値から脚の状態を変更する．
    const std::vector<enums::DiscreteLegPos> discrete_leg_pos_list_;
};

}  // namespace designlab


#endif  // DESIGNLAB_NODE_CREATOR_LEG_HIERARCHY_H_
