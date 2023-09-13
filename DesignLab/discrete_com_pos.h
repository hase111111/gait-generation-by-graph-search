#pragma once

#include <string>


//! @enum 重心パターンの列挙型
//! @brief 重心がどこにあるかを表す．
enum class EDiscreteComPos : char
{
	FRONT = 1,		//!< 重心が前方にある
	FRONT_LEFT,		//!< 重心が左前方にある
	BACK_LEFT,		//!< 重心が左後方にある
	BACK,			//!< 重心が後方にある
	BACK_RIGHT,		//!< 重心が右後方にある
	FRONT_RIGHT,	//!< 重心が右前方にある
	CENTER_FRONT,	//!< 重心が中央前方にある．三角形
	CENTER_BACK,	//!< 重心が中央後方にある．逆三角径
	ERROR_POS		//!< エラー
};


namespace std
{

	//! @brief 離散化された重心位置を文字列で出力するための関数
	//! @param[in] com_pos 離散化された重心位置
	//! @return 離散化された重心位置を表す文字列
	std::string to_string(EDiscreteComPos com_pos);
}