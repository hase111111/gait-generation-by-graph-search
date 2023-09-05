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

	std::string to_string(const EBootMode boot_mode);



	EBootMode sToMode(const std::string str);


}	// namespace std



//! @file boot_mode.h
//! @date 2023/08/27
//! @author 長谷川
//! @brief 起動モードを表す列挙型
//! @n 行数 : @lineinfo