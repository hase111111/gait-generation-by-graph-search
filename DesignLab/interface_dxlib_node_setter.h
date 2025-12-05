
//! @file interface_dxlib_node_setter.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include "robot_state_node.h"

namespace gaitgen {

//! @class IDxlibNodeSetter
//! @brief DxLibのGuiやRendererでノードのセットを行うためのインターフェース.
class IDxlibNodeSetter {
 public:
  virtual ~IDxlibNodeSetter() = default;

  //! @brief ノードをセットする.
  //! @param node ノード番号.
  virtual void SetNode(const RobotStateNode& node) = 0;
};

}  // namespace gaitgen
