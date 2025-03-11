﻿
//! @file node_initializer.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_NODE_INITIALIZER_H_
#define DESIGNLAB_NODE_INITIALIZER_H_

#include "robot_state_node.h"
#include "map_state.h"

namespace designlab {

//! @class NodeInitializer
//! @brief ノードの初期化を行うクラス．
//! @n シミュレーション時にノードの初期値を設定するために使用する．
class NodeInitializer final {
public:
    NodeInitializer(const Vector3& pos, const EulerXYZ& posture,
                    HexapodMove move);

    //! @brief ノードの初期化を行う．
    //! @return 初期化されたノード．
    RobotStateNode InitNode() const;

    //! @brief 地形に適した初期姿勢を設定する．
    RobotStateNode InitNodeForTerrain(const RobotStateNode& node, const MapState map) const;

private:
    const Vector3 pos_;
    const EulerXYZ posture_;
    const HexapodMove move_;
};

}  // namespace designlab


#endif  // DESIGNLAB_NODE_INITIALIZER_H_
