
//! @file      interface_dxlib_node_setter.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_INTERFACE_DXLIB_NODE_SETTER_H_
#define DESIGNLAB_INTERFACE_DXLIB_NODE_SETTER_H_

#include "robot_state_node.h"


namespace designlab
{

//! @class IDxlibNodeSetter
//! @brief DxLibのGuiやRendererでノードのセットを行うためのインターフェース．
class IDxlibNodeSetter
{
public:
    virtual ~IDxlibNodeSetter() = default;

    //! @brief ノードをセットする．
    //! @param node ノード番号．
    virtual void SetNode(const RobotStateNode& node) = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_DXLIB_NODE_SETTER_H_
