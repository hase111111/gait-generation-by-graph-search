
//! @file interface_system_main.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

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
