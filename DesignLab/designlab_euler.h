//! @file designlab_euler.h
//! @brief オイラー角を用いた回転を表す構造体

#ifndef DESIGNLAB_EULER_H_
#define DESIGNLAB_EULER_H_


#include <string>

#include "designlab_math_util.h"
#include "designlab_vector3.h"


namespace designlab
{
	//! @struct designlab::EulerXYZ
	//! @brief 回転を表す構造体．XYZオイラー角．単位は [rad]
	//! @details XYZオイラー角によって回転を表現する．
	//! @n ロール(X軸)，ピッチ(Y軸)，ヨー(Z軸)はそれぞれ右ねじの方向に回転する．
	//! @n
	//! @n 参考資料 :  https://watako-lab.com/2019/01/23/roll_pitch_yaw/
	//! @n 
	//! @n 知っての通り計算時間がかなり掛かるので，速度を考えるならばクォータニオン(四元数)を用いた回転を推奨する．
	struct EulerXYZ
	{
		constexpr EulerXYZ() : x_angle(0.f), y_angle(0.f), z_angle(0.f) {};
		constexpr EulerXYZ(const float x, const float y, const float z) : x_angle(x), y_angle(y), z_angle(z) {};
		constexpr EulerXYZ(const EulerXYZ& other) = default;
		constexpr EulerXYZ(EulerXYZ&& other) noexcept = default;
		constexpr EulerXYZ& operator =(const EulerXYZ& other) = default;
		
		~EulerXYZ() = default;

		constexpr EulerXYZ operator *(const float s) const noexcept { return { x_angle * s, y_angle * s, z_angle * s }; }


		constexpr bool operator ==(const EulerXYZ& other) const noexcept
		{
			return (
				::designlab::math_util::IsEqual(x_angle, other.x_angle) && 
				::designlab::math_util::IsEqual(y_angle, other.y_angle) &&
				::designlab::math_util::IsEqual(z_angle, other.z_angle)
			);
		}
		constexpr bool operator !=(const EulerXYZ& other) const noexcept { return !(*this == other); }


		//! @brief オイラー角を 単位[deg] で初期化する
		//! @param [in] x X軸周りの回転．[deg]
		//! @param [in] y Y軸周りの回転．[deg]
		//! @param [in] z Z軸周りの回転．[deg]
		constexpr void SetDeg(const float x, const float y, const float z) 
		{
			x_angle = ::designlab::math_util::ConvertDegToRad(x);
			y_angle = ::designlab::math_util::ConvertDegToRad(y);
			z_angle = ::designlab::math_util::ConvertDegToRad(z);
		}

		//! @brief オイラー角を文字列に変換する
		//! @n 単位は ラジアン [rad]
		//! @return std::string オイラー角を表す文字列
		std::string ToString() const;

		//! @brief オイラー角を文字列に変換する
		//! @n 単位は 度 [deg]
		//! @return std::string オイラー角を表す文字列
		std::string ToStringDeg() const;


		float x_angle;	//!< X 軸周りの回転．[rad]
		float y_angle;	//!< Y 軸周りの回転．[rad]
		float z_angle;	//!< Z 軸周りの回転．[rad]
	};


	// 出力ストリーム
	template <class Char>
	inline std::basic_ostream<Char>& operator <<(std::basic_ostream<Char>& os, const EulerXYZ& r)
	{
		os << ::designlab::math_util::ConvertFloatToString(r.x_angle) << Char(',') << 
			::designlab::math_util::ConvertFloatToString(r.y_angle) << Char(',') << 
			::designlab::math_util::ConvertFloatToString(r.z_angle);

		return os;
	}

	//入力ストリーム
	template <class Char>
	inline std::basic_istream<Char>& operator >>(std::basic_istream<Char>& is, EulerXYZ& r)
	{
		Char unused;
		return is >> unused >> r.x_angle >> unused >> r.y_angle >> unused >> r.z_angle >> unused;
	}


	//! @brief 回転させたベクトルを返す．三角関数の処理が多く重たいので注意．
	//! @param [in] vec 位置ベクトル
	//! @param [in] rot 回転ベクトル
	//! @return Vector3 回転した後の位置ベクトル
	Vector3 RotateVector3(const Vector3& vec, const EulerXYZ& rot);

}	// namespace designlab


#endif // !DESIGNLAB_EULER_H_