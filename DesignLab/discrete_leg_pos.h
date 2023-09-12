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
};



// 範囲ループをするための記述，https://sanichi999.hateblo.jp/entry/2014/12/19/011820
inline EDiscreteLegPos begin(EDiscreteLegPos) { return EDiscreteLegPos::LOWER_BACK; };
inline EDiscreteLegPos end(EDiscreteLegPos) { return EDiscreteLegPos::UPPER_FRONT; };
inline EDiscreteLegPos operator*(EDiscreteLegPos leg_pos) { return leg_pos; };
inline EDiscreteLegPos operator++(EDiscreteLegPos& leg_pos)
{
	return leg_pos = static_cast<EDiscreteLegPos>(std::underlying_type<EDiscreteLegPos>::type(leg_pos) + 1);
};



namespace std
{
	std::string to_string(EDiscreteLegPos leg_pos);

}	// namespace std



//! @file discrete_leg_pos.h
//! @date 2023/09/12
//! @author 長谷川
//! @breif 離散化された脚位置を表す列挙体
