//! @file boot_mode.h
//! @brief 起動モードを表す列挙型


#ifndef DESIGNLAB_BOOT_MODE_H_
#define DESIGNLAB_BOOT_MODE_H_


#include <string>


//! @enum BootMode
//! @brief 起動モードを表す列挙型
enum class BootMode
{
	kSimulation,	//!< シミュレーションモード
	kViewer,		//!< ビューワーモード
	kDisplayTest,	//!< ディスプレイテストモード
	kResultViewer	//!< リザルトビューワーモード
};


namespace std
{
	//! @brief EBootModeを文字列に変換する
	//! @param [in] boot_mode 変換するEBootMode
	//! @return EBootModeを文字列に変換したもの
	std::string to_string(const BootMode boot_mode);

	//! @brief 文字列をEBootModeに変換する
	//! @param [in] str 変換する文字列
	//! @return 文字列をEBootModeに変換したもの
	BootMode sToMode(const std::string str);

}	// namespace std


#endif	// DESIGNLAB_BOOT_MODE_H_