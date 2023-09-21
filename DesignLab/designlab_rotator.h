#pragma once

#include <fstream>
#include <iostream>
#include <iomanip>
#include <ios>

#include "designlab_math_util.h"
#include "designlab_vector3.h"


namespace designlab
{
	//! @struct designlab::SRotator
	//! @date 2023/08/06
	//! @author 長谷川
	//! @brief 回転を表す構造体．XYZオイラー角
	//! @details XYZオイラー角によって回転を表現する．
	//! @n ロール(X軸)，ピッチ(Y軸)，ヨー(Z軸)はそれぞれ右ねじの方向に回転する．@n
	//! @n 参考資料 :  https://watako-lab.com/2019/01/23/roll_pitch_yaw/
	struct SRotator
	{
		SRotator() = default;
		constexpr SRotator(const float r, const float p, const float y) : roll(r), pitch(p), yaw(y) {};


		constexpr SRotator operator *(const float s) const { return { roll * s, pitch * s, yaw * s }; }


		constexpr bool operator ==(const SRotator& other) const
		{
			return (
				::designlab::math_util::IsEqual(roll, other.roll) && 
				::designlab::math_util::IsEqual(pitch, other.pitch) &&
				::designlab::math_util::IsEqual(yaw, other.yaw)
			);
		}
		constexpr bool operator !=(const SRotator& other) const { return !(*this == other); }


		float roll;		//!< ロール．X軸周りの回転
		float pitch;	//!< ピッチ．Y軸周りの回転
		float yaw;		//!< ヨー．Z軸周りの回転
	};


	// 出力ストリーム
	inline std::ostream& operator<<(std::ostream& os, const SRotator& r)
	{
		const int width = 10;
		const char fill = ' ';
		const int precision = 3;

		os << std::fixed << std::setprecision(precision);
		os << "(roll : ";
		os << std::setw(width) << std::setfill(fill) << r.roll;
		os << "[rad], pitch : ";
		os << std::setw(width) << std::setfill(fill) << r.pitch;
		os << "[rad], yaw : ";
		os << std::setw(width) << std::setfill(fill) << r.yaw;
		os << "[rad])";
		return os;
	}


	//入力ストリーム
	template <class Char>
	inline std::basic_istream<Char>& operator >>(std::basic_istream<Char>& is, SRotator& r)
	{
		Char unused;
		return is >> unused >> r.roll >> unused >> r.pitch >> unused >> r.yaw >> unused;
	}

	//! @brief 回転させたベクトルを返す．三角関数の処理が多く重たいので注意．
	//! @param [in] vec 位置ベクトル
	//! @param [in] rot 回転ベクトル
	//! @return Vector3 回転した後の位置ベクトル
	Vector3 rotVector(const Vector3& vec, const SRotator& rot);

}	// namespace designlab


//! @file designlab_rotator.h
//! @date 2023/08/06
//! @author 長谷川
//! @brief 回転を表す構造体．XYZオイラー角
//! @n 行数 : @lineinfo