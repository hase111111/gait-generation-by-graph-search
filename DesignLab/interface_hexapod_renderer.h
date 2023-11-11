//! @file interface_hexapod_renderer.h
//! @brief 6脚ロボットの描画のインターフェース


#ifndef INTERFACE_HEXAPOD_RENDERER_H_
#define INTERFACE_HEXAPOD_RENDERER_H_

#include "robot_state_node.h"


//! @class IHexapodRenderer
//! @brief 6脚ロボットの描画のインターフェース
class IHexapodRenderer
{
public:

	virtual ~IHexapodRenderer() = default;

	//! @brief ロボットの状態を更新する．
	//! @n 更新の際に，ジョイントの計算が入る可能性があるため，計算負荷が高い．連続して呼び出さないこと．
	//! @param [in] node 描画するロボットの状態
	virtual void SetDrawNode(const RobotStateNode& node) = 0;

	//! @brief ロボットを3D空間に描画する．
	virtual void Draw() const = 0;
};


#endif	// #ifndef INTERFACE_HEXAPOD_RENDERER_H_