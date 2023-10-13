//! @file phantomx_const.h
//! @brief PhantomXのステータスをまとめた定数クラス．

#ifndef DESIGNLAB_PHANTOMX_CONST_H_
#define DESIGNLAB_PHANTOMX_CONST_H_

#include <array>

#include "designlab_math_util.h"


class PhantomXConst final
{
public:

	PhantomXConst() = delete;
	PhantomXConst(const PhantomXConst& other) = delete;
	PhantomXConst& operator=(const PhantomXConst& other) = delete;
	PhantomXConst(PhantomXConst&& other) = delete;

	//! 第1関節の初期角度[rad]
	constexpr static std::array<float, 6> kCoxaDefaultAngle = {
		::designlab::math_util::ConvertDegToRad(-45.0f), ::designlab::math_util::ConvertDegToRad(-90.0f), ::designlab::math_util::ConvertDegToRad(-135.0f),
		::designlab::math_util::ConvertDegToRad(135.0f), ::designlab::math_util::ConvertDegToRad(90.0f),  ::designlab::math_util::ConvertDegToRad(45.0f) 
	};
};

#endif 