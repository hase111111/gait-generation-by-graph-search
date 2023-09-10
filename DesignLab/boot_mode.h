#pragma once

#include <string>



//! @enum EBootMode
//! @date 2023/08/27
//! @author 長谷川
//! @brief 起動モードを表す列挙型
enum class EBootMode : int
{
	SIMULATION,		//!< シミュレーションモード
	VIEWER,			//!< ビューワーモード
	DISPLAY_TEST,	//!< ディスプレイテストモード
	RESULT_VIEWER	//!< リザルトビューワーモード
};



namespace std
{
	//! @brief EBootModeを文字列に変換する
	//! @param [in] boot_mode 変換するEBootMode
	//! @return EBootModeを文字列に変換したもの
	std::string to_string(const EBootMode boot_mode);



	//! @brief 文字列をEBootModeに変換する
	//! @param [in] str 変換する文字列
	//! @return 文字列をEBootModeに変換したもの
	EBootMode sToMode(const std::string str);


}	// namespace std



//! @file boot_mode.h
//! @date 2023/08/27
//! @author 長谷川
//! @brief 起動モードを表す列挙型
//! @n 行数 : @lineinfo