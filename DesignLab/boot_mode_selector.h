
//! @file boot_mode_selector.h

// Copyright(c) 2023 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_BOOT_MODE_SELECTOR_H_
#define DESIGNLAB_BOOT_MODE_SELECTOR_H_

#include <type_traits>

#include "boot_mode.h"


namespace designlab
{

//! @class BootModeSelector
//! @brief 起動モードを選択するクラス．
class BootModeSelector final
{
public:
    BootModeSelector();

    //! @brief デフォルトの起動モードを設定する．
    //! @param[in] default_mode デフォルトの起動モード．
    constexpr void SetDefaultBootMode(const BootMode default_mode)
    {
        default_mode_ = default_mode;
    };

    //! @brief 起動モードを選択する．
    //! @n BootModeが int 型をもとにしているかつ，
    //! 0から始まることを前提にしているので，
    //! うまく動作しない場合は，BootModeの定義を見直すこと．
    //! @return 起動モード．
    BootMode SelectBootMode();

private:
    const int kBootModeNum;  //!< 起動モードの最大数．

    BootMode default_mode_;  //!< デフォルトの起動モード．


    // BootMode が int 型に変換可能か確かめる．
    static_assert(
        std::is_same<std::underlying_type<BootMode>::type, int>::value,
        "'BootMode' must be able to convert to int type.");
};

}  // namespace designlab


#endif  // DESIGNLAB_BOOT_MODE_SELECTOR_H_
