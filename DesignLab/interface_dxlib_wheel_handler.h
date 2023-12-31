
//! @file      interface_dxlib_wheel_handler.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_INTERFACE_DXLIB_WHEEL_HANDLER_H_
#define DESIGNLAB_INTERFACE_DXLIB_WHEEL_HANDLER_H_


namespace designlab
{

//! @class IDxlibWheelHandler
//! @brief マウスホイールの入力を受け取るためのインターフェース．
//! @details マウスホイールの入力を受け取りたいGUIはこのインターフェースを継承する．
class IDxlibWheelHandler
{
public:
    virtual ~IDxlibWheelHandler() = default;

    //! @brief マウスホイールが回転したときの処理を行うかどうかを判定する．
    //! @return マウスホイールが回転したときの処理を行うならばtrue．
    virtual bool CanHandleWheel(int cursor_x, int cursor_y) const = 0;

    //! @brief マウスホイールが回転したときの処理を行う．
    //! @n マウスホイールが回転したときに呼び出される．
    //! @param[in] rot マウスホイールの回転量．
    //! 奥に回した分はプラスの値として，手前に回した分はマイナスの値として返る．
    virtual void RotMouseWheel(int rot) const = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_DXLIB_WHEEL_HANDLER_H_
