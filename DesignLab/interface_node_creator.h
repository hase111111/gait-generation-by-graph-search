
//! @file interface_node_creator.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_INTERFACE_NODE_CREATOR_H_
#define DESIGNLAB_INTERFACE_NODE_CREATOR_H_

#include <vector>

#include "robot_state_node.h"


namespace designlab
{

//! @class INodeCreator
//! @brief ノード生成処理のインターフェース．
class INodeCreator
{
public:
    //! @brief コンストラクタでは次動作を設定する．またマップのポインタを受け取る．
    INodeCreator() = default;
    virtual ~INodeCreator() = default;


    //! @brief 現在のノードから次のノード群を生成する．
    //! @param[in] current_node 現在のノード．
    //! @param[in] current_node_index 現在のノードのインデックス．
    //! @param[out] output_graph 生成したノード群を返す．
    virtual void Create(const RobotStateNode& current_node, int current_node_index,
                        std::vector<RobotStateNode>* output_graph) const = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_NODE_CREATOR_H_
