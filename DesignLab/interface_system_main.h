
//! @file      interface_system_main.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_INTERFACE_SYSTEM_MAIN_H_
#define DESIGNLAB_INTERFACE_SYSTEM_MAIN_H_


namespace designlab
{

//! @class ISystemMain
//! @brief プログラムの面の処理のインターフェース．
class ISystemMain
{
public:
    virtual ~ISystemMain() = default;

    //! @brief 主要な処理を行う関数．
    virtual void Main() = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_SYSTEM_MAIN_H_
