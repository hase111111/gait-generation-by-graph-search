//! @file discrete_com_pos.h
//! @brief 離散化された重心位置を表す列挙体


#ifndef DESIGNLAB_DISCRETE_COM_POS_H_
#define DESIGNLAB_DISCRETE_COM_POS_H_


#include <string>


//! @enum 離散化された重心位置を表す列挙型
//! @brief 重心がどこにあるかを表す．
enum class DiscreteComPos
{
	kFront = 1,		//!< 重心が前方にある
	kFrontLeft,		//!< 重心が左前方にある
	kBackLeft,		//!< 重心が左後方にある
	kBack,			//!< 重心が後方にある
	kBackRight,		//!< 重心が右後方にある
	kFrontRight,	//!< 重心が右前方にある
	kCenterFront,	//!< 重心が中央前方にある．三角形
	kCenterBack,	//!< 重心が中央後方にある．逆三角径
	ERROR_POS		//!< エラー
};



// 範囲ループをするための記述，https://sanichi999.hateblo.jp/entry/2014/12/19/011820
inline DiscreteComPos begin(DiscreteComPos) { return DiscreteComPos::kFront; };
inline DiscreteComPos end(DiscreteComPos) { return DiscreteComPos::ERROR_POS; };
inline DiscreteComPos operator*(DiscreteComPos com_pos) { return com_pos; };
inline DiscreteComPos operator++(DiscreteComPos& com_pos)
{
	return com_pos = static_cast<DiscreteComPos>(std::underlying_type<DiscreteComPos>::type(com_pos) + 1);
};


#endif	// DESIGNLAB_DISCRETE_COM_POS_H_