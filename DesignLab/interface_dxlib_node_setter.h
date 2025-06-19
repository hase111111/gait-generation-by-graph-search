
//! @file interface_dxlib_node_setter.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_INTERFACE_DXLIB_NODE_SETTER_H_
#define DESIGNLAB_INTERFACE_DXLIB_NODE_SETTER_H_

#include "robot_state_node.h"

namespace designlab {

//! @class IDxlibNodeSetter
//! @brief DxLibのGuiやRendererでノードのセットを行うためのインターフェース.
class IDxlibNodeSetter {
 public:
  virtual ~IDxlibNodeSetter() = default;

  //! @brief ノードをセットする.
  //! @param node ノード番号.
  virtual void SetNode(const RobotStateNode& node) = 0;
};

}  // namespace designlab

#endif  // DESIGNLAB_INTERFACE_DXLIB_NODE_SETTER_H_
