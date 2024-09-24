
//! @file boot_mode.h

// Copyright(c) 2023 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_BOOT_MODE_H_
#define DESIGNLAB_BOOT_MODE_H_


namespace designlab
{

//! @enum BootMode
//! @brief 起動モードを表す列挙型．
//! @see BootModeSelector
enum class BootMode : int
{
    kSimulation = 0,    //!< シミュレーションモード．
    kViewer,            //!< ビューワーモード．
    kDisplayModel,      //!< ディスプレイモード．
    kResultViewer,      //!< リザルトビューワーモード．
    kRobotControl,      //!< ロボットコントロールモード．
};

}  // namespace designlab


#endif  // DESIGNLAB_BOOT_MODE_H_
