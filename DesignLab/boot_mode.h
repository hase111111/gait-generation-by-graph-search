//! @file boot_mode.h
//! @brief 起動モードを表す列挙型


#ifndef DESIGNLAB_BOOT_MODE_H_
#define DESIGNLAB_BOOT_MODE_H_


//! @enum BootMode
//! @brief 起動モードを表す列挙型
enum class BootMode : int
{
	kSimulation = 0,//!< シミュレーションモード
	kViewer,		//!< ビューワーモード
	kDisplayTest,	//!< ディスプレイテストモード
	kResultViewer	//!< リザルトビューワーモード
};


#endif	// DESIGNLAB_BOOT_MODE_H_