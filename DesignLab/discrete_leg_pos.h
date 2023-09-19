#pragma once

#include <string>


//! @enum EDiscreteLegPos
//! @date 2023/09/10
//! @author 長谷川
//! @breif 離散化された脚位置を表すenum
enum class EDiscreteLegPos
{
	LOWER_BACK = 1,		//!< 現在の位置より後方かつ下方にある
	BACK,				//!< 現在の位置より後方にある
	UPPER_BACK,			//!< 現在の位置より後方かつ上方にある
	CENTER,				//!< 現在の位置にある
	LOWER_FRONT,		//!< 現在の位置より前方かつ下方にある
	FRONT,				//!< 現在の位置より前方にある
	UPPER_FRONT,		//!< 現在の位置より前方かつ上方にある
	ERROR_POS			//!< エラー
};


namespace std
{
	std::string to_string(EDiscreteLegPos leg_pos);

}	// namespace std



//! @file discrete_leg_pos.h
//! @date 2023/09/12
//! @author 長谷川
//! @breif 離散化された脚位置を表す列挙体
