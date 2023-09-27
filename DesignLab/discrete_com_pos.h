//! @file discrete_com_pos.h
//! @brief 離散化された重心位置を表す列挙体


#ifndef DESIGNLAB_DISCRETE_COM_POS_H_
#define DESIGNLAB_DISCRETE_COM_POS_H_


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
};


#endif	// DESIGNLAB_DISCRETE_COM_POS_H_