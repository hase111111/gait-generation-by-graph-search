//! @file boot_mode_selecter.h
//! @brief 起動モードを選択するクラス

#ifndef DESIGNLAB_BOOT_MODE_SELECTER_H_
#define DESIGNLAB_BOOT_MODE_SELECTER_H_

#include "boot_mode.h"


//! @class BootModeSelecter
//! @brief 起動モードを選択するクラス
class BootModeSelecter final
{
public:

	BootModeSelecter();

	//! @brief デフォルトの起動モードを設定する
	inline void SetDefaultBootMode(BootMode default_mode) { default_mode_ = default_mode; };

	//! @brief 起動モードを選択する
	//! @return BootMode 起動モード
	BootMode SelectBootMode();

private:
	const int kBootModeNum;	//!< 起動モードの最大数

	BootMode default_mode_;	//!< デフォルトの起動モード
};

#endif