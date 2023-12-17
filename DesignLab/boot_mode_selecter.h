//! @file boot_mode_selecter.h
//! @brief 起動モードを選択するクラス．

#ifndef DESIGNLAB_BOOT_MODE_SELECTER_H_
#define DESIGNLAB_BOOT_MODE_SELECTER_H_

#include "boot_mode.h"


namespace designlab
{

//! @class BootModeSelecter
//! @brief 起動モードを選択するクラス．
class BootModeSelecter final
{
public:

	BootModeSelecter();

	//! @brief デフォルトの起動モードを設定する．
	//! @param[in] default_mode デフォルトの起動モード．
	constexpr void SetDefaultBootMode(const enums::BootMode default_mode)
	{
		default_mode_ = default_mode;
	};

	//! @brief 起動モードを選択する．
	//! @n BootModeがint型をもとにしているかつ，0から始まることを前提にしているので，うまく動作しない場合は，BootModeの定義を見直すこと．
	//! @return BootMode 起動モード．
	enums::BootMode SelectBootMode();

private:
	const int kBootModeNum;	//!< 起動モードの最大数．

	enums::BootMode default_mode_;	//!< デフォルトの起動モード．
};

}	// namespace designlab


#endif	// DESIGNLAB_BOOT_MODE_SELECTER_H_